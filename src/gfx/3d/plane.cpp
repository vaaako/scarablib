#include "scarablib/gfx/3d/plane.hpp"
#include "scarablib/opengl/ebo.hpp"
#include "scarablib/opengl/vbo.hpp"
#include "scarablib/gfx/mesh.hpp"


Plane::Plane(const MeshConf& conf) : Mesh(conf, &this->get_vao()) {
	std::vector<Vertex> vertices = {
		// Front face
		Vertex { .position = glm::vec3(-0.5f, -0.5f, 0.0f), .texuv = glm::vec2(0.0f, 0.0f) }, // Bottom left vertex
		Vertex { .position = glm::vec3( 0.5f, -0.5f, 0.0f), .texuv = glm::vec2(1.0f, 0.0f) }, // Bottom right vertex
		Vertex { .position = glm::vec3( 0.5f,  0.5f, 0.0f), .texuv = glm::vec2(1.0f, 1.0f) }, // Top right vertex
		Vertex { .position = glm::vec3(-0.5f,  0.5f, 0.0f), .texuv = glm::vec2(0.0f, 1.0f) }, // Top left vertex

		// Back face
		// Vertex { .position = glm::vec3(-0.5f, -0.5f, 0.0f), .texuv = glm::vec2(1.0f, 0.0f) }, // 4
		// Vertex { .position = glm::vec3(-0.5f,  0.5f, 0.0f), .texuv = glm::vec2(1.0f, 1.0f) }, // 5
		// Vertex { .position = glm::vec3( 0.5f,  0.5f, 0.0f), .texuv = glm::vec2(0.0f, 1.0f) }, // 6
		// Vertex { .position = glm::vec3( 0.5f, -0.5f, 0.0f), .texuv = glm::vec2(0.0f, 0.0f) }, // 7
	};

	std::vector<uint32> indices = {
		// Front face
		0, 1, 2,
		0, 2, 3,

		// Back face
		// 4, 5, 6,
		// 4, 6, 7,
	};

	this->conf.size = calc_size(vertices);

	const VAO& vao = this->get_vao();
	vao.bind();

	// Gen VBO and EBO
	VBO vbo = VBO();
	EBO ebo = EBO(indices);

	vbo.make_from_vertex(vertices, 3);

	// Unbind vao
	vao.unbind();
	vbo.unbind();
	ebo.unbind();

	this->indices_length = static_cast<uint32>(indices.size());
}


void Plane::face_position(const vec3<float>& position, const vec3<float> axis) {
	// Plane distance from camera
	glm::vec3 direction = position - this->get_position();
	direction = glm::normalize(direction);

	// Ignore Y
	float angle = glm::degrees(atan2(direction.x, direction.z));

	// plane.set_rotation(angle, { 0.0f, 1.0f, 0.0f });
	this->set_orientation(angle, axis);
}
