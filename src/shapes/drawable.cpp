#include "scarablib/shapes/drawable.hpp"
#include "scarablib/opengl/ebo.hpp"
#include "scarablib/opengl/shader.hpp"
#include "scarablib/opengl/vbo.hpp"
#include "scarablib/types/vertex.hpp"


Drawable::Drawable() {
	glm::mat4 projection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f);

	// Update shader projection value
	// Set in order to not bind for accident
	shader.use();
	shader.set_matrix4f("projection", projection);

	shader_circle.use();
	shader_circle.set_matrix4f("projection", projection);

	std::vector<Vertex> vertices = {
		// Rectangle
		{ .position2d = glm::vec2(0.0f, 0.0f), .texuv = glm::vec2(1.0f, 1.0f) }, // Bottom Left
		{ .position2d = glm::vec2(1.0f, 0.0f), .texuv = glm::vec2(0.0f, 1.0f) }, // Bottom Right
		{ .position2d = glm::vec2(1.0f, 1.0f), .texuv = glm::vec2(0.0f, 0.0f) }, // Top Right
		{ .position2d = glm::vec2(0.0f, 1.0f), .texuv = glm::vec2(1.0f, 0.0f) },  // Top Left

		// Triangle
		{ .position2d = glm::vec2(1.0f, 1.0f), .texuv = glm::vec2(0.0f, 0.0f) }, // Bottom Right
		{ .position2d = glm::vec2(0.0f, 1.0f), .texuv = glm::vec2(1.0f, 0.0f) }, // Bottom Left
		{ .position2d = glm::vec2(0.5f, 0.0f), .texuv = glm::vec2(0.5f, 1.0f) }  // Middle
	};

	// Indices are not necessary for simple shapes

	#define POSITION_DIMENSION 2
	#define TEXTURE_DIMENSION 2

	VBO vbo = VBO();

	// Make VAO
	vao.bind();

	// Build VBO
	vbo.alloc_data(vertices.size() * sizeof(Vertex), vertices.data());
	vbo.link_attrib(0, POSITION_DIMENSION, sizeof(Vertex), offsetof(Vertex, position2d));
	vbo.link_attrib(1, TEXTURE_DIMENSION, sizeof(Vertex), offsetof(Vertex, texuv));

	// Unbind all
	vao.unbind(); // Must unbind first
	vbo.unbind();
}


void Drawable::draw_rectangle() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	shader.use();

	glm::mat4 model = glm::mat4(1.0f);

	// Apply transformations (move, resize, rotate)
	model = glm::translate(model, glm::vec3(100, 100, 0.0f))
		  * glm::scale(model, glm::vec3(100.0f, 100.0f, 1.0f));

	shader.set_matrix4f("model", model);
	shader.set_vector4f("shapeColor", { 255.0f, 0.0f, 255.0f, 255.0f });

	this->vao.bind();
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	this->vao.unbind();
	shader.unbind();
}

void Drawable::draw_triangle() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	shader.use();

	glm::mat4 model = glm::mat4(1.0f);

	// Apply transformations (move, resize, rotate)
	model = glm::translate(model, glm::vec3(position.x, position.y, 0.0f))
		  * glm::scale(model, glm::vec3(100.0f, 100.0f, 1.0f)); // Scale

	shader.set_matrix4f("model", model);

	shader.set_vector4f("shapeColor", { 255.0f, 0.0f, 255.0f, 255.0f });
	
	this->vao.bind();
	glDrawArrays(GL_TRIANGLES, 4, 3);
	this->vao.unbind();
	shader.unbind();
}

// Min - 0.001
// Default - 0.0
// Max - 1.0
void Drawable::draw_circle(const vec2<float>& pos, const float blur) {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	shader_circle.use();

	// Make a new each frame
	glm::mat4 model = glm::mat4(1.0f);

	// Apply transformations (move, resize, rotate)
	model = glm::translate(model, glm::vec3(pos.x, pos.y, 0.0f))
		  * glm::scale(model, glm::vec3(100.0f, 100.0f, 1.0f));
	shader_circle.set_matrix4f("model", model);

	shader_circle.set_vector4f("shapeColor", { 255.0f, 0.0f, 255.0f, 255.0f });
	shader_circle.set_float("blur", blur);

	this->vao.bind();
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	this->vao.unbind();
	shader_circle.unbind();
}


// For specifing each point of a triangle
// Making a new triangle each frame? putting on a cache and checking vertices each frame?
// i dont know how to optimize this
// vertices[2].position2d = glm::vec2(100.0f, 100.f); // Update top vertex position
// vertices[1].position2d = glm::vec2(50.0f, 150.0f); // Update bottom-left vertex position
// vertices[0].position2d = glm::vec2(150.0f, 150.0f); // Update bottom-right vertex position

// this->vbo.bind();
// glBindBuffer(GL_ARRAY_BUFFER, vbo.get_id());
// glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
// this->vbo.unbind();
