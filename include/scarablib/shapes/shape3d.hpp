#pragma once

#include "glm/fwd.hpp"
#include "scarablib/opengl/shader.hpp"
#include "scarablib/proper/vector/vec3.hpp"
#include "scarablib/types/color.hpp"
#include "scarablib/types/texture.hpp"

class Shape3D {
	public:
		Shape3D(const vec3<float>& position, const vec3<float>& size, const Color& color = Colors::WHITE, const float angle = 0.0f);

		virtual void draw(const Shader& shader) = 0;

		// Get current texture
		inline Texture get_texture() const {
			return *this->texture;
		}

		// Get current position
		inline vec3<float> get_position() const {
			return this->position;
		}

		// Get current size of each axis
		inline vec3<float> get_size() const {
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

		// Set a texture to be used
		inline void set_texture(Texture* texture) {
			if(texture == nullptr){
				this->texture = &this->get_deftex();
				return;
			}

			this->texture = texture;
		}

		// Removes the shape's texture
		inline void remove_texture() {
			this->texture = &this->get_deftex();
		}

		// Set a new position using a vector
		inline void set_position(const vec3<float>& position) {
			this->position = position;
			this->isdirty = true;
		}

		// Set a new size using a vector (X, Y and Z)
		inline void set_size(const vec3<float>& size) {
			this->size = size;
			this->isdirty = true;
		}

		// Scale the shape using a single value for all axis.
		// e.g., `size = size.xyz * scale`
		inline void set_scale(const float& scale) {
			this->size * scale;
			this->isdirty = true;
		}

		// Scale the shape using a different value for each axis.
		// e.g., `size.x = size.x * scale.x`, `size.y = size.y * scale.y` and `size.z = size.z * scale.z`
		inline void set_scale(const vec3<float>& scale) {
			this->size * scale;
			this->isdirty = true;
		}

		// Set a new color
		// If using a texture and a color at the same time, the texture will be colorized using the color defined
		inline void set_color(const Color& color) {
			this->color = color;
		}

		// Set a new rotation angle
		inline void set_angle(const float angle) {
			this->angle = angle;
			this->isdirty = true;
		}

	protected:
		void update_model();

		vec3<float> position;
		vec3<float> size;
		Color color;
		float angle;

		// Texture will always be a "reference" to another existing texture
		Texture* texture = &this->get_deftex(); // Current texture being used

		// This need to be intialized on constructor, so the inheritance goes well
		glm::mat4 model;
		bool isdirty; // Change model if changed

		// Default texture (solid white)
		inline Texture& get_deftex() const {
			// I don't like data being statically allocated but whatever
			static uint8 data[4] = { 255, 255, 255, 255 }; 
			static Texture def_tex = Texture(data, 1, 1, GL_RGBA);
			return def_tex;
		}
};
