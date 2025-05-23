#pragma once

#include <GL/glew.h>
#include "scarablib/gfx/mesh.hpp"
#include "scarablib/opengl/shader.hpp"
#include "scarablib/scenes/camera2d.hpp"
#include "scarablib/types/color.hpp"
#include "scarablib/types/vertex.hpp"

// Renderable 2D object with transform/state
class Sprite : public Mesh {
	public:
		Sprite(const std::vector<Vertex>& vertices) noexcept;

		// Draws the shape
		virtual void draw(const Camera2D& camera, const Shader& shader) noexcept;


		// GETTERS //

		// Returns current position
		inline vec2<float> get_position() const noexcept {
			return this->position;
		}

		// Returns current size
		inline vec2<float> get_size() const noexcept {
			return this->size;
		}

		// Returns color reference
		inline Color& get_color() noexcept {
			return this->color;
		}

		// Returns current rotation angle
		inline float get_angle() const noexcept {
			return this->angle;
		}

		// SETTERS //

		// Sets a new position using a 2D vector.
		inline void set_position(const vec2<float>& position) noexcept {
			this->position = position;
			this->isdirty = true;
		}

		// Sets a new X position
		inline void set_x(const float newx) noexcept {
			this->position.x = newx;
			this->isdirty = true;
		}

		// Sets a new Y position
		inline void set_y(const float newy) noexcept {
			this->position.y = newy;
			this->isdirty = true;
		}

		// Moves the sprite using a 2D vector
		inline void move(const vec2<float>& offset) noexcept {
			this->position += offset;
			this->isdirty = true;
		}

		// Set a new size using a 2D vector
		inline void set_size(const vec2<float>& size) noexcept {
			this->size = size;
			this->isdirty = true;
		}

		// Sets a new color.
		// New color affects texture color if a texture is in use
		inline void set_color(const Color& color) noexcept {
			this->color = color;
		}

		// Sets a new rotation angle.
		// `angle` should be in degrees
		inline void set_angle(const float angle) noexcept {
			this->angle = angle;
			this->isdirty = true;
		}

	protected:
		// Attributes
		vec2<float> position;
		vec2<float> size = vec2<float>(1.0f, 1.0f);
		Color color      = Colors::WHITE;
		float angle      = 0.0f;

		bool isdirty = false; // Re-calculates model matrix if anything has changed
		glm::mat4 model = glm::mat4(1.0f);

		void update_model_matrix() noexcept;
};

