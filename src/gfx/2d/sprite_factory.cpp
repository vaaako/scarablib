#include "scarablib/gfx/2d/sprite_factory.hpp"
#include "scarablib/opengl/vao_manager.hpp"

// CW

Rectangle* SpriteFactory::new_rectangle() noexcept {
	static std::vector<Vertex> vertices = {
		// Rectangle
		Vertex { .position = glm::vec3(0.0f, 0.0f, 0.0f), .texuv = glm::vec2(0.0f, 1.0f) }, // Bottom Left
		Vertex { .position = glm::vec3(1.0f, 0.0f, 0.0f), .texuv = glm::vec2(1.0f, 1.0f) }, // Bottom Right
		Vertex { .position = glm::vec3(1.0f, 1.0f, 0.0f), .texuv = glm::vec2(1.0f, 0.0f) }, // Top Right
		Vertex { .position = glm::vec3(0.0f, 1.0f, 0.0f), .texuv = glm::vec2(0.0f, 0.0f) }, // Top Left
	};

	// Make VAO and return created Rectangle
	VAOManager::get_instance().make_vao(vertices);
	return new Rectangle(vertices);
}

Triangle* SpriteFactory::new_triangle() noexcept {
	static std::vector<Vertex> vertices = {
		// Triangle
		Vertex { .position = glm::vec3(1.0f, 1.0f, 0.0f), .texuv = glm::vec2(1.0f, 0.0f) }, // Bottom Right
		Vertex { .position = glm::vec3(0.0f, 1.0f, 0.0f), .texuv = glm::vec2(0.0f, 0.0f) }, // Bottom Left
		Vertex { .position = glm::vec3(0.5f, 0.0f, 0.0f), .texuv = glm::vec2(0.5f, 1.0f) }  // Middle
	};

	// Make VAO and return created triangle
	VAOManager::get_instance().make_vao(vertices);
	return new Triangle(vertices);
}


Circle* SpriteFactory::new_circle() noexcept {
	static std::vector<Vertex> vertices = {
		// Rectangle
		{ .position = glm::vec3(0.0f, 0.0f, 0.0f), .texuv = glm::vec2(0.0f, 1.0f) }, // Bottom Left
		{ .position = glm::vec3(1.0f, 0.0f, 0.0f), .texuv = glm::vec2(1.0f, 1.0f) }, // Bottom Right
		{ .position = glm::vec3(1.0f, 1.0f, 0.0f), .texuv = glm::vec2(1.0f, 0.0f) }, // Top Right
		{ .position = glm::vec3(0.0f, 1.0f, 0.0f), .texuv = glm::vec2(0.0f, 0.0f) },  // Top Left
	};

	// Make VAO and return created circle
	VAOManager::get_instance().make_vao(vertices);
	return new Circle(vertices);
}
