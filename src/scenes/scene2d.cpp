#include "scarablib/scenes/scene2d.hpp"
#include "scarablib/opengl/vao.hpp"
#include "scarablib/opengl/vbo.hpp"
#include "scarablib/types/vertex.hpp"
#include "scarablib/window/window.hpp"
#include <cstdio>

Scene2D::Scene2D(const Window& window) : Scene<Shape2D>(window) {
	// Set projection
	this->update_viewport(window.get_width(), window.get_height());

	const std::vector<Vertex> vertices = {
		// Rectangle
		{ .position = glm::vec3(0.0f, 0.0f, 0.0f), .texuv = glm::vec2(1.0f, 1.0f) }, // Bottom Left
		{ .position = glm::vec3(1.0f, 0.0f, 0.0f), .texuv = glm::vec2(0.0f, 1.0f) }, // Bottom Right
		{ .position = glm::vec3(1.0f, 1.0f, 0.0f), .texuv = glm::vec2(0.0f, 0.0f) }, // Top Right
		{ .position = glm::vec3(0.0f, 1.0f, 0.0f), .texuv = glm::vec2(1.0f, 0.0f) },  // Top Left

		// Triangle
		{ .position = glm::vec3(1.0f, 1.0f, 0.0f), .texuv = glm::vec2(0.0f, 0.0f) }, // Bottom Right
		{ .position = glm::vec3(0.0f, 1.0f, 0.0f), .texuv = glm::vec2(1.0f, 0.0f) }, // Bottom Left
		{ .position = glm::vec3(0.5f, 0.0f, 0.0f), .texuv = glm::vec2(0.5f, 1.0f) }  // Middle
	};

	// Indices are not necessary for simple shapes
	VBO vbo = VBO();

	// Make VAO
	this->vao->bind();

	// Build VBO
	vbo.make_from_vertex(vertices, 2);

	// Unbind all
	this->vao->unbind();
	vbo.unbind();
}

Scene2D::~Scene2D() {
	delete this->vao;
	delete this->shader;
}

void Scene2D::update_viewport(const uint32 width, const uint32 height) {
	// Update width and : height
	this->width = width;
	this->height = height;

	// Make Projection
	glm::mat4 projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);

	// Update shaders projection uniform
	shader->use();
	shader->set_matrix4f("projection", projection);

	// Update circle shader
	Shader& circle_shader = Circle::get_circle_shader();
	circle_shader.use();
	circle_shader.set_matrix4f("projection", projection);

	// Update viewport in opengl too
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
}

void Scene2D::draw_shape(Shape2D& shape) {
	// glDisable(GL_DEPTH_TEST);

	this->vao->bind();
	this->shader->use();
	shape.draw(*this->shader); // If circle it will use the shader in the circle struct
	this->vao->unbind();
}

void Scene2D::draw_all() {
	// glDisable(GL_DEPTH_TEST); // I couldn't find out why Font gets a background when this is enabled and why 2D shapes draws below 3D shapes when this is disabled (i know how DEPTH TEST works, but i dont know why this is happening here and not on the code pre-revamp)

	// this->vao->bind();
	// this->shader->use();
	//
	// for(auto& [_, shape] : this->scene) {
	// 	shape->draw(*this->shader); // If circle it will use the shader in the circle struct
	// }
	//
	// this->vao->unbind();
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
