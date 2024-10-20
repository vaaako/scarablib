#include "scarablib/scenes/scene2d.hpp"
#include "scarablib/opengl/vao.hpp"
#include "scarablib/opengl/vbo.hpp"
#include "scarablib/proper/log.hpp"
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
	delete this->shader;
	delete this->shader_circle;
}

void Scene2D::update_viewport(const uint32 width, const uint32 height) {
	// Update width and : height
	this->width = width;
	this->height = height;

	// Make Projection
	glm::mat4 projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f);

	// Update shaders projection uniform
	shader->use();
	shader->set_matrix4f("projection", projection);
	shader_circle->use();
	shader_circle->set_matrix4f("projection", projection);

	// Update viewport in opengl too
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
}


void Scene2D::draw_font(Font& font) {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	this->begin_draw();

	font.draw(*this->shader);

	this->end_draw();
}

void Scene2D::draw_rectangle(Rectangle& rectangle) {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	this->begin_draw();

	rectangle.draw(*this->shader);

	this->end_draw();
}

void Scene2D::draw_triangle(Triangle& triangle) {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	this->begin_draw();

	triangle.draw(*this->shader);

	this->end_draw();
}

// Min - 0.001
// Default - 0.0
// Max - 1.0
void Scene2D::draw_circle(Circle& circle) {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	this->begin_draw();

	circle.draw(*this->shader);

	this->end_draw();
}

// Not proud of this
void Scene2D::draw_shape(Shape2D& shape) {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	this->vao->bind();
	if(typeid(shape) == typeid(Circle)) {
		this->shader_circle->use();
		shape.draw(*this->shader_circle);
	} else {
		this->shader->use();
		shape.draw(*this->shader);
	}
	this->vao->unbind();
}


void Scene2D::add_to_scene(Shape2D* shape) {
	this->scene.emplace_back(shape);
}

void Scene2D::add_to_scene(const std::vector<Shape2D*> shapes) {
	for(Shape2D* shape : shapes) {
		this->scene.emplace_back(shape);
	}
}

void Scene2D::draw_all() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	this->vao->bind();

	for(auto& shape : this->scene) {
		// If circle use circle shader
		if(dynamic_cast<Circle*>(shape)) {
			this->shader_circle->use();
			shape->draw(*this->shader_circle);

		// If not, use default shader
		} else {
			this->shader->use();
			shape->draw(*this->shader);
		}
	}

	this->vao->unbind();
}

void Scene2D::remove_index(const uint32 index) {
	const uint64 last_index = this->scene.size() -1;

	if(index > last_index) {
		LOG_ERROR("The index you are trying to remove is higher than the size of the objects in scene (%d)", this->scene.size());
		return;
	}

	// Move to last place
	if(index < last_index) {
		std::swap(this->scene[index], this->scene[last_index]);
	}

	// Remove last element
	this->scene.pop_back();
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
