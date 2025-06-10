#pragma once

#include <GL/glew.h>
#include "scarablib/gfx/mesh.hpp"
#include "scarablib/opengl/shader.hpp"
#include "scarablib/scenes/camera.hpp"
#include "scarablib/typedef.hpp"
#include "scarablib/types/vertex.hpp"

// An object used for as a base for 3D Shapes
class Model : public Mesh {
	public:
		// Model is not build, you should provide vertices and indices with `set_geometry` method
		Model() noexcept = default;
		// To make a model use ModelFactory.
		// Init model using custom config and pre-defined VAO.
		// Vertices and Indices will be used to gerate VBO and EBO in this VAO
		template <typename T>
		Model(const std::vector<Vertex>& vertices, const std::vector<T>& indices) noexcept;
		// Make a model using a wavefront .obj file
		Model(const char* path) noexcept;

		// Make a copy of this model
		// inline Model make_instance() const noexcept {
		// 	return Model(*this);
		// }

		// This method does not draw the model to the screen, as it does not bind the VAO and Shader.
		// This is a auxiliary method used by Scene3D. It does not bind VAO and Shader because of batch rendering.
		// This is the default draw method and a different model may have a overrided draw method (like billboard)
		virtual void draw(const Camera& camera, const Shader& shader) noexcept;

		// Returns current position
		inline vec3<float> get_position() const noexcept {
			return this->position;
		}

		// Returns current scale
		inline vec3<float> get_scale() const noexcept {
			return this->scale;
		}

		// Returns current color
		inline Color& get_color() noexcept {
			return this->color;
		}

		// Returns current angle
		inline float get_angle() const noexcept {
			return this->angle;
		}

		// SETTERS //

		// Sets a new position using a 3D vector
		inline void set_position(const vec3<float>& position) noexcept {
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

		// Sets a new Z position
		inline void set_z(const float newz) noexcept {
			this->position.z = newz;
			this->isdirty = true;
		}

		// Moves the sprite using a 3D vector
		inline void move(const vec3<float>& offset) noexcept {
			this->position += offset;
			this->isdirty = true;
		}

		// Sets a new scale using a 3D vector
		inline void set_scale(const vec3<float>& scale) noexcept {
			this->scale = scale;
			this->isdirty = true;
		}

		// Sets a new color.
		// New color affects texture color if a texture is in use
		inline void set_color(const Color& color) noexcept {
			this->color = color;
		}

		// Sets a new rotation angle.
		// - `angle`: should be in degrees
		// - `axis`: which axis the angle will be applied (x, y, z)
		void set_rotation(const float angle, const vec3<bool>& axis) noexcept;

		// Sets a new orientation to the model.
		// - `angle`: should be in degrees
		// - `axis`: which axis the orientation will be applied (x, y, z)
		void set_orientation(const float angle, const vec3<bool>& axis) noexcept;

	protected:
		// Attributes
		vec3<float> position    = vec3<float>(0.0f);
		vec3<float> scale       = vec3<float>(1.0f);
		Color color             = Colors::WHITE;
		float orient_angle      = 0.0f;
		vec3<float> orient_axis = { 1.0f, 0.0f, 0.0f }; // Axis where the orientation angle will be applied
		float angle             = 0.0f;                 // Rotation based on orientation
		vec3<float> axis        = { 1.0f, 0.0f, 0.0f }; // Need to have at least one axis to work, even if angle is 0.0

		void update_model_matrix() noexcept override;
};


template <typename T>
Model::Model(const std::vector<Vertex>& vertices, const std::vector<T>& indices) noexcept
	: Mesh(vertices, indices) {}
