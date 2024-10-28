#pragma once

#include "glm/ext/vector_float3.hpp"
#include "glm/geometric.hpp"
#include "scarablib/input/mouse.hpp"
#include "scarablib/proper/vector/vec3.hpp"
#include "scarablib/proper/vector/vecutil.hpp"
#include "scarablib/window/window.hpp"

enum class Zoom : bool {
	OUT = 0,
	IN = 1
};

class Camera {
	friend class Scene3D;
	friend struct Mesh;

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
			// return this->position;
			return { this->position.x, this->position.y, this->position.z };
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
		void rotate(const Mouse& mouse);

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
		
		glm::vec3 position    = { 0.0f, 0.0f, 2.0f };
		glm::vec3 orientation = { 0.0f, 0.0f, -1.0f };

		glm::vec3 up    = { 0.0f, 1.0f, 0.0f };
		glm::vec3 front = { 0.0f, 0.0f, -1.0f };
};
