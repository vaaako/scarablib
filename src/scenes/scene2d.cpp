#include "scarablib/scenes/scene2d.hpp"
#include "scarablib/opengl/vbo.hpp"
#include "scarablib/types/vertex.hpp"
#include "scarablib/window/window.hpp"
#include <cstdio>

Scene2D::Scene2D(const Window& window) : Scene(window) {
	// Set projection
	this->update_viewport(window.get_width(), window.get_height());

	const std::vector<Vertex> vertices = {
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
	this->vao->bind();

	// Build VBO
	vbo.alloc_data(vertices.size() * sizeof(Vertex), vertices.data());
	vbo.link_attrib(0, POSITION_DIMENSION, sizeof(Vertex), offsetof(Vertex, position2d));
	vbo.link_attrib(1, TEXTURE_DIMENSION, sizeof(Vertex), offsetof(Vertex, texuv));

	// Unbind all
	this->vao->unbind();
	vbo.unbind();
}


Scene2D::~Scene2D() {
	delete this->vao;

	for(Shader* shader : this->shaders) {
		delete shader;
	}

	this->shaders.clear();
}

void Scene2D::update_viewport(const uint32 width, const uint32 height) {
	// Update width and : height
	this->width = width;
	this->height = height;

	// Make Projection
	glm::mat4 projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f);

	// Update shader projection uniform
	for (Shader* shader : shaders) {
		shader->use();
		shader->set_matrix4f("projection", projection);
	}

	// Update viewport in opengl too
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
}


void Scene2D::draw_rectangle(const Rectangle& rectangle) const {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	this->shader.use();

	this->vao->bind();
	rectangle.draw(shader);
	this->vao->unbind();

	this->shader.unbind();
}

void Scene2D::draw_triangle(const Triangle& triangle) const {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	this->shader.use();

	this->vao->bind();
	triangle.draw(shader);
	this->vao->unbind();

	this->shader.unbind();
}

// Min - 0.001
// Default - 0.0
// Max - 1.0
void Scene2D::draw_circle(const Circle& circle) const {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	this->shader_circle.use();

	this->vao->bind();
	circle.draw(this->shader_circle);
	this->vao->unbind();

	this->shader_circle.unbind();
}

// Not proud of this
void Scene2D::draw_shape(const Shape2D& shape) {
	Shader* shader = &this->shader;
	if(typeid(shape) == typeid(Circle)) {
		shader = &this->shader_circle;
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	shader->use();

	this->vao->bind();
	shape.draw(*shader);
	this->vao->unbind();

	shader->unbind();
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
