#include "scarablib/scenes/camera.hpp"
#include "glm/geometric.hpp"
#include "scarablib/window/window.hpp"

Camera::Camera(const Window& window, const float fov, const float sensitivity) noexcept
	: sensitivity(sensitivity), width(window.get_width()),
	  height(window.get_height()), fov(fov), max_fov(fov) {

	this->update_proj_matrix();
}

void Camera::rotate(const Window& mouse) noexcept {
	const vec2<int> move = mouse.relative_move();

	this->yaw   += static_cast<float>(move.x) * this->sensitivity;
	this->pitch += static_cast<float>(-move.y) * this->sensitivity;
	this->pitch  = glm::clamp(this->pitch, -89.0f, 89.0f); // Prevent gimbal lock

	this->update_camera_vectors();
}

void Camera::update_camera_vectors() noexcept {
	const float radpitch = glm::radians(this->pitch);
	const float cospitch = std::cos(radpitch);
	const float radyaw = glm::radians(this->yaw);

	this->forward = glm::normalize(vec3<float> {
		std::cos(radyaw) * cospitch,
		std::sin(radpitch),
		std::sin(radyaw) * cospitch
	});

	// Use fixed up position
	this->right = glm::normalize(glm::cross(this->forward, { 0.0f, 1.0f, 0.0f }));
	this->up   = glm::normalize(glm::cross(this->right, this->forward));
}


void Camera::move(const vec3<int8>& dir, const float dt) noexcept {
	// Combine movement direction
	vec3<float> move = this->forward * (float)dir.z +
					   this->up * (float)dir.y +
					   this->right * (float)dir.x;

	// Normalize only horizontal (XZ) to prevent faster diagonal movement
	if(dir.x != 0.0f || dir.z != 0.0f) {
		move = glm::normalize(move);
	}

	// Update position
	this->position += move * this->speed * dt;
}
