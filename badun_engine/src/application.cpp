
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "mtypedefs.h"
#include "mlog.h"

#include "application.h"

void Application::MultiplyMatrixVector(glm::vec3 &i, glm::vec3 &o, glm::mat4x4 &mat)
{
	o.x = i.x * mat[0][0] + i.y * mat[1][0] + i.z * mat[2][0] + mat[3][0];
	o.y = i.x * mat[0][1] + i.y * mat[1][1] + i.z * mat[2][1] + mat[3][1];
	o.z = i.x * mat[0][2] + i.y * mat[1][2] + i.z * mat[2][2] + mat[3][2];
	float w = i.x * mat[0][3] + i.y * mat[1][3] + i.z * mat[2][3] + mat[3][3];

	if (w != 0.0f)
	{
		o.x /= w; o.y /= w; o.z /= w;
	}
}

glm::mat4x4 Application::matrix_PointAt(glm::vec3 &pos, glm::vec3 &target, glm::vec3 &up)
{
	// Calculate new forward direction
	glm::vec3 new_forward = target - pos;
	new_forward = glm::normalize(new_forward);

	// Calculate new up direction
	glm::vec3 a = new_forward * glm::dot(up, new_forward);
	glm::vec3 new_up = up - a;
	new_up = glm::normalize(new_up);

	// Calculate new right direction
	glm::vec3 new_right = glm::cross(new_up, new_forward);

	// construct "dimensioning and translation matrix"
	glm::mat4x4 matrix;
	matrix[0][0] = new_right.x;   matrix[0][1] = new_right.y;   matrix[0][2] = new_right.z;   matrix[0][3] = 0.0f;
	matrix[1][0] = new_up.x;      matrix[1][1] = new_up.y;      matrix[1][2] = new_up.z;      matrix[1][3] = 0.0f;
	matrix[2][0] = new_forward.x; matrix[2][1] = new_forward.y; matrix[2][2] = new_forward.z; matrix[2][3] = 0.0f;
	matrix[3][0] = pos.x;         matrix[3][1] = pos.y;         matrix[3][2] = pos.z;         matrix[3][3] = 1.0f;
	return matrix;
}

bool Application::OnUserCreate()
{
	// mesh_spaceship.loadFromObjectFile("resources/teapot.obj");
	// mesh_spaceship.loadFromObjectFile("resources/video_ship.obj");
	mesh_spaceship.loadFromObjectFile("resources/axis.obj");

	// Project Matrix

	float fNear = 0.1f;
	float fFar = 1000.0f;
	float fFov = 90.0f;
	float fAspectRatio = (float)ScreenHeight() / (float)ScreenWidth();
	float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);

	mat_proj = glm::mat4x4();

	mat_proj = glm::perspectiveLH_ZO(fFov, fAspectRatio, fNear, fFar);

	Log::log_mat4x4(mat_proj);

	mat_proj[0][0] = fAspectRatio * fFovRad;
	mat_proj[1][1] = fFovRad;
	mat_proj[2][2] = fFar / (fFar - fNear);
	mat_proj[3][2] = (-fFar * fNear) / (fFar - fNear);
	mat_proj[2][3] = 1.0f;
	mat_proj[3][3] = 0.0f;

	Log::log_mat4x4(mat_proj);

	// Called once at the start, so create things here
	return true;
}

bool Application::OnUserUpdate(float fElapsedTime)
{
	Clear(olc::Pixel(0, 0, 0));

	// Log::white("Beginning frame {0}", frame_count);
	frame_count++;

	if (GetKey(olc::ESCAPE).bHeld)
		olc_Terminate();
	if (GetKey(olc::K1).bPressed)
		draw_wireframes = !draw_wireframes;
	if (GetKey(olc::E).bHeld)
		v_camera_pos.y += 8.0f * fElapsedTime;
	if (GetKey(olc::Q).bHeld)
		v_camera_pos.y -= 8.0f * fElapsedTime;
	if (GetKey(olc::D).bHeld)
		v_camera_pos.x += 8.0f * fElapsedTime;
	if (GetKey(olc::A).bHeld)
		v_camera_pos.x -= 8.0f * fElapsedTime;
	if (GetKey(olc::W).bHeld)
		v_camera_pos +=  v_look_direction * 8.0f * fElapsedTime;
	if (GetKey(olc::S).bHeld)
		v_camera_pos -=  v_look_direction * 8.0f * fElapsedTime;

	if (GetKey(olc::RIGHT).bHeld)
		f_yaw += 2.0f * fElapsedTime;
	if (GetKey(olc::LEFT).bHeld)
		f_yaw -= 2.0f * fElapsedTime;

	// Log::green("camera position: {0}, {1}, {2}", v_camera_pos.x, v_camera_pos.y, v_camera_pos.z);

	f_yaw;

	glm::mat4x4 mat_rot_z{}, mat_rot_x{};
	// fTheta += fElapsedTime;

	// Rotation Z
	mat_rot_z[0][0] =  cosf(fTheta);
	mat_rot_z[0][1] =  sinf(fTheta);
	mat_rot_z[1][0] = -sinf(fTheta);
	mat_rot_z[1][1] =  cosf(fTheta);
	mat_rot_z[2][2] = 1;
	mat_rot_z[3][3] = 1;

	// Rotation X
	mat_rot_x[0][0] = 1;
	mat_rot_x[1][1] =  cosf(fTheta * 0.5f);
	mat_rot_x[1][2] =  sinf(fTheta * 0.5f);
	mat_rot_x[2][1] = -sinf(fTheta * 0.5f);
	mat_rot_x[2][2] =  cosf(fTheta * 0.5f);
	mat_rot_x[3][3] = 1;

	glm::vec3 v_up = { 0, 1, 0 };
	glm::vec3 v_target = { 0, 0, 1 };
	glm::mat4x4 m_camera_rot = matrix_MakeRotationY(f_yaw);
	// v_look_direction = m_camera_rot * v_target; // TODO convert vec3s to vec4s then this'll replace matrix_multiplymatrixvector
	MultiplyMatrixVector(v_target, v_look_direction, m_camera_rot);
	v_target = v_camera_pos + v_look_direction;

	glm::mat4x4 m_camera = matrix_PointAt(v_camera_pos, v_target, v_up);

	glm::mat4x4 m_view = matrix_QuickInverse(m_camera);

	std::vector<Triangle> v_triangles_to_raster;

	for (auto tri : mesh_spaceship.tris)
	{
		Triangle tri_translated;
		{
			// Rotate mesh
			Triangle tri_rotated_z, tri_rotated_zx;
			for (i32 i = 0; i < 3; i++)
			{
				MultiplyMatrixVector(tri.vertices[i], tri_rotated_z.vertices[i], mat_rot_z);
			}

			for (i32 i = 0; i < 3; i++)
			{
				MultiplyMatrixVector(tri_rotated_z.vertices[i], tri_rotated_zx.vertices[i], mat_rot_x);
			}

			tri_translated = tri_rotated_zx;

			// translate mesh
			for (i32 i = 0; i < 3; i++)
			{
				tri_translated.vertices[i].z += 8.0f;
			}
		}

		// calculate surface normal
		glm::vec3 normal;
		{
			glm::vec3 line1, line2;
			line1 = tri_translated.vertices[1] - tri_translated.vertices[0];
			line2 = tri_translated.vertices[2] - tri_translated.vertices[0];
			
			normal = glm::normalize(glm::cross(line1, line2));
		}

		if (glm::dot(glm::normalize(tri_translated.vertices[0] - v_camera_pos), normal) < 0) {
			// Illumination
			float light_factor;
			{
				glm::vec3 light_direction = {0, 0, -1};
				light_direction = glm::normalize(light_direction);

				light_factor = glm::dot(light_direction, normal);
			}

			// Convert world space -> view space
			{
				Triangle t_viewed{};
				for (i32 i = 0; i < 3; i++)
				{
					MultiplyMatrixVector(tri_translated.vertices[i], t_viewed.vertices[i], m_view);
				}

				tri_translated = t_viewed;
			}
			
			tri_translated.color = tri.color;

			// Clip
			i32 n_clipped_tris = 0;
			Triangle clipped[2] = {};

			n_clipped_tris = t_clipAgainstPlane({0.0f, 0.0f, 0.1f}, {0.0f, 0.0f, 1.0f}, tri_translated, clipped[0], clipped[1]);
			for (i32 n = 0; n < n_clipped_tris; n++)
			{
				Triangle tri_projected;

				// project triangles from 3d -> 2d
				MultiplyMatrixVector(clipped[n].vertices[0], tri_projected.vertices[0], mat_proj);
				MultiplyMatrixVector(clipped[n].vertices[1], tri_projected.vertices[1], mat_proj);
				MultiplyMatrixVector(clipped[n].vertices[2], tri_projected.vertices[2], mat_proj);

				// scale into view
				for (i32 i = 0; i < 3; i++)
				{
					tri_projected.vertices[i] += 1.0f;
					tri_projected.vertices[i] *= 0.5f * glm::vec3((float)ScreenWidth(), (float)ScreenHeight(), 2.0f);
				}

				tri_projected.light_factor = light_factor;
				tri_projected.color = clipped[n].color;

				v_triangles_to_raster.push_back(tri_projected);
			}
		}
	}

	// Sort triangles from back to front
	std::sort(
		v_triangles_to_raster.begin(),
		v_triangles_to_raster.end(),
		[](Triangle &a, Triangle &b)
		{
			float a_z_sum = a.vertices[0].z + a.vertices[1].z + a.vertices[2].z;
			float b_z_sum = b.vertices[0].z + b.vertices[1].z + b.vertices[2].z;

			return a_z_sum > b_z_sum;
		} );

	for (auto tri : v_triangles_to_raster)
	{
		tri.draw(this);
	}

	// Log::white("Ending frame {0}", frame_count);

	return true;
}

//
// 
//

glm::mat4x4 Application::matrix_MakeRotationY(f32 fAngleRad)
{
	glm::mat4x4 matrix{};
	matrix[0][0] = cosf(fAngleRad);
	matrix[0][2] = sinf(fAngleRad);
	matrix[2][0] = -sinf(fAngleRad);
	matrix[1][1] = 1.0f;
	matrix[2][2] = cosf(fAngleRad);
	matrix[3][3] = 1.0f;
	return matrix;
}

glm::mat4x4 Application::matrix_QuickInverse(glm::mat4x4 &m)
{
	glm::mat4x4 matrix{};
	matrix[0][0] = m[0][0]; matrix[0][1] = m[1][0]; matrix[0][2] = m[2][0]; matrix[0][3] = 0.0f;
	matrix[1][0] = m[0][1]; matrix[1][1] = m[1][1]; matrix[1][2] = m[2][1]; matrix[1][3] = 0.0f;
	matrix[2][0] = m[0][2]; matrix[2][1] = m[1][2]; matrix[2][2] = m[2][2]; matrix[2][3] = 0.0f;

	matrix[3][0] = -(m[3][0] * matrix[0][0] + m[3][1] * matrix[1][0] + m[3][2] * matrix[2][0]);
	matrix[3][1] = -(m[3][0] * matrix[0][1] + m[3][1] * matrix[1][1] + m[3][2] * matrix[2][1]);
	matrix[3][2] = -(m[3][0] * matrix[0][2] + m[3][1] * matrix[1][2] + m[3][2] * matrix[2][2]);
	matrix[3][3] = 1.0f;

	return matrix;
}

// returns the point of intersection with the plane ... I think
glm::vec3 Application::v_IntersectPlane(
	glm::vec3 &plane_pos,
	glm::vec3 &plane_normal,
	glm::vec3 &line_start_pos,
	glm::vec3 &line_end_pos
) {
	plane_normal = glm::normalize(plane_normal);
	f32 plane_dot = glm::dot(plane_normal, plane_pos);
	f32 ad = glm::dot(line_start_pos, plane_normal);
	f32 bd = glm::dot(line_end_pos, plane_normal);
	f32 t  = (plane_dot - ad) / (bd - ad);

	glm::vec3 line_diff = line_end_pos - line_start_pos;
	glm::vec3 lineToIntersect = line_diff * t;

	return line_start_pos + lineToIntersect;
}

i32 Application::t_clipAgainstPlane(
	vec3 plane_pos,
	vec3 plane_normal,
	Triangle &in_tri,
	Triangle &out_tri1,
	Triangle &out_tri2
) {
	plane_normal = normalize(plane_normal);

	// Return shortest distance from point to plane
	auto dist = [plane_normal, plane_pos](vec3 &p)
	{
		vec3 n = normalize(p);
		return (
			  plane_normal.x * p.x
			+ plane_normal.y * p.y
			+ plane_normal.z * p.z
			- dot(plane_normal, plane_pos));
	};

	// classify points as either good side or bad side of plane
	// if distance sign is positive, point lies on good side of plane
	vec3* good_points[3]; i32 n_good_point_count = 0;
	vec3* bad_points[3] ; i32 n_bad_point_count  = 0;

	// get distance of each point in triangle to plane
	f32 d0 = dist(in_tri.vertices[0]);
	f32 d1 = dist(in_tri.vertices[1]);
	f32 d2 = dist(in_tri.vertices[2]);

	if (d0 >= 0) {
		good_points[n_good_point_count++] = &in_tri.vertices[0];
	} else {
		bad_points[n_bad_point_count++  ] = &in_tri.vertices[0];
	}
	if (d1 >= 0) {
		good_points[n_good_point_count++] = &in_tri.vertices[1];
	} else {
		bad_points[n_bad_point_count++  ] = &in_tri.vertices[1];
	}
	if (d2 >= 0) {
		good_points[n_good_point_count++] = &in_tri.vertices[2];
	} else {
		bad_points[n_bad_point_count++  ] = &in_tri.vertices[2];
	}

	// now classify classify triangle points,
	//   and break the input triangle into smaller output triangles if required

	if (n_good_point_count == 0)
	{
		// triangle is totally clipped. The returned triangles are not valid
		return 0;
	}
	else if (n_good_point_count == 3)
	{
		// triangle is good, not clipped by plane at all
		out_tri1 = in_tri;

		return 1; // pass the input triangle through unchanged
	}
	else if (n_good_point_count == 1 && n_bad_point_count == 2)
	{
		// only one good point,
		//   make a new triangle from it and the two intersecting points along the plane

		out_tri1.light_factor = in_tri.light_factor;
		out_tri1.color = in_tri.color;

		// take the good point
		out_tri1.vertices[0] = *good_points[0];

		// get the points of intersection with the plane, add them to the new tri
		out_tri1.vertices[1] = v_IntersectPlane(plane_pos, plane_normal, *good_points[0], *bad_points[0]);
		out_tri1.vertices[2] = v_IntersectPlane(plane_pos, plane_normal, *good_points[0], *bad_points[1]);

		return 1;
	} else if (n_good_point_count == 2 && n_bad_point_count == 1)
	{
		// Need to clip and make a quad from 2 good points & 2 intersections with the plane

		out_tri1.light_factor = in_tri.light_factor;
		out_tri2.light_factor = in_tri.light_factor;

		out_tri1.color = in_tri.color;
		out_tri2.color = in_tri.color;

		vec3 intersect0 = v_IntersectPlane(plane_pos, plane_normal, *good_points[0], *bad_points[0]);;
		vec3 intersect1 = v_IntersectPlane(plane_pos, plane_normal, *good_points[1], *bad_points[0]);;

		out_tri1.vertices[0] = *good_points[0];
		out_tri1.vertices[1] = *good_points[1];
		out_tri1.vertices[2] = intersect0;

		out_tri2.vertices[0] = *good_points[1];
		out_tri2.vertices[1] = intersect0;
		out_tri2.vertices[2] = intersect1;

		return 2;
	}

}
