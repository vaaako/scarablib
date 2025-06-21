#include "scarablib/scenes/camera.hpp"

Camera::Camera(const Window& window) noexcept : width(window.get_width()), height(window.get_height()) {};

void Camera::update_viewport(const uint32 width, const uint32 height) noexcept {
	this->width = width;
	this->height = height;
	this->update_proj_matrix(); // OpenGL viewport update
}

