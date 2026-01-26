#pragma once

#include "scarablib/typedef.hpp"
#include "scarablib/window/window.hpp"

class Camera {
	public:
		vec3<float> position = vec3<float>(0.0f); // 3D default

		Camera(const Window& window) noexcept;

		// Updates viewport using window object
		inline void update_viewport(const Window& window) noexcept {
			this->update_viewport(window.get_width(), window.get_height());
		}

		// Updates viewport using custom width and height
		void update_viewport(const uint32 width, const uint32 height) noexcept;

		// Zoom in or Zoom out camera
		virtual void set_zoom(const float speed) noexcept = 0;

		// Returns camera's projection matrix
		inline glm::mat4 get_proj_matrix() const noexcept {
			return this->proj;
		}

		virtual inline glm::mat4 get_view_matrix() const noexcept = 0;

	protected:
		uint32 width;
		uint32 height;
		float aspectratio;
		float zoom = 1.0f;

		vec3<float> up          = vec3<float>(0.0f, 1.0f, 0.0f);
		vec3<float> forward     = vec3<float>(0.0f, 0.0f, -1.0f);
		vec3<float> right       = vec3<float>(1.0f, 0.0f, 0.0f);

		// Matrices
		glm::mat4 proj;

		// Pre-calculate proj matrix, since it doesnt change often
		virtual void update_proj_matrix() noexcept = 0;
};
