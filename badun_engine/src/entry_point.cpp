#include <vector>

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
};

struct Mesh
{
	std::vector<Triangle> tris;
};

class Application : public olc::PixelGameEngine
{
public:
	Application()
	{
		sAppName = "The Daggers of Badun!";
	}

private:
	Mesh mesh_cube;
	glm::mat4x4 mat_proj;

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
		mesh_cube.tris = {

			// south
			{glm::vec3{0, 0, 0},    glm::vec3{0, 1, 0},    glm::vec3{1, 1, 0}},
			{glm::vec3{0, 0, 0},    glm::vec3{1, 1, 0},    glm::vec3{1, 0, 0}},

			// east
			{glm::vec3{1, 0, 0},    glm::vec3{1, 1, 0},    glm::vec3{1, 1, 1}},
			{glm::vec3{1, 0, 0},    glm::vec3{1, 1, 1},    glm::vec3{1, 0, 1}},

			// north
			{glm::vec3{1, 0, 1},    glm::vec3{1, 1, 1},    glm::vec3{1, 1, 1}},
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
		
		for (auto tri : mesh_cube.tris) 
		{
			Triangle tri_projected, tri_translated;

			tri_translated = tri;
			for (i32 i = 0; i < 3; i++) 
			{
				tri_translated.vertices[i].z = tri.vertices[i].z + 3.0f;
			}

			MultiplyMatrixVector(tri_translated.vertices[0], tri_projected.vertices[0], mat_proj);
			MultiplyMatrixVector(tri_translated.vertices[1], tri_projected.vertices[1], mat_proj);
			MultiplyMatrixVector(tri_translated.vertices[2], tri_projected.vertices[2], mat_proj);

			for (i32 i = 0; i < 3; i++) 
			{
				tri_projected.vertices[i] += 1.0f;
				tri_projected.vertices[i] *= 0.5f * glm::vec3((float)ScreenWidth(), (float)ScreenHeight(), 2.0f);
			}

			olc::vi2d a(tri_projected.vertices[0].x, tri_projected.vertices[0].y);
			olc::vi2d b(tri_projected.vertices[1].x, tri_projected.vertices[1].y);
			olc::vi2d c(tri_projected.vertices[2].x, tri_projected.vertices[2].y);

			// TODO draw lines and triangles manually

			DrawTriangle(a, b, c, olc::Pixel(0, 255, 0));

		}

		// for (int x = 0; x < ScreenWidth(); x++)
		// 	for (int y = 0; y < ScreenHeight(); y++)
		// 		Draw(x, y, olc::Pixel(rand() % 255, rand() % 255, rand()% 255));	
		return true;
	}
};

i32 main() {
	Application demo;
	auto success = demo.Construct(256, 240, 4, 4);
	if (success)
		demo.Start();
	else
		Log::red("pixel game engine did not initialize! rcode: {0}", success);

	return 0;
}

