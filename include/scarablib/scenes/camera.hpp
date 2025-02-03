#pragma once

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"
#include "scarablib/input/mouse.hpp"
#include "scarablib/window/window.hpp"

class Camera {
	public:
		enum class Zoom : bool {
			OUT = 0,
			IN = 1
		};

		Camera(const Window& window, const float fov = 45.0f, const float sensitivity = 100.0f) noexcept;

		// GETTERS

		// Get camera position
		inline vec3<float> get_position() const noexcept {
			return this->position;
		}

		// Get current X position
		inline float get_x() const noexcept {
			return this->position.x;
		}

		// Get current Y position
		inline float get_y() const noexcept {
			return this->position.y;
		}

		// Get current Z position
		inline float get_z() const noexcept {
			return this->position.z;
		}

		// Get camera view matrix
		inline glm::mat4 get_view_matrix() const noexcept {
			return glm::lookAt(this->position, this->position + this->orientation, this->up);
		}

		// Get camera view matrix
		inline glm::mat4 get_proj_matrix() const noexcept {
			return glm::perspective(glm::radians(this->fov),
				(static_cast<float>(this->width) / static_cast<float>(this->height)),
				this->near_plane, this->far_plane);
		}


		// SETTERS
		inline void update_viewport(const Window& window) noexcept {
			this->update_viewport(window.get_width(), window.get_height());
		}

		inline void update_viewport(const uint32 width, const uint32 height) noexcept {
			this->width = width;
			this->height = height;
		}

		// Set camera's near plane. How near to render
		inline void set_near_plane(const float near_plane) noexcept {
			if(near_plane > this->far_plane) {
				LOG_ERROR("new \"near plane\" value can't be higher than \"far plane\" current value");
				return;
			}

			this->near_plane = near_plane;
		}

		// Set camera's far plane. How far to render
		inline void set_far_plane(const float far_plane) noexcept {
			if(far_plane < this->near_plane) {
				LOG_ERROR("new \"far plane\" value can't be lower than \"near plane\" current value");
				return;
			}

			this->far_plane = far_plane;
		}

		// Set camera's fov
		inline void set_fov(const float fov) noexcept {
			if(fov < this->near_plane || fov > this->far_plane) {
				LOG_ERROR("new \"fov\" value must be between \"near plane\" and \"far plane\" current values");
				return;
			}

			this->fov = fov;
		}

		// Set camera's minimum amount of fov (used for zoom out)
		inline void set_min_fov(const float min_fov) noexcept {
			if(min_fov < this->max_fov) {
				LOG_ERROR("new \"min fov\" value can't be higher than \"max fov\" current value");
				return;
			}

			this->min_fov = min_fov;
		}

		// Set camera's maximum amount of fov (used for zoom in)
		inline void set_max_fov(const float max_fov) noexcept {
			if(max_fov < this->min_fov) {
				LOG_ERROR("new \"max fov\" value can't be lower than \"min fov\" current value");
				return;
			}

			this->max_fov = max_fov;
		}

		// Zoom in or out the camera using an speed value
		// Use Camera::Zoom::IN or Camera::Zoom::OUT
		inline void zoom(const float speed, const Camera::Zoom zoom_dir) noexcept {
			this->fov = (zoom_dir == Zoom::IN)
				? std::max(fov - speed, min_fov)
				: std::min(fov + speed, max_fov);
		}

		// Set camera's movement speed
		inline void set_speed(const float speed) noexcept {
			this->speed = speed / 10.0f;
		}

		// Set camera's position
		inline void set_posiion(const vec3<float>& position) noexcept {
			this->position = position;
		}

		inline void set_sensitivity(const float sensitivity) noexcept {
			this->sensitivity = sensitivity;
		}


		// MOVEMENT

		// Moves foward to the axis the camera is looking
		inline void move_foward(const float dt) noexcept {
			this->position += (this->orientation * this->speed) * dt;
		}

		// Moves backward to the axis the camera is looking
		inline void move_backward(const float dt) noexcept {
			this->position -= (this->orientation * this->speed) * dt;
		}

		// Moves foward relative to the ground, ignoring Y axis
		inline void move_front(const float dt) noexcept {
			this->position +=
				(glm::normalize(glm::vec3(this->orientation.x, 0.0f, this->orientation.z)) * this->speed) * dt;
		}

		// Moves backward  relative to the ground, ignoring Y axis
		inline void move_back(const float dt) noexcept {
			this->position -=
				(glm::normalize(glm::vec3(this->orientation.x, 0.0f, this->orientation.z)) * this->speed) * dt;
		}

		// Moves right on X and Z axis
		inline void move_right(const float dt) noexcept {
			this->position +=
				(glm::normalize(glm::cross(this->orientation, this->up)) * this->speed) * dt;
		}

		// Moves left on X and Z axis
		inline void move_left(const float dt) noexcept {
			this->position -=
				(glm::normalize(glm::cross(this->orientation, this->up)) * this->speed) * dt;
		}

		// Fly up
		inline void fly_up(const float dt) noexcept {
			this->position += (this->up * this->speed) * dt;
		}

		// Fly down
		inline void fly_down(const float dt) noexcept {
			this->position -= (this->up * this->speed) * dt;
		}

		// Use the mouse to rotate the camera view
		void rotate(const MouseHandler& mouse) noexcept;

	private:
		uint32 width;
		uint32 height;
		// Pre-calculated used in rotate
		float half_width;
		float half_height;

		float sensitivity;
		float speed = 0.1f;

		float fov;
		float max_fov = 100.0f;
		float min_fov = fov;

		float near_plane = 0.1f;
		float far_plane  = 100.0f;

		// Movement
		// bool firstclick = true;
		// -90 in yaw prevents camera from jumping on the first click
		float yaw = -90.0f; // Horizontal rotation
		float pitch = 0.0f; // Vertical rotation
		
		vec3<float> position    = { 0.0f, 0.0f, 2.0f }; // Real camera's position
		vec3<float> orientation = { 0.0f, 0.0f, -1.0f };
		vec3<float> up          = { 0.0f, 1.0f, 0.0f };
};
