#include "scarablib/scenes/scene2d.hpp"
#include "scarablib/opengl/vao.hpp"
#include "scarablib/opengl/vbo.hpp"
#include "scarablib/types/font.hpp"
#include "scarablib/types/vertex.hpp"
#include "scarablib/window/window.hpp"
#include <cstdio>

Scene2D::Scene2D(const Window& window) : IScene<Shape2D>(window) {
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
	this->vao->bind();

	// Build VBO
	this->vbo->bind();
	this->vbo->make_from_vertex(vertices, 2, GL_STATIC_DRAW);

	// Unbind all
	this->vao->unbind();
	this->vbo->unbind();
}

Scene2D::~Scene2D() {
	delete this->vao;
	delete this->vbo;
	delete this->shader;
}

void Scene2D::add_to_scene(const std::string& key, Shape2D* shape) {
	if(!shape) {
		throw ScarabError("Try to add null '%s'", shape);
	}

	this->scene.emplace(key, shape);
}

void Scene2D::draw_all() const {
	// This is just necessary if doing 2D and 3D
	#ifdef SCARAB_2D_AND_3D
		glDepthFunc(GL_ALWAYS);
	#endif

	this->vao->bind();
	this->shader->use();

	for(const auto& [_, shape] : scene) {
		shape->draw(*this->shader);
	}

	this->vao->unbind();
	this->shader->unbind();

	#ifdef SCARAB_USE_2D_AND_3D
		glDepthFunc(GL_LESS);
	#endif
}


void Scene2D::update_viewport(const uint32 width, const uint32 height) {
	// Update width and : height
	this->width = width;
	this->height = height;

	// Make Projection
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, -1.0f, 1.0f);

	// Update shaders projection uniform
	shader->use();
	shader->set_matrix4f("projection", projection);
	shader->unbind();

	// Update circle shader
	Shader& circle_shader = Circle::get_shader();
	circle_shader.use();
	circle_shader.set_matrix4f("projection", projection);
	circle_shader.unbind();

	// Update Font shader
	Shader& font_shader = Font::get_shader();
	font_shader.use();
	font_shader.set_matrix4f("projection", projection);
	font_shader.unbind();


	// Update viewport in opengl too
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
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
