#pragma once

#include <glm/ext/matrix_transform.hpp>
#include "scarablib/camera/camera.hpp"
#include "scarablib/window/window.hpp"

class PerspectiveCamera : public Camera {
	public:
		vec3<float> position = vec3<float>(0.0f);
		// Camera's sensitivity
		float sensitivity;
		// Horizontal rotation.
		// To rotate camera using the mouse, use `Camera::rotate(const Window& window)`
		float yaw = -90.0f; // Looking to where is -Z (WEST)
		// Vertical rotation
		// To rotate camera using the mouse, use `Camera::rotate(const Window& window)`
		float pitch = 0.0f;

		PerspectiveCamera(const Window& window, const float fov = 45.0f, const float sensitivity = 0.1f) noexcept;

		// MOVEMENT


		// Convert local direction to world-space direction
		vec3<float> to_wordspace(const vec3<float>& dir, const float speed) const noexcept;

		// Moves the camera using a direction vector.
		// This will convert the direction vector to world-space before moving.
		// Where X is front/back, Y is up/down and Z is left/right.
		// Example: [1, 0, -1] means the camera moved to Front-Left
		void move(const vec3<float>& dir, const float speed) noexcept;

		// Use mouse moved direction to rotate the camera
		// void rotate(const MouseHandler& mouse) noexcept;
		void rotate(const Window& mouse) noexcept;

		// GETTERS

		// Returns camera view matrix
		inline glm::mat4 get_view_matrix() const noexcept override {
			return glm::lookAt(this->position, this->position + this->forward, this->up);
		}

		// Retuns camera unit direction vector
		inline vec3<float> get_forward() const noexcept {
			return this->forward;
		}

		// Returns camera unit right vector
		inline vec3<float> get_right() const noexcept {
			return this->right;
		}

		// Returns camera unit up vector
		inline vec3<float> get_up() const noexcept {
			return this->up;
		}

		// Returns camera current fov
		inline float get_fov() const noexcept {
			return this->fov;
		}

		// Returns near plane
		inline float get_near_plane() const noexcept {
			return this->near_plane;
		}
		// Returns far plane
		inline float get_far_plane() const noexcept {
			return this->far_plane;
		}

		// SETTERS //

		// Zoom in (positive) or Zoom out (negative) the camera using an speed value
		void set_zoom(const float speed) noexcept override;

		// Sets camera near plane. How near to render
		void set_near_plane(const float near_plane);

		// Sets camera far plane. How far to render
		void set_far_plane(const float far_plane);

		// Sets camera fov
		void set_fov(const float fov);

		// Sets minimum amount of fov. Used for zoom out
		void set_min_fov(const float min_fov);

		// Sets maximum amount of fov. Used for zoom in
		void set_max_fov(const float max_fov);

		// Changes the camera rotation using yaw and pitch angles.
		// Yaw is horizontal rotation, Pitch is vertical
		void set_rotation(const float yaw, const float pitch) noexcept;

		// Sets camera height in cm
		inline void set_height(float height_cm) noexcept {
			this->position.y = std::max(height_cm * 0.01f, 0.0f); // Convert to meters 
		}

		// Returns a string with the cardinal direction the camera is facing based on its yaw
		static std::string_view get_cardinal_direction(const float yaw);

	private:
		float fov;
		float max_fov = 100.0f;
		float min_fov = 1.0f;

		float near_plane = 0.1f;
		float far_plane  = 100.0f;

		// Movement
		vec3<float> up          = vec3<float>(0.0f, 1.0f, 0.0f);
		vec3<float> forward     = vec3<float>(0.0f, 0.0f, -1.0f);
		vec3<float> right       = vec3<float>(1.0f, 0.0f, 0.0f);

		void update_proj_matrix() noexcept override;
		// Update position, up, foward and left
		void update_camera_vectors() noexcept;
};
