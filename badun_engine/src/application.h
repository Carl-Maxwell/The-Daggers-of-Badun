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
	bool draw_wireframes = false;
	
	glm::vec3 v_camera_pos{};

	void MultiplyMatrixVector(glm::vec3 &i, glm::vec3 &o, glm::mat4x4 &mat);

public:
	bool OnUserCreate() override;

	bool OnUserUpdate(float fElapsedTime) override;
};