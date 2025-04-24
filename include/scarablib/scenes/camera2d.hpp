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
		inline void set_position(const vec2<float>& position) noexcept{
			this->position = position;
			this->update_view_matrix();
		}

		// Moves camera around
		inline void move(const vec2<float>& delta, const float dt) noexcept{
			this->position += delta * dt;
			this->update_view_matrix();
		}

		// SETTERS //

		// Updates viewport using window object
		inline void update_viewport(const Window& window) noexcept {
			this->update_viewport(window.get_width(), window.get_height());
		}

		// Updates viewport using custom width and height
		inline void update_viewport(const uint32 width, const uint32 height) noexcept {
			this->width  = static_cast<float>(width);
			this->height = static_cast<float>(height);
			this->update_proj_matrix();
		}

		// Sets camera's zoom
		inline void set_zoom(const float zoom) noexcept{
			this->zoom = zoom;
			this->update_proj_matrix();
		}

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

		inline void update_proj_matrix() noexcept {
			// Orthogonal projection
			this->proj = glm::mat4(1.0f);
			this->proj = glm::ortho(0.0f, this->width, this->height, 0.0f, -1.0f, 1.0f);
			// Apply zoom
			this->proj = glm::scale(this->proj, vec3<float>(zoom, zoom, 1.0f));
		}

		inline void update_view_matrix() {
			this->view = glm::mat4(1.0f);
			// Move camera in opposite direction
			this->view = glm::translate(this->view, glm::vec3(-this->position.x, -this->position.y, 0.0f));
		}
};
