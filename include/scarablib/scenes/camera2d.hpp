#pragma once

#include "scarablib/typedef.hpp"
#include "scarablib/window/window.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

class Camera2D {
	public:
		Camera2D(const Window& window);

		// MOVE //
		inline void set_position(const vec2<float>& position) noexcept{
			this->position = position;
			this->update_view_matrix();
		}

		inline void move(const vec2<float>& delta, const float dt) noexcept{
			this->position += delta * dt;
			this->update_view_matrix();
		}

		// SETTERS //

		inline void update_viewport(const uint32 width, const uint32 height) noexcept {
			this->width  = static_cast<float>(width);
			this->height = static_cast<float>(height);
			this->update_proj_matrix();
		}

		inline void set_zoom(const float zoom) noexcept{
			this->zoom = zoom;
			this->update_proj_matrix();
		}

		// MATRIX //

		inline glm::mat4 get_view_matrix() const noexcept {
			return this->view;
		}

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
