#pragma once

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"
#include "scarablib/input/mouse.hpp"
#include "scarablib/window/window.hpp"

enum class Zoom : bool {
	OUT = 0,
	IN = 1
};

class Camera {
	public:
		Camera(const Window& window, const float fov = 45.0f, const float sensitivity = 100.0f);

		// GETTERS

		// Get camera's viewport width
		inline uint32 get_width() const {
			return this->width;
		}

		// Get camera's viewport height
		inline uint32 get_height() const {
			return this->height;
		}

		// Get camera position
		inline vec3<float> get_position() const {
			return this->position;
		}

		// Get current X position
		inline float get_x() const {
			return this->position.x;
		}

		// Get current Y position
		inline float get_y() const {
			return this->position.y;
		}

		// Get current Z position
		inline float get_z() const {
			return this->position.z;
		}

		// Get camera view matrix
		inline glm::mat4 get_view_matrix() const {
			return glm::lookAt(this->position, this->position + this->orientation, this->up);
		}

		// Get camera view matrix
		inline glm::mat4 get_proj_matrix() const {
			return glm::perspective(glm::radians(this->fov),
				(static_cast<float>(this->get_width()) / static_cast<float>(this->get_height())),
				this->near_plane, this->far_plane);
		}


		// SETTERS
		inline void update_viewport(const Window& window) {
			this->update_viewport(window.get_width(), window.get_height());
		}

		inline void update_viewport(const uint32 width, const uint32 height) {
			this->width = width;
			this->height = height;
		}

		// Set camera's near plane. How near to render
		void set_near_plane(const float near_plane);

		// Set camera's far plane. How far to render
		void set_far_plane(const float far_plane);

		// Set camera's fov
		void set_fov(const float fov);

		// Set camera's minimum amount of fov (used for zoom out)
		void set_min_fov(const float min_fov);

		// Set camera's maximum amount of fov (used for zoom in)
		void set_max_fov(const float max_fov);

		// Set camera's movement speed
		void set_speed(const float speed) {
			this->speed = speed / 10;
		}

		void set_posiion(const vec3<float>& position) {
			this->position = position;
		}

		// MOVEMENT

		// Moves foward to the axis the camera is looking
		inline void move_foward() {
			this->position += this->orientation * this->speed;
		}

		// Moves backward to the axis the camera is looking
		inline void move_backward() {
			this->position -= this->orientation * this->speed;
		}

		// Moves foward to all axis except Y axis
		inline void move_front() {
			this->position += glm::normalize(glm::vec3(this->orientation.x, 0.0f, this->orientation.z)) * this->speed;
		}

		// Moves backward to all axis except Y axis
		inline void move_back() {
			this->position -= glm::normalize(glm::vec3(this->orientation.x, 0.0f, this->orientation.z)) * this->speed;
		}

		// Moves right on X and Z axis
		inline void move_right() {
			this->position += glm::normalize(glm::cross(this->orientation, this->up)) * this->speed ;
		}

		// Moves left on X and Z axis
		inline void move_left() {
			this->position -= glm::normalize(glm::cross(this->orientation, this->up)) * this->speed;
		}

		// Fly
		inline void fly_up() {
			this->position += this->up * this->speed;
		}

		inline void fly_down() {
			this->position -= this->up * this->speed;
		}

		// Zoom in or out the camera using an speed value
		void zoom(const float speed, const Zoom zoom_dir);

		// Use the mouse to rotate the camera view
		void rotate(const MouseHandler& mouse);

	private:
		uint32 width;
		uint32 height;

		float sensitivity;
		float speed = 0.1f;

		float fov;
		float max_fov = 100.0f;
		float min_fov = fov;

		float near_plane = 0.1f;
		float far_plane  = 100.0f;

		// Movement
		bool firstclick = true;
		// -90 in yaw prevents camera from jumping on the first click
		float yaw = -90.0f; // Horizontal rotation
		float pitch = 0.0f; // Vertical rotation
		
		vec3<float> position    = { 0.0f, 0.0f, 2.0f };
		vec3<float> orientation = { 0.0f, 0.0f, -1.0f };
		vec3<float> up          = { 0.0f, 1.0f, 0.0f };
		// vec3<float> front = { 0.0f, 0.0f, -1.0f };
};
