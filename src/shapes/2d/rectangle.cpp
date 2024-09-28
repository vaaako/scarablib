#include "scarablib/shapes/2d/rectangle.hpp"
#include <glm/ext/vector_float3.hpp>

Rectangle::Rectangle()
	: Shape2D({
			Vertex { .position2d = glm::vec2(0.0f, 0.0f), .texuv = glm::vec2(1.0f, 1.0f) }, // Bottom Left
			Vertex { .position2d = glm::vec2(1.0f, 0.0f), .texuv = glm::vec2(0.0f, 1.0f) }, // Bottom Right
			Vertex { .position2d = glm::vec2(1.0f, 1.0f), .texuv = glm::vec2(0.0f, 0.0f) }, // Top Right
			Vertex { .position2d = glm::vec2(0.0f, 1.0f), .texuv = glm::vec2(1.0f, 0.0f) }  // Top Left
		},
		{
			0, 1, 2, // First triangle
			2, 3, 0  // Second triangle
		}) {}

