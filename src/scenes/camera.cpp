#include "scarablib/scenes/camera.hpp"
#include "scarablib/window/window.hpp"
#include <algorithm>

Camera::Camera(const Window& window, const float fov, const float sensitivity) noexcept
	: width(window.get_width()), height(window.get_height()), half_width(window.get_half_width()), half_height(window.get_half_height()),
	  sensitivity(sensitivity), fov(fov), max_fov(fov) {}

void Camera::rotate(const MouseHandler& mouse) noexcept {
	// dt is not necessary here

	const vec2<uint32> mousemotion = mouse.get_motion();

	// Calculate move relative to the screen middle
	float xoffset = this->sensitivity * (static_cast<float>(mousemotion.x) - static_cast<float>(this->half_width)) / static_cast<float>(this->width);
	float yoffset = this->sensitivity * (static_cast<float>(mousemotion.y) - static_cast<float>(this->half_height)) / static_cast<float>(this->height);

	this->yaw += xoffset;
	this->pitch -= yoffset;
	// Clamp the pitch to avoid flipping
	// If >89.0f = 89.0f else <-89.0f = -89.0f
	this->pitch = std::clamp(this->pitch, -89.0f, 89.0f);

	const float cospitch = std::cos(glm::radians(this->pitch));
	const float radyaw = glm::radians(this->yaw);

	// Update orientation
	// orientation = front
	this->orientation = glm::normalize(vec3<float> {
		std::cos(radyaw) * cospitch,
		std::sin(glm::radians(this->pitch)),
		std::sin(radyaw) * cospitch
	});
}

