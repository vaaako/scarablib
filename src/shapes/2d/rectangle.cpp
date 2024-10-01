#include "scarablib/shapes/2d/rectangle.hpp"
#include "scarablib/opengl/ebo.hpp"
#include "scarablib/types/vertex.hpp"
#include <glm/ext/vector_float3.hpp>

std::vector<glm::mat4> Rectangle::instance_matrices;
bool Rectangle::buffers_initialized = false; // Initialization flag

Rectangle::Rectangle() {
	if(!buffers_initialized) {
		this->initialize_buffers();
	}
}

void Rectangle::initialize_buffers() {
	// Define vertices and indices once
	std::vector<Vertex> vertices = {
		{ .position2d = glm::vec2(0.0f, 0.0f), .texuv = glm::vec2(1.0f, 1.0f) }, // Bottom Left
		{ .position2d = glm::vec2(1.0f, 0.0f), .texuv = glm::vec2(0.0f, 1.0f) }, // Bottom Right
		{ .position2d = glm::vec2(1.0f, 1.0f), .texuv = glm::vec2(0.0f, 0.0f) }, // Top Right
		{ .position2d = glm::vec2(0.0f, 1.0f), .texuv = glm::vec2(1.0f, 0.0f) }  // Top Left
	};

	std::vector<uint32> indices = {
		0, 1, 2, // First triangle
		2, 3, 0  // Second triangle
	};

#define POSITION_DIMENSION 2 // 2D
#define TEXTURE_DIMENSION 2

	VAO& vao = get_vao();
	VBO& vbo = get_vbo();
	EBO& ebo = get_ebo();

	// Make VAO
	vao.bind();

	// Build VBO
	vbo.alloc_data(vertices.size() * sizeof(Vertex), vertices.data());
	vbo.link_attrib(0, POSITION_DIMENSION, sizeof(Vertex), 0);
	vbo.link_attrib(1, TEXTURE_DIMENSION, sizeof(Vertex), POSITION_DIMENSION);

	// Build EBO
	ebo.build(indices);

	// Unbind all
	vao.unbind(); // Must unbind first
	vbo.unbind();
	ebo.unbind();
}


void Rectangle::draw(const Shader& shader, const vec2<float>& position) const {
	shader.use();

	// Temporary
	glm::mat4 model = glm::mat4(1.0f);

	// Apply transformations (move, resize, rotate)
	model = glm::translate(model, glm::vec3(position.x, position.y, 0.0f))
		  * glm::scale(model, glm::vec3(50.0f, 50.0f, 1.0f));
	shader.set_matrix4f("model", model);

	// Draw
	VAO& vao = get_vao();
	vao.bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
	vao.unbind();
}

// void Rectangle::draw(const Shader& shader) const {
// 	// Bind the static VAO
// 	shader.set_vector4f("shapeColor", { 1.0f, 1.0f, 1.0f, 1.0f });
//
// 	vao.bind();
//
// 	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//
// 	// Unbind VAO
// 	vao.unbind();
// }
