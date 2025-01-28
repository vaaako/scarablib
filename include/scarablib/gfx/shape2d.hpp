#pragma once

#include <GL/glew.h>
#include "scarablib/opengl/shader.hpp"
#include "scarablib/types/color.hpp"
#include "scarablib/types/texture.hpp"


// Struct used to initialize Shape2D
struct Shape2DConf {
	vec2<float> position;
	vec2<float> size;
	Color color = Colors::WHITE;
	float angle = 0.0f;
};

// This is a struct used to make 2D shapes
class Shape2D {
	public:
		// Initialize current shape using position, size, color and angle (last two are optional)
		Shape2D(const Shape2DConf& conf) noexcept;
		virtual ~Shape2D() = default;

		// Scene2D call this method.
		// Draw current shape using shader defined by Scene2D class
		virtual void draw(const Shader& shader) = 0;

		// GETTERS //

		// Get current texture
		inline Texture get_texture() const noexcept {
			return *this->texture;
		}

		// Get current position
		inline vec2<float> get_position() const noexcept {
			return this->conf.position;
		}

		// Get current size of each axis
		inline vec2<float> get_size() const noexcept {
			return this->conf.size;
		}

		// Get current color
		inline Color get_color() const noexcept {
			return this->conf.color;
		}

		// Get current angle
		inline float get_angle() const noexcept {
			return this->conf.angle;
		}

		// SETTERS //

		// Set a texture to be used
		inline Shape2D& set_texture(Texture* texture) noexcept {
			if(texture == nullptr){
				this->texture = &this->get_deftex();
				return *this;
			}

			this->texture = texture;
			return *this;
		}

		// Removes the shape's texture
		inline Shape2D& remove_texture() noexcept {
			this->texture = &this->get_deftex();
			return *this;
		}

		// Set a new position using a vector
		inline Shape2D& set_position(const vec2<float>& position) noexcept {
			this->conf.position = position;
			this->isdirty = true;
			return *this;
		}

		// Set a new size using a vector (X and Y)
		inline Shape2D& set_size(const vec2<float>& size) noexcept {
			this->conf.size = size;
			this->isdirty = true;
			return *this;
		}

		// Scale the shape using a single value for all axis.
		// e.g., `size = size.xy * scale`
		// inline Shape2D& set_scale(const float& scale) {
		// 	this->conf.size * scale;
		// 	this->isdirty = true;
		// 	return *this;
		// }

		// Scale the shape using a different value for each axis.
		// e.g., `size.x = size.x * scale.x` and `size.y = size.y * scale.y`
		// inline Shape2D& set_scale(const vec2<float>& scale) {
		// 	this->conf.size * scale;
		// 	this->isdirty = true;
		// 	return *this;
		// }

		// Set a new color
		// If using a texture and a color at the same time, the texture will be colorized using the color defined
		inline Shape2D& set_color(const Color& color) noexcept {
			this->conf.color = color;
			return *this;
		}

		// Set a new rotation angle
		inline Shape2D& set_angle(const float angle) noexcept {
			this->conf.angle = angle;
			this->isdirty = true;
			return *this;
		}

	protected:
		void update_model() noexcept;

		// Storing config
		Shape2DConf conf;

		// NOTE -- Texture will always be a "reference" to another existing texture (except for fonts). Using a pointer so the OpenGL ID doens't get copied
		Texture* texture = &this->get_deftex(); // Current texture being used

		// These need to be intialized on constructor, so the inheritance goes right
		glm::mat4 model;
		bool isdirty; // Calculate matrix if anything changed

		// Default texture (solid white)
		inline Texture& get_deftex() const noexcept {
			static Texture def_tex = Texture(); // Make solid white texture
			return def_tex;
		}
};

