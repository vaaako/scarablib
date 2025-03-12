#include "scarablib/gfx/2d/sprite_factory.hpp"

// CW

Rectangle* SpriteFactory::create_rectangle() noexcept {
	static std::vector<Vertex> vertices = {
		// Rectangle
		Vertex { .position = glm::vec3(0.0f, 0.0f, 0.0f), .texuv = glm::vec2(0.0f, 1.0f) }, // Bottom Left
		Vertex { .position = glm::vec3(1.0f, 0.0f, 0.0f), .texuv = glm::vec2(1.0f, 1.0f) }, // Bottom Right
		Vertex { .position = glm::vec3(1.0f, 1.0f, 0.0f), .texuv = glm::vec2(1.0f, 0.0f) }, // Top Right
		Vertex { .position = glm::vec3(0.0f, 1.0f, 0.0f), .texuv = glm::vec2(0.0f, 0.0f) }, // Top Left
	};

	// Return created rectangle
	return new Rectangle(vertices); // The vao is made by mesh
}

Triangle* SpriteFactory::create_triangle() noexcept {
	static std::vector<Vertex> vertices = {
		// Triangle
		Vertex { .position = glm::vec3(1.0f, 1.0f, 0.0f), .texuv = glm::vec2(1.0f, 0.0f) }, // Bottom Right
		Vertex { .position = glm::vec3(0.0f, 1.0f, 0.0f), .texuv = glm::vec2(0.0f, 0.0f) }, // Bottom Left
		Vertex { .position = glm::vec3(0.5f, 0.0f, 0.0f), .texuv = glm::vec2(0.5f, 1.0f) }  // Middle
	};

	// Return created triangle
	return new Triangle(vertices); // The vao is made by mesh
}


Circle* SpriteFactory::create_circle() noexcept {
	static std::vector<Vertex> vertices = {
		// Rectangle
		{ .position = glm::vec3(0.0f, 0.0f, 0.0f), .texuv = glm::vec2(0.0f, 1.0f) }, // Bottom Left
		{ .position = glm::vec3(1.0f, 0.0f, 0.0f), .texuv = glm::vec2(1.0f, 1.0f) }, // Bottom Right
		{ .position = glm::vec3(1.0f, 1.0f, 0.0f), .texuv = glm::vec2(1.0f, 0.0f) }, // Top Right
		{ .position = glm::vec3(0.0f, 1.0f, 0.0f), .texuv = glm::vec2(0.0f, 0.0f) },  // Top Left
	};

	// Return created circle
	return new Circle(vertices); // The vao is made by mesh
}
