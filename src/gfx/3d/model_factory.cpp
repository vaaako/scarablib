#include "scarablib/gfx/3d/model_factory.hpp"
#include "scarablib/opengl/vao_manager.hpp"

Cube* ModelFactory::new_cube() noexcept {
	static std::vector<Vertex> vertices = {
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

		// Right face
		Vertex { .position = glm::vec3(0.5f, -0.5f, -0.5f), .texuv = glm::vec2(1.0f, 0.0f) }, // 16
		Vertex { .position = glm::vec3(0.5f,  0.5f, -0.5f), .texuv = glm::vec2(1.0f, 1.0f) }, // 17
		Vertex { .position = glm::vec3(0.5f,  0.5f,  0.5f), .texuv = glm::vec2(0.0f, 1.0f) }, // 18
		Vertex { .position = glm::vec3(0.5f, -0.5f,  0.5f), .texuv = glm::vec2(0.0f, 0.0f) }, // 19

		// Left face
		Vertex { .position = glm::vec3(-0.5f, -0.5f, -0.5f), .texuv = glm::vec2(0.0f, 0.0f) }, // 20
		Vertex { .position = glm::vec3(-0.5f, -0.5f,  0.5f), .texuv = glm::vec2(1.0f, 0.0f) }, // 21
		Vertex { .position = glm::vec3(-0.5f,  0.5f,  0.5f), .texuv = glm::vec2(1.0f, 1.0f) }, // 22
		Vertex { .position = glm::vec3(-0.5f,  0.5f, -0.5f), .texuv = glm::vec2(0.0f, 1.0f) }  // 23
	};

	static std::vector<uint32> indices = {
		// Front face
		0, 1, 2,
		0, 2, 3,

		// Back face
		4, 5, 6,
		4, 6, 7,

		// Top face
		8, 9, 10,
		8, 10, 11,

		// Bottom face
		12, 13, 14,
		12, 14, 15,

		// Right face
		16, 17, 18,
		16, 18, 19,

		// Left face
		20, 21, 22,
		20, 22, 23
	};

	// Make VAO and return created Cube
	VAOManager::get_instance().make_vao(vertices, indices);
	return new Cube(vertices, indices);
}

Plane* ModelFactory::new_plane() noexcept {
	static std::vector<Vertex> vertices = {
		// Front face
		Vertex { .position = glm::vec3(-0.5f, -0.5f, 0.0f), .texuv = glm::vec2(0.0f, 0.0f) }, // Bottom left
		Vertex { .position = glm::vec3( 0.5f, -0.5f, 0.0f), .texuv = glm::vec2(1.0f, 0.0f) }, // Bottom right
		Vertex { .position = glm::vec3( 0.5f,  0.5f, 0.0f), .texuv = glm::vec2(1.0f, 1.0f) }, // Top right
		Vertex { .position = glm::vec3(-0.5f,  0.5f, 0.0f), .texuv = glm::vec2(0.0f, 1.0f) }, // Top left
	};

	static std::vector<uint32> indices = {
		// Front face
		0, 1, 2,
		0, 2, 3,
	};

	// Make VAO and return created Plane
	VAOManager::get_instance().make_vao(vertices, indices);
	return new Plane(vertices, indices);
}


Billboard* ModelFactory::new_billboard() noexcept {
	static std::vector<Vertex> vertices = {
		// Front face
		Vertex { .position = glm::vec3(-0.5f, -0.5f, 0.0f), .texuv = glm::vec2(0.0f, 0.0f) }, // Bottom left
		Vertex { .position = glm::vec3( 0.5f, -0.5f, 0.0f), .texuv = glm::vec2(1.0f, 0.0f) }, // Bottom right
		Vertex { .position = glm::vec3( 0.5f,  0.5f, 0.0f), .texuv = glm::vec2(1.0f, 1.0f) }, // Top right
		Vertex { .position = glm::vec3(-0.5f,  0.5f, 0.0f), .texuv = glm::vec2(0.0f, 1.0f) }, // Top left
	};

	static std::vector<uint32> indices = {
		// Front face
		0, 1, 2,
		0, 2, 3,
	};

	// Make VAO and return created billboard
	VAOManager::get_instance().make_vao(vertices, indices);
	return new Billboard(vertices, indices);
}
