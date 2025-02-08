#pragma once

#include <GL/glew.h>
#include "scarablib/gfx/3d/boundingbox.hpp"
#include "scarablib/gfx/mesh.hpp"
#include "scarablib/opengl/shader.hpp"
#include "scarablib/scenes/camera.hpp"
#include "scarablib/typedef.hpp"
#include "scarablib/types/vertex.hpp"

class Scene3D;

// An object used for as a base for 3D Shapes
class Model : public Mesh {
	// Access bounding object and model matrix
	friend Scene3D;

	public:
		// Struct used to initialize a Model
		struct Config {
			vec3<float> position;
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
			vec3<float> axis = { 1.0f, 0.0f, 0.0f }; // Need to have at least one axis to work, even if angle is 0.0
		};

		// Init model using custom config and pre-defined VAO.
		// Vertices and Indices will be used to gerate VBO and EBO in this VAO
		Model(const Model::Config& conf, const std::vector<Vertex>& vertices, const std::vector<uint32>& indices) noexcept;
		// Make a model using a wavefront .obj file
		Model(const char* path) noexcept;

		// Draw current Model.
		// It needs a camera object and a shader
		virtual void draw(const Camera& camera, const Shader& shader) noexcept;

		// Uses AABB to check if two models are colliding
		static inline bool check_collision(const Model& a, const Model& b) noexcept {
			return (a.bounding.box_max.x >= b.bounding.box_min.x &&
					a.bounding.box_min.x <= b.bounding.box_max.x &&
					a.bounding.box_max.y >= b.bounding.box_min.y &&
					a.bounding.box_min.y <= b.bounding.box_max.y &&
					a.bounding.box_max.z >= b.bounding.box_min.z &&
					a.bounding.box_min.z <= b.bounding.box_max.z);
		}

		// Returns the position of the center of the model
		inline vec3<float> center_pos() const noexcept {
			return (this->bounding.box_min + this->bounding.box_max) * 0.5f;
		}


		// Returns current X position
		inline float get_x() const noexcept {
			return this->conf.position.x;
		}

		// Returns current Y position
		inline float get_y() const noexcept {
			return this->conf.position.y;
		}

		// Returns current Z position
		inline float get_z() const noexcept {
			return this->conf.position.z;
		}

		// Returns current position
		inline vec3<float> get_position() const noexcept {
			return this->conf.position;
		}

		// Returns current scale of each axis
		inline vec3<float> get_scale() const noexcept {
			return this->conf.scale;
		}

		// Returns current color
		inline Color get_color() const noexcept {
			return this->conf.color;
		}

		// Returns current angle
		inline float get_angle() const noexcept {
			return this->conf.angle;
		}


		// Returns minimum corner of the bounding box (AABB)
		inline vec3<float> get_box_min() const noexcept {
			return this->bounding.box_min;
		}

		// Returns maximum corner of the bounding box (AABB)
		inline vec3<float> get_box_max() const noexcept {
			return this->bounding.box_max;
		}

		// Returns the difference of the bounding box min and max.
		// This represents the size of the model
		inline vec3<float> get_box_size() const noexcept {
			return this->bounding.box_size;
		}

		// Returns if the model is showing the bounding box
		inline bool is_showing_box() const noexcept {
			return this->show_box;
		}


		// SETTERS //

		// Set a new position using a vector
		inline void set_position(const vec3<float>& position) noexcept {
			this->conf.position = position;
			// this->update_min_and_max();
			this->isdirty = true;
		}

		// Set a new X position
		inline void set_x(const float newx) noexcept {
			this->conf.position.x = newx;
			this->isdirty = true;
		}

		// Set a new Y position
		inline void set_y(const float newy) noexcept {
			this->conf.position.y = newy;
			this->isdirty = true;
		}

		// Set a new Z position
		inline void set_z(const float newz) noexcept {
			this->conf.position.z = newz;
			this->isdirty = true;
		}

		// Set a new size using a vector (X and Y)
		// inline void set_size(const vec3<float>& size) noexcept {
		// 	this->conf.size = size;
		// 	this->update_min_and_max();
		// 	this->isdirty = true;
		// }
		//
		// Set a new size using a vector (X and Y)
		// inline void set_size(const float size) noexcept {
		// 	this->conf.size = { size, size, size };
		// 	this->update_min_and_max();
		// 	this->isdirty = true;
		// }

		// Scale the shape using a different value for each axis.
		// e.g., `size.x = size.x * scale.x`, `size.y = size.y * scale.y` and `size.z = size.z * scale.z`
		inline void set_scale(const vec3<float>& scale) noexcept {
			this->conf.scale = scale;
			this->isdirty = true;
		}

		// Scale the shape using a different value for each axis.
		// e.g., `size.x = size.x * scale.x`, `size.y = size.y * scale.y` and `size.z = size.z * scale.z`
		inline void set_scale(const float scale) noexcept {
			vec3<float> newscale = { scale, scale, scale };
			this->conf.scale = newscale;
			this->isdirty = true;
		}

		// Show or hide the bounding box
		inline void show_bounding_box(const bool show_box) noexcept {
			this->show_box = show_box;
		}

		// If the Model bounding box is not changing as expected or the collision is failed, you may have forget to call this function.
		// This will enable the bounding box update calculation every time the model is updated
		inline void enable_collision(const bool enable) noexcept {
			this->collision_enabled = enable;
		}

		// Set a new color.
		// If using a texture and a color at the same time, the texture will be colorized using the color defined
		inline void set_color(const Color& color) noexcept {
			this->conf.color = color;
		}

		// Set a new rotation angle in degrees.
		// Axis is wich axis the angle will be applied (XYZ)
		void set_rotation(const float angle, const vec3<bool> axis) noexcept;

		// Set a new orientation to the Model in degrees.
		// Orientation is the "default" angle of the Model.
		// The rotation will be applied using the orientation as base
		void set_orientation(const float angle, const vec3<bool> axis) noexcept;

		// Make a copy of this model
		inline Model make_instance() const noexcept {
			return Model(*this);
		}

		// This returns nullptr since primarily used shader is located in Scene3D.
		// This is a method that is overrided by models using a different shader.
		// Scene3D then checks if the shader is nullptr and if it is, it uses the default shader
		virtual inline Shader* get_shader() const noexcept {
			return nullptr;
		}

	protected:
		glm::mat4 model = glm::mat4(1.0f);
		bool isdirty; // Calculate matrix if anything changed (initializing on constructor so inheritance works)

		// Storing config
		Model::Config conf;

		// Collission
		BoundingBox bounding;
		bool show_box = false;
		bool collision_enabled = false;


		void update_model_matrix() noexcept;
};

