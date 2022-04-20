#pragma once

#include <fstream>
#include <strstream>
#include <filesystem>

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
