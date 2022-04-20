#pragma once

#include <glm/glm.hpp>

#include "olcPixelGameEngine.h"

#include "mtypedefs.h"
#include "mlog.h"

class Application;

struct Triangle
{
	glm::vec3 vertices[3];
	float light_factor = 1.0f;

	void Triangle::draw(Application* app);
};