#pragma once

#include <GL/glew.h>
#include "scarablib/opengl/shader.hpp"
#include "scarablib/proper/vector/vec2.hpp"
#include "scarablib/types/color.hpp"

// This is a struct used to make 2D shapes
class Shape2D {
	public:
		// Initialize current shape using the following values:
		// - `position`: Shape's position on the screen
		// - `size`: Size of the shape in pixels
		// - `color`: Shape's color (white by default)
		// - `angle`: Shape's angle (0.0f by default)
		Shape2D(const vec2<float>& position, const vec2<float>& size, const Color& color = Colors::WHITE, const float angle = 0.0f);

		// Scene2D call this method.
		// Draw current shape using shader defined by Scene2D class
		virtual void draw(const Shader& shader) const = 0;

		// GETTERS //

		// Get current position
		inline vec2<float> get_position() const {
			return this->position;
		}

		// Get current size of each axis
		inline vec2<float> get_size() const {
			return this->size;
		}

		// Get current color
		inline Color get_color() const {
			return this->color;
		}

		// Get current angle
		inline float get_angle() const {
			return this->angle;
		}

		// SETTERS //

		// Set a new position using a vector
		inline void set_position(const vec2<float>& position) {
			this->position = position;
			this->isdirty = true;
		}

		// Set a new size using a vector (X and Y)
		inline void set_size(const vec2<float>& size) {
			this->size = size;
			this->isdirty = true;
		}

		// Scale the shape using a single value for all axis.
		// e.g., `size = size.xy * scale`
		inline void set_scale(const float& scale) {
			this->size * scale;
			this->isdirty = true;
		}

		// Scale the shape using a different value for each axis.
		// e.g., `size.x = size.x * scale` / `size.y = size.y * scale`
		inline void set_scale(const vec2<float>& scale) {
			this->size * scale;
			this->isdirty = true;
		}

		// Set a new color
		// If using a texture and a color at the same time, the texture will be colorized using the color defined
		inline void set_color(const Color& color) {
			this->color = color;
			this->isdirty = true;
		}

		// Set a new rotation angle
		inline void set_angle(const float angle) {
			this->angle = angle;
			this->isdirty = true;
		}

	protected:
		vec2<float> position;
		vec2<float> size;
		Color color;
		float angle;

		// This need to be intialized on constructor, so the inheritance goes well
		mutable glm::mat4 model;
		mutable bool isdirty; // Change model if changed
};

