#pragma once

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/trigonometric.hpp"
#include "scarablib/proper/log.hpp"
#include "scarablib/window/window.hpp"
#include <array>

class Camera {
	public:
		// Camera's position.
		// To move the camera using a unit vector, use `Camera::move(const vec3<float>& dir, const float dt)`
		vec3<float> position = vec3<float>(0.0f);
		// Camera's sensitivity
		float sensitivity;
		// Horizontal rotation.
		// To rotate camera using the mouse, use `Camera::rotate(const Window& window)`
		float yaw = -90.0f; // Looking to where is -Z (WEST)
		// Vertical rotation
		// To rotate camera using the mouse, use `Camera::rotate(const Window& window)`
		float pitch = 0.0f;

		enum Zoom : bool {
			OUT,
			IN
		};

		Camera(const Window& window, const float fov = 45.0f, const float sensitivity = 0.1f) noexcept;

		// GETTERS

		// Returns camera view matrix
		inline glm::mat4 get_view_matrix() const noexcept {
			return glm::lookAt(this->position, this->position + this->forward, this->up);
		}

		// Returns camera projection matrix
		inline glm::mat4 get_proj_matrix() const noexcept {
			return this->proj;
		}

		// Retuns camera unit direction vector
		inline vec3<float> get_forward() const noexcept {
			return this->forward;
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

		// Update viewport using window object
		inline void update_viewport(const Window& window) noexcept {
			this->update_viewport(window.get_width(), window.get_height());
		}

		// Update viewport using custom width and height
		inline void update_viewport(const uint32 width, const uint32 height) noexcept {
			this->width = width;
			this->height = height;
			this->update_proj_matrix(); // OpenGL viewport update
		}

		// Sets camera near plane. How near to render
		inline void set_near_plane(const float near_plane) noexcept {
			if(near_plane > this->far_plane) {
				LOG_ERROR("new \"near plane\" value can't be higher than \"far plane\" current value");
				return;
			}

			this->near_plane = near_plane;
			this->update_proj_matrix();
		}

		// Sets camera far plane. How far to render
		inline void set_far_plane(const float far_plane) noexcept {
			if(far_plane < this->near_plane) {
				LOG_ERROR("new \"far plane\" value can't be lower than \"near plane\" current value");
				return;
			}

			this->far_plane = far_plane;
			this->update_proj_matrix();
		}

		// Sets camera fov
		inline void set_fov(const float fov) noexcept {
			if(fov < this->near_plane || fov > this->far_plane) {
				LOG_ERROR("new \"fov\" value must be between \"near plane\" and \"far plane\" current values");
				return;
			}

			this->fov = fov;
			this->update_proj_matrix();
		}

		// Sets minimum amount of fov. Used for zoom out
		inline void set_min_fov(const float min_fov) noexcept {
			if(min_fov < this->max_fov) {
				LOG_ERROR("new \"min fov\" value can't be higher than \"max fov\" current value");
				return;
			}

			this->min_fov = min_fov;
		}

		// Sets maximum amount of fov. Used for zoom in
		inline void set_max_fov(const float max_fov) noexcept {
			if(max_fov < this->min_fov) {
				LOG_ERROR("new \"max fov\" value can't be lower than \"min fov\" current value");
				return;
			}

			this->max_fov = max_fov;
		}

		// Zoom in or out the camera using an speed value
		inline void zoom(const float speed, const Camera::Zoom zoom_dir) noexcept {
			this->fov = (zoom_dir == Zoom::IN)
				? std::max(fov - speed, min_fov)
				: std::min(fov + speed, max_fov);

			this->update_proj_matrix();
		}

		// Sets camera movement speed
		inline void set_speed(const float speed) noexcept {
			this->speed = speed / 10.0f;
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

		// Moves the camera using a unit vector.
		// Where X is front/back, Y is up/down and Z is left/right.
		// Example: [1, 0, -1] means the camera moved to Front-Left
		void move(const vec3<float>& dir, const float dt) noexcept;

		// Use mouse moved direction to rotate the camera
		// void rotate(const MouseHandler& mouse) noexcept;
		void rotate(const Window& mouse) noexcept;

		// Returns a string with the cardinal direction the camera is facing based on its yaw
		static std::string_view get_cardinal_direction(const float yaw) {
			// Define the cardinal directions
			static  std::array<std::string_view, 8> directions = {
				"North (-Z)", "Northeast", "East (+X)", "Southeast",
				"South (+Z)", "Southwest", "West (-X)", "Northwest"
			};

			// Determine the sector index. Normalizing yaw to [0, 360]
			return directions[static_cast<size_t>((std::fmod((yaw + 360.0f), 360.0f) + 22.5f) / 45.0f) % 8];
		}

	private:
		uint32 width;
		uint32 height;

		float speed = 0.1f;

		float fov;
		float max_fov = 100.0f;
		float min_fov = fov;

		float near_plane = 0.1f;
		float far_plane  = 100.0f;

		// Movement
		// bool firstclick = true;
		// -90 in yaw prevents camera from jumping on the first click

		vec3<float> up          = vec3<float>(0.0f, 1.0f, 0.0f);
		vec3<float> forward     = vec3<float>(0.0f, 0.0f, -1.0f);
		vec3<float> right       = vec3<float>(1.0f, 0.0f, 0.0f);

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
