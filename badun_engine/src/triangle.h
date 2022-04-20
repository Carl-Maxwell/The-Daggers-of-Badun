#pragma once

struct Triangle
{
	glm::vec3 vertices[3];
	float light_factor = 1.0f;

	void draw(olc::PixelGameEngine* app) {
				// convert to olc vectors for API
				olc::vi2d a((int)vertices[0].x, (int)vertices[0].y);
				olc::vi2d b((int)vertices[1].x, (int)vertices[1].y);
				olc::vi2d c((int)vertices[2].x, (int)vertices[2].y);

				// TODO draw lines and triangles manually
				app->FillTriangle(a, b, c, olc::Pixel(255, 255, 255) * light_factor);
				// if (app->draw_wireframes) 
					app->DrawTriangle(a, b, c, olc::Pixel(0, 128, 0));
	}
};