#include "scarablib/scenes/scene2d.hpp"
#include "scarablib/window/window.hpp"
#include <cstdio>

Scene2D::Scene2D(const Window& window) : Scene(window) {
	// Make shaders
	this->shader = new Shader(this->vertex_shader, this->fragment_shader);
	this->shader_texture = new Shader(this->vertex_shader_texture, this->fragment_shader_texture);

	// Unecessary shader strings
	delete []this->vertex_shader;
	delete []this->fragment_shader;
	delete []this->vertex_shader_texture;
	delete []this->fragment_shader_texture;

	// Make viewport
	this->update_viewport(window.get_width(), window.get_height());
}

void Scene2D::update_viewport(const uint32 width, const uint32 height) {
	// Update width and height
	this->width = width;
	this->height = height;

	// Make camera
	glm::mat4 projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f);

	// Update shader projection value
	// Set in order to not bind for accident
	this->shader->use();
	this->shader->set_matrix4f("projection", projection);

	this->shader_texture->use();
	this->shader_texture->set_matrix4f("projection", projection);

	// Update viewport in opengl too
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
}

