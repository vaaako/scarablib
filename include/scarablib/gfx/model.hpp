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
		// Struct used to initialize a Model
		struct Config {
			vec3<float> position;
			vec3<float> size = vec3<float>(1.0f); // This will change
			vec3<float> scale = vec3<float>(1.0f);
			Color color = Colors::WHITE;

			// Orientation (default angle of the Model)
			// Orientation of the Model
			float orient_angle = 0.0f;
			// Axis where the orientation angle will be applied
			vec3<float> orient_axis = { 1.0f, 0.0f, 0.0f };

			// Rotation based on orientation
			float angle = 0.0f;
			// Axis where the rotation will be applied
			vec3<float> axis = { 1.0f, 0.0f, 0.0f };
		};

		// Init model using custom config and pre-defined VAO.
		// Vertices and Indices will be used to gerate VBO and EBO in this VAO
		Model(const Model::Config& conf, const std::vector<Vertex>& vertices, const std::vector<uint32>& indices) noexcept;
		// Make a model using a wavefront .obj file
		Model(const char* path) noexcept;
		~Model() = default;

		// Draw current Model.
		// It needs a camera object and a shader
		virtual void draw(Camera& camera, const Shader& shader) noexcept;

		// Get current X position
		inline float get_x() const noexcept {
			return this->conf.position.x;
		}

		// Get current Y position
		inline float get_y() const noexcept {
			return this->conf.position.y;
		}

		// Get current Z position
		inline float get_z() const noexcept {
			return this->conf.position.z;
		}

		// Get current position
		inline vec3<float> get_position() const noexcept {
			return this->conf.position;
		}

		// Get current size of each axis
		inline vec3<float> get_size() const noexcept {
			return this->conf.size;
		}

		// Get current scale of each axis
		inline vec3<float> get_scale() const noexcept {
			return this->conf.scale;
		}

		// Get current color
		inline Color get_color() const noexcept {
			return this->conf.color;
		}

		// Get current angle
		inline float get_angle() const noexcept {
			return this->conf.angle;
		}


		// Get minimum corner of the bounding box (AABB)
		inline vec3<float> get_min() const noexcept {
			return this->min;
		}

		// Get maximum corner of the bounding box (AABB)
		inline vec3<float> get_max() const noexcept {
			return this->max;
		}


		// SETTERS //

		// Set a new position using a vector
		inline Model& set_position(const vec3<float>& position) noexcept {
			this->conf.position = position;
			this->update_min_and_max();
			this->isdirty = true;
			return *this;
		}

		// Set a new X position
		inline Model& set_x(const float newx) noexcept {
			this->conf.position.x = newx;
			this->isdirty = true;
			return *this;
		}

		// Set a new Y position
		inline Model& set_y(const float newy) noexcept {
			this->conf.position.y = newy;
			this->isdirty = true;
			return *this;
		}

		// Set a new Z position
		inline Model& set_z(const float newz) noexcept {
			this->conf.position.z = newz;
			this->isdirty = true;
			return *this;
		}

		// Set a new size using a vector (X and Y)
		inline Model& set_size(const vec3<float>& size) noexcept {
			this->conf.size = size;
			this->update_min_and_max();
			this->isdirty = true;
			return *this;
		}

		// Set a new size using a vector (X and Y)
		inline Model& set_size(const float size) noexcept {
			this->conf.size = { size, size, size };
			this->update_min_and_max();
			this->isdirty = true;
			return *this;
		}

		// Scale the shape using a different value for each axis.
		// e.g., `size.x = size.x * scale.x`, `size.y = size.y * scale.y` and `size.z = size.z * scale.z`
		inline Model& set_scale(const vec3<float>& scale) noexcept {
			this->conf.scale = scale;
			this->isdirty = true;
			return *this;
		}

		// Scale the shape using a different value for each axis.
		// e.g., `size.x = size.x * scale.x`, `size.y = size.y * scale.y` and `size.z = size.z * scale.z`
		inline Model& set_scale(const float scale) noexcept {
			this->conf.scale = { scale, scale, scale };
			this->isdirty = true;
			return *this;
		}

		// Set a new color.
		// If using a texture and a color at the same time, the texture will be colorized using the color defined
		inline Model& set_color(const Color& color) noexcept {
			this->conf.color = color;
			return *this;
		}

		// Set a new rotation angle in degrees.
		// Axis is wich axis the angle will be applied (XYZ)
		Model& set_rotation(const float angle, const vec3<bool> axis) noexcept;

		// Set a new orientation to the Model in degrees.
		// Orientation is the "default" angle of the Model.
		// The rotation will be applied using the orientation as base
		Model& set_orientation(const float angle, const vec3<bool> axis) noexcept;

		// Make a copy of this model
		inline Model make_instance() const noexcept {
			return Model(*this);
		}

	protected:
		glm::mat4 model = glm::mat4(1.0f);
		bool isdirty; // Calculate matrix if anything changed (initializing on constructor so inheritance works)

		// Storing config
		Model::Config conf;

		// min and max corner of the bounding box (AABB)
		// This will be calculated on constructor
		vec3<float> min = vec3<float>(0.0f);
		vec3<float> max = vec3<float>(0.0f);

		// Calculates the size of a shape using it's vertices
		vec3<float> calc_size(const std::vector<Vertex>& vertices) noexcept;

		// Inline //

		inline void update_min_and_max() noexcept {
			// Scale or Size?
			this->min = vec3<float>(
				this->conf.position.x - this->conf.scale.x / 2,
				this->conf.position.y - this->conf.scale.y / 2,
				this->conf.position.z - this->conf.scale.z / 2
			);

			this->max = vec3<float>(
				this->conf.position.x + this->conf.scale.x / 2,
				this->conf.position.y + this->conf.scale.y / 2,
				this->conf.position.z + this->conf.scale.z / 2
			);
		}
};

