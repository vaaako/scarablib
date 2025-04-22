#include "scarablib/gfx/geometry_factory.hpp"

std::vector<Vertex> GeometryFactory::make_cube_vertices() noexcept {
	return {
		// Vertices: 24
		// Front face
		Vertex { .position = glm::vec3(-0.5f, -0.5f, 0.5f), .texuv = glm::vec2(0.0f, 0.0f) }, // Bottom left
		Vertex { .position = glm::vec3( 0.5f, -0.5f, 0.5f), .texuv = glm::vec2(1.0f, 0.0f) }, // Bottom right
		Vertex { .position = glm::vec3( 0.5f,  0.5f, 0.5f), .texuv = glm::vec2(1.0f, 1.0f) }, // Top right
		Vertex { .position = glm::vec3(-0.5f,  0.5f, 0.5f), .texuv = glm::vec2(0.0f, 1.0f) }, // Top left

		// Back face
		Vertex { .position = glm::vec3(-0.5f, -0.5f, -0.5f), .texuv = glm::vec2(1.0f, 0.0f) }, // 4
		Vertex { .position = glm::vec3(-0.5f,  0.5f, -0.5f), .texuv = glm::vec2(1.0f, 1.0f) }, // 5
		Vertex { .position = glm::vec3( 0.5f,  0.5f, -0.5f), .texuv = glm::vec2(0.0f, 1.0f) }, // 6
		Vertex { .position = glm::vec3( 0.5f, -0.5f, -0.5f), .texuv = glm::vec2(0.0f, 0.0f) }, // 7

		// Top face
		Vertex { .position = glm::vec3(-0.5f, 0.5f, -0.5f), .texuv = glm::vec2(0.0f, 1.0f) }, // 8
		Vertex { .position = glm::vec3(-0.5f, 0.5f,  0.5f), .texuv = glm::vec2(0.0f, 0.0f) }, // 9
		Vertex { .position = glm::vec3( 0.5f, 0.5f,  0.5f), .texuv = glm::vec2(1.0f, 0.0f) }, // 10
		Vertex { .position = glm::vec3( 0.5f, 0.5f, -0.5f), .texuv = glm::vec2(1.0f, 1.0f) }, // 11

		// Bottom face
		Vertex { .position = glm::vec3(-0.5f, -0.5f, -0.5f), .texuv = glm::vec2(0.0f, 0.0f) }, // 12
		Vertex { .position = glm::vec3( 0.5f, -0.5f, -0.5f), .texuv = glm::vec2(1.0f, 0.0f) }, // 13
		Vertex { .position = glm::vec3( 0.5f, -0.5f,  0.5f), .texuv = glm::vec2(1.0f, 1.0f) }, // 14
		Vertex { .position = glm::vec3(-0.5f, -0.5f,  0.5f), .texuv = glm::vec2(0.0f, 1.0f) }, // 15

		// Left face
		Vertex { .position = glm::vec3(0.5f, -0.5f, -0.5f), .texuv = glm::vec2(1.0f, 0.0f) }, // 16
		Vertex { .position = glm::vec3(0.5f,  0.5f, -0.5f), .texuv = glm::vec2(1.0f, 1.0f) }, // 17
		Vertex { .position = glm::vec3(0.5f,  0.5f,  0.5f), .texuv = glm::vec2(0.0f, 1.0f) }, // 18
		Vertex { .position = glm::vec3(0.5f, -0.5f,  0.5f), .texuv = glm::vec2(0.0f, 0.0f) }, // 19

		// Right face
		Vertex { .position = glm::vec3(-0.5f, -0.5f, -0.5f), .texuv = glm::vec2(0.0f, 0.0f) }, // 20
		Vertex { .position = glm::vec3(-0.5f, -0.5f,  0.5f), .texuv = glm::vec2(1.0f, 0.0f) }, // 21
		Vertex { .position = glm::vec3(-0.5f,  0.5f,  0.5f), .texuv = glm::vec2(1.0f, 1.0f) }, // 22
		Vertex { .position = glm::vec3(-0.5f,  0.5f, -0.5f), .texuv = glm::vec2(0.0f, 1.0f) }  // 23
	};
}

std::vector<Vertex> GeometryFactory::make_plane_vertices() noexcept {
	return {
		// Front face
		Vertex { .position = glm::vec3(-0.5f, -0.5f, 0.0f), .texuv = glm::vec2(0.0f, 0.0f) }, // Bottom left
		Vertex { .position = glm::vec3( 0.5f, -0.5f, 0.0f), .texuv = glm::vec2(1.0f, 0.0f) }, // Bottom right
		Vertex { .position = glm::vec3( 0.5f,  0.5f, 0.0f), .texuv = glm::vec2(1.0f, 1.0f) }, // Top right
		Vertex { .position = glm::vec3(-0.5f,  0.5f, 0.0f), .texuv = glm::vec2(0.0f, 1.0f) }, // Top left
	};
}


std::vector<Vertex> GeometryFactory::make_rectangle_vertices() noexcept {
	return {
		Vertex { .position = glm::vec3(0.0f, 0.0f, 0.0f), .texuv = glm::vec2(0.0f, 1.0f) }, // Bottom Left
		Vertex { .position = glm::vec3(1.0f, 0.0f, 0.0f), .texuv = glm::vec2(1.0f, 1.0f) }, // Bottom Right
		Vertex { .position = glm::vec3(1.0f, 1.0f, 0.0f), .texuv = glm::vec2(1.0f, 0.0f) }, // Top Right
		Vertex { .position = glm::vec3(0.0f, 1.0f, 0.0f), .texuv = glm::vec2(0.0f, 0.0f) }, // Top Left
	};
}

std::vector<Vertex> GeometryFactory::make_triangle_vertices() noexcept {
	return {
		Vertex { .position = glm::vec3(1.0f, 1.0f, 0.0f), .texuv = glm::vec2(1.0f, 0.0f) }, // Bottom Right
		Vertex { .position = glm::vec3(0.0f, 1.0f, 0.0f), .texuv = glm::vec2(0.0f, 0.0f) }, // Bottom Left
		Vertex { .position = glm::vec3(0.5f, 0.0f, 0.0f), .texuv = glm::vec2(0.5f, 1.0f) }  // Middle
	};
}
