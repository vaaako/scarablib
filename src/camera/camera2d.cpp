#include "scarablib/camera/camera2d.hpp"

Camera2D::Camera2D(const Window& window) noexcept
	: Camera(window) {
	this->update_proj_matrix();
	this->update_view_matrix();
}

void Camera2D::set_position(const vec2<float>& position) noexcept {
	this->position = position;
	this->update_view_matrix();
}

// Moves camera around
void Camera2D::move(const vec2<float>& delta, const float dt) noexcept {
	this->position += delta * dt;
	this->update_view_matrix();
}

void Camera2D::set_zoom(const float zoom) noexcept {
	this->zoom = zoom;
	this->update_proj_matrix();
}

void Camera2D::update_proj_matrix() noexcept {
	// Orthogonal projection
	this->proj = glm::mat4(1.0f);
	this->proj = glm::ortho(0.0f, (float)this->width, (float)this->height, 0.0f, -1.0f, 1.0f);
	// Apply zoom
	this->proj = glm::scale(this->proj, vec3<float>(zoom, zoom, 1.0f));
}

void Camera2D::update_view_matrix() noexcept {
	this->view = glm::mat4(1.0f);
	// Move camera in opposite direction
	this->view = glm::translate(this->view, glm::vec3(-this->position.x, -this->position.y, 0.0f));
}

