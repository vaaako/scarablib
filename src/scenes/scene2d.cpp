#include "scarablib/scenes/scene2d.hpp"
#include "scarablib/window/window.hpp"
#include <cstdio>

Scene2D::Scene2D(const Window& window) : Scene(window) {
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
	this->shader.use();
	this->shader.set_matrix4f("projection", projection);

	// Update viewport in opengl too
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
}

