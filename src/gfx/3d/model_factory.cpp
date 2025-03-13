#include "scarablib/gfx/3d/model_factory.hpp"

Cube* ModelFactory::create_cube() noexcept {
	std::vector<Vertex> vertices = {
		// Vertices: 24
		// Front face
		Vertex { .position = glm::vec3(-0.5f, -0.5f, 0.5f), .texuv = glm::vec3(0.0f, 0.0f, 0.0f) }, // Bottom left
		Vertex { .position = glm::vec3( 0.5f, -0.5f, 0.5f), .texuv = glm::vec3(1.0f, 0.0f, 0.0f) }, // Bottom right
		Vertex { .position = glm::vec3( 0.5f,  0.5f, 0.5f), .texuv = glm::vec3(1.0f, 1.0f, 0.0f) }, // Top right
		Vertex { .position = glm::vec3(-0.5f,  0.5f, 0.5f), .texuv = glm::vec3(0.0f, 1.0f, 0.0f) }, // Top left

		// Back face
		Vertex { .position = glm::vec3(-0.5f, -0.5f, -0.5f), .texuv = glm::vec3(1.0f, 0.0f, 0.0f) }, // 4
		Vertex { .position = glm::vec3(-0.5f,  0.5f, -0.5f), .texuv = glm::vec3(1.0f, 1.0f, 0.0f) }, // 5
		Vertex { .position = glm::vec3( 0.5f,  0.5f, -0.5f), .texuv = glm::vec3(0.0f, 1.0f, 0.0f) }, // 6
		Vertex { .position = glm::vec3( 0.5f, -0.5f, -0.5f), .texuv = glm::vec3(0.0f, 0.0f, 0.0f) }, // 7

		// Top face
		Vertex { .position = glm::vec3(-0.5f, 0.5f, -0.5f), .texuv = glm::vec3(0.0f, 1.0f, 0.0f) }, // 8
		Vertex { .position = glm::vec3(-0.5f, 0.5f,  0.5f), .texuv = glm::vec3(0.0f, 0.0f, 0.0f) }, // 9
		Vertex { .position = glm::vec3( 0.5f, 0.5f,  0.5f), .texuv = glm::vec3(1.0f, 0.0f, 0.0f) }, // 10
		Vertex { .position = glm::vec3( 0.5f, 0.5f, -0.5f), .texuv = glm::vec3(1.0f, 1.0f, 0.0f) }, // 11

		// Bottom face
		Vertex { .position = glm::vec3(-0.5f, -0.5f, -0.5f), .texuv = glm::vec3(0.0f, 0.0f, 0.0f) }, // 12
		Vertex { .position = glm::vec3( 0.5f, -0.5f, -0.5f), .texuv = glm::vec3(1.0f, 0.0f, 0.0f) }, // 13
		Vertex { .position = glm::vec3( 0.5f, -0.5f,  0.5f), .texuv = glm::vec3(1.0f, 1.0f, 0.0f) }, // 14
		Vertex { .position = glm::vec3(-0.5f, -0.5f,  0.5f), .texuv = glm::vec3(0.0f, 1.0f, 0.0f) }, // 15

		// Right face
		Vertex { .position = glm::vec3(0.5f, -0.5f, -0.5f), .texuv = glm::vec3(1.0f, 0.0f, 0.0f) }, // 16
		Vertex { .position = glm::vec3(0.5f,  0.5f, -0.5f), .texuv = glm::vec3(1.0f, 1.0f, 0.0f) }, // 17
		Vertex { .position = glm::vec3(0.5f,  0.5f,  0.5f), .texuv = glm::vec3(0.0f, 1.0f, 0.0f) }, // 18
		Vertex { .position = glm::vec3(0.5f, -0.5f,  0.5f), .texuv = glm::vec3(0.0f, 0.0f, 0.0f) }, // 19

		// Left face
		Vertex { .position = glm::vec3(-0.5f, -0.5f, -0.5f), .texuv = glm::vec3(0.0f, 0.0f, 0.0f) }, // 20
		Vertex { .position = glm::vec3(-0.5f, -0.5f,  0.5f), .texuv = glm::vec3(1.0f, 0.0f, 0.0f) }, // 21
		Vertex { .position = glm::vec3(-0.5f,  0.5f,  0.5f), .texuv = glm::vec3(1.0f, 1.0f, 0.0f) }, // 22
		Vertex { .position = glm::vec3(-0.5f,  0.5f, -0.5f), .texuv = glm::vec3(0.0f, 1.0f, 0.0f) }  // 23
	};

	std::vector<uint32> indices = {
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

	// Returns created cube
	return new Cube(vertices, indices); // The vao is made by mesh
}

Plane* ModelFactory::create_plane() noexcept {
	std::vector<Vertex> vertices = {
		// Front face
		Vertex { .position = glm::vec3(-0.5f, -0.5f, 0.0f), .texuv = glm::vec3(0.0f, 0.0f, 0.0f) }, // Bottom left
		Vertex { .position = glm::vec3( 0.5f, -0.5f, 0.0f), .texuv = glm::vec3(1.0f, 0.0f, 0.0f) }, // Bottom right
		Vertex { .position = glm::vec3( 0.5f,  0.5f, 0.0f), .texuv = glm::vec3(1.0f, 1.0f, 0.0f) }, // Top right
		Vertex { .position = glm::vec3(-0.5f,  0.5f, 0.0f), .texuv = glm::vec3(0.0f, 1.0f, 0.0f) }, // Top left
	};

	std::vector<uint32> indices = {
		// Front face
		0, 1, 2,
		0, 2, 3,
	};

	// Returns created Plane
	return new Plane(vertices, indices); // The vao is made by mesh
}


Billboard* ModelFactory::create_billboard() noexcept {
	std::vector<Vertex> vertices = {
		// Front face
		Vertex { .position = glm::vec3(-0.5f, -0.5f, 0.0f), .texuv = glm::vec3(0.0f, 0.0f, 0.0f) }, // Bottom left
		Vertex { .position = glm::vec3( 0.5f, -0.5f, 0.0f), .texuv = glm::vec3(1.0f, 0.0f, 0.0f) }, // Bottom right
		Vertex { .position = glm::vec3( 0.5f,  0.5f, 0.0f), .texuv = glm::vec3(1.0f, 1.0f, 0.0f) }, // Top right
		Vertex { .position = glm::vec3(-0.5f,  0.5f, 0.0f), .texuv = glm::vec3(0.0f, 1.0f, 0.0f) }, // Top left
	};

	std::vector<uint32> indices = {
		// Front face
		0, 1, 2,
		0, 2, 3,
	};

	// Return created billboard
	return new Billboard(vertices, indices); // The vao is made by mesh
}
