#pragma once

#include <GL/glew.h>
#include "scarablib/gfx/mesh.hpp"
#include "scarablib/opengl/shader.hpp"
#include "scarablib/scenes/camera.hpp"
#include "scarablib/typedef.hpp"
#include "scarablib/types/vertex.hpp"

class Scene3D;

// An object used for as a base for 3D Shapes
class Model : public Mesh {
	// Access bounding object and model matrix without using get method (less memory override)
	friend Scene3D;

	public:
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

		// Draws the bounding box of the model.
		// If the bounding box is in world origin (0, 0, 0) you need to call `set_transform()`
		void draw_collider(const Camera& camera, const Color& color = Colors::RED, const bool stripped = false) noexcept;

		// Applies the model's transformations to the bounding box.
		// For dynamic transformations, use `set_dynamic_transform(bool)`.
		// Important: Call this *before* any bounding box calculations, otherwise, results will be incorrect.
		void set_transform() {
			this->update_model_matrix(); // Needs updated matrix
			this->bbox.update_world_bounds(this->model);
		}

		// Sets whether the bounding box should be dynamically recalculated
		void set_dynamic_transform(const bool value) noexcept {
			this->dynamic_bounding = value;
			this->set_transform(); // Just in case
		}

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

		// Matrix
		bool isdirty; // Calculate matrix if anything changed
		glm::mat4 model = glm::mat4(1.0f);

		bool dynamic_bounding = false;

		void update_model_matrix() noexcept;
};


template <typename T>
Model::Model(const std::vector<Vertex>& vertices, const std::vector<T>& indices) noexcept
	: Mesh(vertices, indices) {}
