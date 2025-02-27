#pragma once

#include <GL/glew.h>
#include "scarablib/gfx/mesh.hpp"
#include "scarablib/opengl/shader.hpp"
#include "scarablib/scenes/camera2d.hpp"
#include "scarablib/types/color.hpp"
#include "scarablib/types/vertex.hpp"

// Avoid circular dependencies
class Scene2D;

// This is a struct used to make 2D shapes
class Sprite : public Mesh {
	public:
		// For making a sprite use SpriteFactory
		Sprite(const std::vector<Vertex>& vertices) noexcept;

		// Scene2D call this method.
		// Draw current shape using shader defined by Scene2D class
		virtual void draw(const Camera2D& camera, const Shader& shader) noexcept;

		// GETTERS //

		// This returns nullptr since by default is used shader is located in Scene2D.
		// This is a method that is overrided by models using a different shader.
		// Scene2D then checks if the shader is nullptr and if it is, it uses the default shader
		virtual inline Shader* get_shader() const noexcept {
			return nullptr;
		}

		// Returns current X position
		inline float get_x() const noexcept {
			return this->position.x;
		}

		// Returns current Y position
		inline float get_y() const noexcept {
			return this->position.y;
		}

		// Returns current position
		inline vec2<float> get_position() const noexcept {
			return this->position;
		}

		// Returns current size of each axis
		inline vec2<float> get_size() const noexcept {
			return this->size;
		}

		// Returns current color
		inline Color get_color() const noexcept {
			return this->color;
		}

		// Returns current angle
		inline float get_angle() const noexcept {
			return this->angle;
		}

		// SETTERS //

		// This method does nothing yet
		inline void make_collider() noexcept override {

		}

		// Set a new position using a vector
		inline void set_position(const vec2<float>& position) noexcept {
			this->position = position;
			this->isdirty = true;
		}

		// Set a new X position
		inline void set_x(const float newx) noexcept {
			this->position.x = newx;
			this->isdirty = true;
		}

		// Set a new Y position
		inline void set_y(const float newy) noexcept {
			this->position.y = newy;
			this->isdirty = true;
		}

		// Set a new size using a vector (X and Y)
		inline void set_size(const vec2<float>& size) noexcept {
			this->size = size;
			this->isdirty = true;
		}

		// Set a new color
		// If using a texture and a color at the same time, the texture will be colorized using the color defined
		inline void set_color(const Color& color) noexcept {
			this->color = color;
		}

		// Set a new rotation angle
		inline void set_angle(const float angle) noexcept {
			this->angle = angle;
			this->isdirty = true;
		}

	protected:
		// Attributes
		vec2<float> position;
		vec2<float> size = vec2<float>(1.0f, 1.0f);
		Color color = Colors::WHITE;
		float angle = 0.0f;

		// Matrix
		bool isdirty = false; // Calculate matrix if anything changed
		glm::mat4 model = glm::mat4(1.0f);

		void update_model_matrix() noexcept;
};

