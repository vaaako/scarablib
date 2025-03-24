#pragma once

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/trigonometric.hpp"
#include "scarablib/input/mouse.hpp"
#include "scarablib/proper/log.hpp"
#include "scarablib/window/window.hpp"

class Camera {
	public:
		enum Zoom : bool {
			OUT = 0,
			IN = 1
		};

		Camera(const Window& window, const float fov = 45.0f, const float sensitivity = 0.1f) noexcept;

		// GETTERS

		// Returns camera position
		inline vec3<float> get_position() const noexcept {
			return this->position;
		}

		// Returns current X position
		inline float get_x() const noexcept {
			return this->position.x;
		}

		// Returns current Y position
		inline float get_y() const noexcept {
			return this->position.y;
		}

		// Returns current Z position
		inline float get_z() const noexcept {
			return this->position.z;
		}

		// Returns current yaw. Horizontal rotation
		inline float get_yaw() const noexcept {
			return this->yaw;
		}

		// Returns current pitch. Vertical rotation
		inline float get_pitch() const noexcept {
			return this->pitch;
		}

		// Returns camera view matrix
		inline glm::mat4 get_view_matrix() const noexcept {
			return glm::lookAt(this->position, this->position + this->forward, this->up);
		}

		// Returns camera projection matrix
		inline glm::mat4 get_proj_matrix() const noexcept {
			return this->proj;
		}

		// Retuns camera's unit direction vector
		inline vec3<float> get_forward() const noexcept {
			return this->forward;
		}

		// SETTERS
		inline void update_viewport(const Window& window) noexcept {
			this->update_viewport(window.get_width(), window.get_height());
		}

		inline void update_viewport(const uint32 width, const uint32 height) noexcept {
			this->width = width;
			this->height = height;
			// Opengl update
			this->update_proj_matrix();
		}

		// Set camera's near plane. How near to render
		inline void set_near_plane(const float near_plane) noexcept {
			if(near_plane > this->far_plane) {
				LOG_ERROR("new \"near plane\" value can't be higher than \"far plane\" current value");
				return;
			}
			this->near_plane = near_plane;

			this->update_proj_matrix();
		}

		// Set camera's far plane. How far to render
		inline void set_far_plane(const float far_plane) noexcept {
			if(far_plane < this->near_plane) {
				LOG_ERROR("new \"far plane\" value can't be lower than \"near plane\" current value");
				return;
			}
			this->far_plane = far_plane;

			this->update_proj_matrix();
		}

		// Set camera's fov
		inline void set_fov(const float fov) noexcept {
			if(fov < this->near_plane || fov > this->far_plane) {
				LOG_ERROR("new \"fov\" value must be between \"near plane\" and \"far plane\" current values");
				return;
			}
			this->fov = fov;

			this->update_proj_matrix();
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

			this->update_proj_matrix();
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

		// Changes the camera rotation using yaw and pitch angles.
		// Yaw is horizontal rotation, Pitch is vertical
		inline void set_rotation(const float yaw, const float pitch) noexcept {
			// Wrap yaw to stay within 360 range
			this->yaw = std::fmod(yaw, 360.0f);
			// Prevent gimbal lock
			this->pitch = glm::clamp(pitch, -89.0f, 89.0f);
			this->update_camera_vectors();
		}


		// MOVEMENT

		// Gets a vec3 contanining the direction the camera has moved.
		// Where X is front/back, Y is up/down and Z is left/right.
		// Example: [1, 0, -1] means the camera moved to Front-Left
		inline void move(const vec3<float>& dir, const float dt) noexcept {
			// Normalize for diagonal movement and if any value is >1.0 or <-1.0
			vec3<float> mov = this->forward * dir.x +
							  this->up      * dir.y +
							  this->left    * dir.z;
			// Apply speed and delta time
			this->position += mov * this->speed * dt;
		}

		// Use mouse moved direction to rotate the camera
		void rotate(const MouseHandler& mouse) noexcept;

		static std::string get_cardinal_direction(const float yaw) {
			// Define the cardinal directions
			static std::vector<std::string> directions = {
				"North", "Northeast", "East", "Southeast",
				"South", "Southwest", "West", "Northwest"
			};

			// Determine the sector index
			size_t sector = static_cast<size_t>((yaw + 45.0f / 2.0f) / 45.0f) % 8;

			return directions[sector];
		}

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
		// bool firstclick = true;
		// -90 in yaw prevents camera from jumping on the first click
		float yaw = -90.0f; // Horizontal rotation
		float pitch = 0.0f; // Vertical rotation

		vec3<float> position    = { 0.0f, 0.0f,  2.0f }; // Real camera's position
		vec3<float> up          = { 0.0f, 1.0f,  0.0f };
		vec3<float> forward     = { 0.0f, 0.0f, -1.0f };
		vec3<float> left        = { -1.0f, 0.0f, 0.0f };

		glm::mat4 proj;

		// Update position, up, foward and left
		void update_camera_vectors() noexcept;

		// Pre-calculate proj matrix, since it doesnt change much
		inline void update_proj_matrix() noexcept {
			this->proj = glm::mat4(1.0f);
			this->proj = glm::perspective(glm::radians(this->fov),
				(static_cast<float>(this->width) / static_cast<float>(this->height)),
				this->near_plane, this->far_plane);
		}
};
