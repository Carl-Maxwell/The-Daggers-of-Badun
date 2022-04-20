#include <vector>

#include <fstream>
#include <strstream>
#include <filesystem>

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "mtypedefs.h"
#include "mlog.h"

// get vec 3 from the glm lib
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Triangle
{
	glm::vec3 vertices[3];
	float light_factor = 1.0f;

	void draw(olc::PixelGameEngine* app) {
				// convert to olc vectors for API
				olc::vi2d a(vertices[0].x, vertices[0].y);
				olc::vi2d b(vertices[1].x, vertices[1].y);
				olc::vi2d c(vertices[2].x, vertices[2].y);

				// TODO draw lines and triangles manually
				app->FillTriangle(a, b, c, olc::Pixel(255, 255, 255) * light_factor);
				app->DrawTriangle(a, b, c, olc::Pixel(0, 128, 0));
	}
};

struct Mesh
{
	std::vector<Triangle> tris;

	bool loadFromObjectFile(std::string s_filename)
	{
		std::ifstream f(s_filename);
		if (!f.is_open()) {
			Log::red("loadFromObjectFile: Unable to load {0}", s_filename);
			Log::white("loadFromObjectFile: Current working directory: {0}", std::filesystem::current_path());
			return false;
		}

		// Local cache of vertices
		std::vector<glm::vec3> file_vertices;

		while (!f.eof()) 
		{
			char line[128];
			f.getline(line, 128);

			std::strstream s;
			s << line;

			char junk;

			if (line[0] == 'v')
			{
				glm::vec3 v;
				s >> junk >> v.x >> v.y >> v.z;
				file_vertices.push_back(v);
			} 
			else if (line[0] == 'f') 
			{
				int f[3];
				s >> junk >> f[0] >> f[1] >> f[2];
				tris.push_back( {
					file_vertices[ f[0] -1 ],
					file_vertices[ f[1] -1 ],
					file_vertices[ f[2] -1 ]
				} );
			}
		}

		return true;
	}
};

class Application : public olc::PixelGameEngine
{
public:
	Application()
	{
		sAppName = "The Daggers of Badun!";
	}

private:
	Mesh mesh_spaceship;
	glm::mat4x4 mat_proj{};
	float fTheta = 0;
	
	glm::vec3 v_camera_pos{};

	void MultiplyMatrixVector(glm::vec3 &i, glm::vec3 &o, glm::mat4x4 &mat)
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

public:
	bool OnUserCreate() override
	{
		mesh_spaceship.tris = {};

		mesh_spaceship.loadFromObjectFile("resources/video_ship.obj");

		// Project Matrix

		float fNear = 0.1f;
		float fFar  = 1000.0f;
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

	bool OnUserUpdate(float fElapsedTime) override
	{
		// Log::log("time1: {0}", fElapsedTime);

		Clear(olc::Pixel(0, 0, 0));

		glm::mat4x4 mat_rot_z{}, mat_rot_x{};
		fTheta += fElapsedTime;

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

				Triangle tri_projected;

				// project triangles from 3d -> 2d
				MultiplyMatrixVector(tri_translated.vertices[0], tri_projected.vertices[0], mat_proj);
				MultiplyMatrixVector(tri_translated.vertices[1], tri_projected.vertices[1], mat_proj);
				MultiplyMatrixVector(tri_translated.vertices[2], tri_projected.vertices[2], mat_proj);

				// scale into view
				for (i32 i = 0; i < 3; i++) 
				{
					tri_projected.vertices[i] += 1.0f;
					tri_projected.vertices[i] *= 0.5f * glm::vec3((float)ScreenWidth(), (float)ScreenHeight(), 2.0f);
				}

				tri_projected.light_factor = light_factor;

				v_triangles_to_raster.push_back(tri_projected);
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

		// for (int x = 0; x < ScreenWidth(); x++)
		// 	for (int y = 0; y < ScreenHeight(); y++)
		// 		Draw(x, y, olc::Pixel(rand() % 255, rand() % 255, rand()% 255));	
		return true;
	}
};

Mesh createCube() 
{
	Mesh cube;
	cube.tris = {
		// south
		{glm::vec3{0, 0, 0},    glm::vec3{0, 1, 0},    glm::vec3{1, 1, 0}},
		{glm::vec3{0, 0, 0},    glm::vec3{1, 1, 0},    glm::vec3{1, 0, 0}},

		// east
		{glm::vec3{1, 0, 0},    glm::vec3{1, 1, 0},    glm::vec3{1, 1, 1}},
		{glm::vec3{1, 0, 0},    glm::vec3{1, 1, 1},    glm::vec3{1, 0, 1}},

		// north
		{glm::vec3{1, 0, 1},    glm::vec3{1, 1, 1},    glm::vec3{0, 1, 1}},
		{glm::vec3{1, 0, 1},    glm::vec3{0, 1, 1},    glm::vec3{0, 0, 1}},

		// west
		{glm::vec3{0, 0, 1},    glm::vec3{0, 1, 1},    glm::vec3{0, 1, 0}},
		{glm::vec3{0, 0, 1},    glm::vec3{0, 1, 0},    glm::vec3{0, 0, 0}},

		// top
		{glm::vec3{0, 1, 0},    glm::vec3{0, 1, 1},    glm::vec3{1, 1, 1}},
		{glm::vec3{0, 1, 0},    glm::vec3{1, 1, 1},    glm::vec3{1, 1, 0}},

		// bottom
		{glm::vec3{1, 0, 1},    glm::vec3{0, 0, 1},    glm::vec3{0, 0, 0}},
		{glm::vec3{1, 0, 1},    glm::vec3{0, 0, 0},    glm::vec3{1, 0, 0}},
	};
	return cube;
}

i32 main() {
	Application demo;
	auto success = demo.Construct(256, 240, 4, 4);
	if (success)
		demo.Start();
	else
		Log::red("pixel game engine did not initialize! rcode: {0}", success);

	return 0;
}

