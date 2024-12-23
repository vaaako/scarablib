#include "scarablib/gfx/3d/cube.hpp"
#include "scarablib/opengl/ebo.hpp"
#include "scarablib/opengl/vbo.hpp"
#include "scarablib/gfx/mesh.hpp"


Cube::Cube(const MeshConf& conf) : Mesh(conf, &this->get_vao()) {
	std::vector<Vertex> vertices = {
		// Vertices: 24
		// Front face
		Vertex { .position = glm::vec3(-0.5f, -0.5f, 0.5f), .texuv = glm::vec2(0.0f, 0.0f) }, // Bottom left vertex
		Vertex { .position = glm::vec3( 0.5f, -0.5f, 0.5f), .texuv = glm::vec2(1.0f, 0.0f) }, // Bottom right vertex
		Vertex { .position = glm::vec3( 0.5f,  0.5f, 0.5f), .texuv = glm::vec2(1.0f, 1.0f) }, // Top right vertex
		Vertex { .position = glm::vec3(-0.5f,  0.5f, 0.5f), .texuv = glm::vec2(0.0f, 1.0f) }, // Top left vertex

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

	this->conf.size = calc_size(vertices);

	const VAO& vao = this->get_vao();
	vao.bind();

	// Gen VBO and EBO
	VBO vbo = VBO();
	EBO ebo = EBO(indices);

	vbo.bind();
	vbo.make_from_vertex(vertices, 3);

	// Unbind vao
	vao.unbind();
	vbo.unbind();
	ebo.unbind();

	this->indices_length = static_cast<uint32>(indices.size());
}
