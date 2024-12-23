#include "scarablib/scenes/camera.hpp"
#include "scarablib/window/window.hpp"

Camera::Camera(const Window& window, const float fov, const float sensitivity)
	: width(window.get_width()), height(window.get_height()), sensitivity(sensitivity), fov(fov), max_fov(fov) {}



void Camera::rotate(const MouseHandler& mouse) {
	// Calculate move relative to the screen middle
	float xoffset = this->sensitivity * (static_cast<float>(mouse.motion.x) - static_cast<float>(this->width) / 2) / static_cast<float>(this->width);
	float yoffset = this->sensitivity * (static_cast<float>(mouse.motion.y) - static_cast<float>(this->height) / 2) / static_cast<float>(this->height);

	this->yaw += xoffset;
	this->pitch -= yoffset;

	// Constrain the pitch
	if(this->pitch > 89.0f) {
		this->pitch = 89.0f;
	} else if(this->pitch < -89.0f) {
		this->pitch = -89.0f;
	}


	// Update orientation
	// orientation = front
	this->orientation = glm::normalize(vec3<float> {
		std::cos(glm::radians(this->yaw)) * std::cos(glm::radians(this->pitch)),
		std::sin(glm::radians(this->pitch)),
		std::sin(glm::radians(this->yaw)) * std::cos(glm::radians(this->pitch))
	});
}




void Camera::set_near_plane(const float near_plane) {
	if(near_plane > this->far_plane) {
		LOG_ERROR("new \"near plane\" value can't be higher than \"far plane\" current value");
		return;
	}

	this->near_plane = near_plane;
}

void Camera::set_far_plane(const float far_plane) {
	if(far_plane < this->near_plane) {
		LOG_ERROR("new \"far plane\" value can't be lower than \"near plane\" current value");
		return;
	}

	this->far_plane = far_plane;
}

void Camera::set_fov(const float fov) {
	if(fov < this->near_plane || fov > this->far_plane) {
		LOG_ERROR("new \"fov\" value must be between \"near plane\" and \"far plane\" current values");
		return;
	}

	this->fov = fov;
}

void Camera::set_min_fov(const float min_fov) {
	if(min_fov < this->max_fov) {
		LOG_ERROR("new \"min fov\" value can't be higher than \"max fov\" current value");
		return;
	}

	this->min_fov = min_fov;
}

void Camera::set_max_fov(const float max_fov) {
	if(max_fov < this->min_fov) {
		LOG_ERROR("new \"max fov\" value can't be lower than \"min fov\" current value");
		return;
	}

	this->max_fov = max_fov;
}

void Camera::zoom(const float speed, const Zoom zoom_dir) {
	if(zoom_dir == Zoom::IN) {
		this->fov = std::max(this->fov - speed, this->min_fov);
	} else {
		this->fov = std::min(this->fov + speed, this->max_fov);
	}
}
