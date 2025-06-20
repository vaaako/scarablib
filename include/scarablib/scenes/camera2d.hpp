#pragma once

#include "scarablib/typedef.hpp"
#include "scarablib/window/window.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

// Camera for 2D objects
class Camera2D {
	public:
		Camera2D(const Window& window);

		// MOVE //

		// Sets camera's position
		void set_position(const vec2<float>& position) noexcept;

		// Moves camera around
		void move(const vec2<float>& delta, const float dt) noexcept;

		// SETTERS //

		// Updates viewport using window object
		inline void update_viewport(const Window& window) noexcept {
			this->update_viewport(window.get_width(), window.get_height());
		}

		// Updates viewport using custom width and height
		void update_viewport(const uint32 width, const uint32 height) noexcept;

		// Sets camera's zoom
		void set_zoom(const float zoom) noexcept;

		// MATRIX //

		// Returns camera's view matrix
		inline glm::mat4 get_view_matrix() const noexcept {
			return this->view;
		}

		// Returns camera's projection matrix
		inline glm::mat4 get_proj_matrix() const noexcept {
			return this->proj;
		}

	private:
		vec2<float> position = vec2<float>(0.0f, 0.0f);

		float width;
		float height;

		float zoom = 1.0f;
		glm::mat4 proj;
		glm::mat4 view;

		void update_proj_matrix() noexcept;
		void update_view_matrix() noexcept;
};
