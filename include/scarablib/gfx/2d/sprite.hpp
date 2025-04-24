#pragma once

#include <GL/glew.h>
#include "scarablib/gfx/mesh.hpp"
#include "scarablib/opengl/shader.hpp"
#include "scarablib/scenes/camera2d.hpp"
#include "scarablib/types/color.hpp"
#include "scarablib/types/vertex.hpp"

// Avoid circular import
class Scene2D;

// Renderable 2D object with transform/state
class Sprite : public Mesh {
	public:
		// Position of the mesh in world.
		// Use this as read-only.
		// Changes will only be applied if using `set_position`
		vec2<float> position;
		// Size of the mesh.
		// Use this as read-only.
		// Changes will only be applied if using `set_size`
		vec2<float> size = vec2<float>(1.0f, 1.0f);
		// Color of the mesh.
		// Use this as read-only.
		// Changes will only be applied if using `set_color`
		Color color = Colors::WHITE;
		// Angle of the mesh.
		// Use this as read-only.
		// Changes will only be applied if using `set_angle`
		float angle = 0.0f;

		Sprite(const std::vector<Vertex>& vertices) noexcept;

		// Scene2D call this method.
		// Draw current shape using shader defined by Scene2D class
		virtual void draw(const Camera2D& camera, const Shader& shader) noexcept;

		// GETTERS //

		// This returns nullptr, since by default, will be using the shader of Scene2D.
		// This is a method that is overrided by models using a different shaders.
		// Scene2D checks if this method returns a nullptr, if does, it uses the default shader,
		// if not, uses the shader inside this method
		virtual inline Shader* get_shader() const noexcept {
			return nullptr;
		}

		// SETTERS //

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

		// Set a new color.
		// If using a texture and a color at the same time,
		// the texture will be colorized using the color defined
		inline void set_color(const Color& color) noexcept {
			this->color = color;
		}

		// Set a new rotation angle
		inline void set_angle(const float angle) noexcept {
			this->angle = angle;
			this->isdirty = true;
		}

	protected:
		bool isdirty = false; // Re-calculates model matrix if anything has changed
		glm::mat4 model = glm::mat4(1.0f);

		void update_model_matrix() noexcept;
};

