#pragma once

#include "olcPixelGameEngine.h"

#include "mesh.h"

class Application : public olc::PixelGameEngine
{
public:
	Application()
	{
		sAppName = "The Daggers of Badun!";
	}

public:
	Mesh mesh_spaceship;
	glm::mat4x4 mat_proj{};
	float fTheta = 0;
	u64 frame_count = 0;
	bool draw_wireframes = false;

	glm::vec3 v_camera_pos{};
	glm::vec3 v_look_direction{};
	f32 f_yaw{0};

	// quick inverse only for Rotation/Translation Matrices
	glm::mat4x4 matrix_QuickInverse(glm::mat4x4 &m); 
	void MultiplyMatrixVector(glm::vec3 &i, glm::vec3 &o, glm::mat4x4 &mat);
	glm::mat4x4 matrix_PointAt(glm::vec3 &pos, glm::vec3 &target, glm::vec3 &up);
	glm::mat4x4 matrix_MakeRotationY(f32 fAngleRad);
	glm::vec3 v_IntersectPlane(glm::vec3 &plane_pos, glm::vec3 &plane_normal, glm::vec3 &line_start_pos, glm::vec3 &line_end_pos);
	i32 t_clipAgainstPlane(glm::vec3 plane_pos, glm::vec3 plane_normal, Triangle &in_tri, Triangle &out_tri1, Triangle &out_tri2);

public:
	bool OnUserCreate() override;

	bool OnUserUpdate(float fElapsedTime) override;
};