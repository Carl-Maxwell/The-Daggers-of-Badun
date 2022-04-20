#include "mesh.h"

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
