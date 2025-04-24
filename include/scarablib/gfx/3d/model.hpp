#pragma once

#include <GL/glew.h>
#include <cassert>
#include "scarablib/gfx/3d/boundingbox.hpp"
#include "scarablib/gfx/mesh.hpp"
#include "scarablib/opengl/shader.hpp"
#include "scarablib/proper/log.hpp"
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

		// Draw current Model.
		// It needs a camera object and a shader
		virtual void draw(const Camera& camera, const Shader& shader) noexcept;



		// This returns nullptr since by default is used shader is located in Scene3D.
		// This is a method that is overrided by models using a different shader.
		// Scene3D then checks if the shader is nullptr and if it is, it uses the default shader
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

		// Returns current Z position
		inline float get_z() const noexcept {
			return this->position.z;
		}

		// Returns current position
		inline vec3<float> get_position() const noexcept {
			return this->position;
		}

		// Returns current scale of each axis
		inline vec3<float> get_scale() const noexcept {
			return this->scale;
		}

		// Returns current color
		inline Color get_color() const noexcept {
			return this->color;
		}

		// Returns current angle
		inline float get_angle() const noexcept {
			return this->angle;
		}

		// Returns the model bounding box
		inline const BoundingBox& get_bounding_box() const noexcept {
			return this->bbox;
		}

		// SETTERS //

		// Draw the bounding box of the model
		void draw_collider(const Camera& camera, const Color& color = Colors::RED, const bool stripped = false) noexcept;

		// Apply the transformations of the model until this moment.
		// This is one time only, for dynamic transformation use `set_dynamic_transform(bool)`.
		// If any calculation is made with the bounding box but
		// `set_transform` was not called, the calculation will be wrong.
		// This is used to apply initial transformations into a model
		void set_transform() {
			this->update_model_matrix(); // Needs updated matrix
			this->bbox.update_world_bounds(this->model);
		}

		// Set the bounding box to be recalculated when necessary.
		// This is usefull for dynamic models
		void set_dynamic_transform(const bool value) noexcept {
			this->dynamic_bounding = value;
			this->set_transform(); // Just in case
		}

		// Set a new position using a vector
		inline void set_position(const vec3<float>& position) noexcept {
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

		// Set a new Z position
		inline void set_z(const float newz) noexcept {
			this->position.z = newz;
			this->isdirty = true;
		}

		// Scale the shape using a different value for each axis.
		// e.g., `size.x = size.x * scale.x`, `size.y = size.y * scale.y` and `size.z = size.z * scale.z`
		inline void set_scale(const vec3<float>& scale) noexcept {
			this->scale = scale;
			this->isdirty = true;
		}

		// Set a new color.
		// If using a texture and a color at the same time, the texture will be colorized using the color defined
		inline void set_color(const Color& color) noexcept {
			this->color = color;
		}

		// Set a new rotation angle in degrees.
		// Axis is wich axis the angle will be applied (XYZ)
		void set_rotation(const float angle, const vec3<bool>& axis) noexcept;

		// Set a new orientation to the Model in degrees.
		// Orientation is the "default" angle of the Model.
		// The rotation will be applied using the orientation as base
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
