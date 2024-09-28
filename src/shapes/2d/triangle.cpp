#include "scarablib/shapes/2d/triangle.hpp"

Triangle::Triangle()
	: Shape2D({
			Vertex { .position2d = glm::vec2(1.0f, 1.0f), .texuv = glm::vec2(0.0f, 0.0f) }, // Bottom Right
			Vertex { .position2d = glm::vec2(0.0f, 1.0f), .texuv = glm::vec2(1.0f, 0.0f) }, // Bottom Left
			Vertex { .position2d = glm::vec2(0.5f, 0.0f), .texuv = glm::vec2(0.5f, 1.0f) }  // Middle
		},
		{
			0, 1, 2
		}) {}

