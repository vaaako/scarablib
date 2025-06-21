#pragma once

#include "scarablib/scenes/camera.hpp"
#include "scarablib/typedef.hpp"
#include "scarablib/window/window.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

// Camera for 2D objects
class Camera2D : public Camera {
	public:
		Camera2D(const Window& window) noexcept;

		// Moves camera around
		void move(const vec2<float>& delta, const float dt) noexcept;

		// SETTERS //

		// Changes camera position
		void set_position(const vec2<float>& position) noexcept;

		// Sets camera's zoom
		void set_zoom(const float zoom) noexcept override;

		// MATRIX //

		// Returns camera's view matrix
		inline glm::mat4 get_view_matrix() const noexcept override{
			return this->view;
		}

	private:
		vec2<float> position = vec2<float>(0.0f, 0.0f);

		glm::mat4 view;

		void update_proj_matrix() noexcept override;
		void update_view_matrix() noexcept;
};
