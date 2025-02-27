#pragma once

#include <GL/glew.h>
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
		Model(const std::vector<Vertex>& vertices, const std::vector<uint32>& indices) noexcept;
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

		// Returns the center of the model
		inline vec3<float> get_center_position() const noexcept {
		return this->boxsize.min + (this->boxsize.size * 0.5f);	
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


		// Returns minimum corner of the bounding box (AABB)
		inline vec3<float> get_min() const noexcept {
			return this->bounding->boxsize.min;
		}

		// Returns maximum corner of the bounding box (AABB)
		inline vec3<float> get_max() const noexcept {
			return this->bounding->boxsize.max;
		}

		// Returns the difference of the bounding box min and max.
		// This represents the size of the model
		inline vec3<float> get_size() const noexcept {
			return this->bounding->boxsize.size;
		}


		// Checks if the model has created a collider
		inline bool has_collider() const noexcept {
			return this->bounding != nullptr;
		}

		// Uses AABB to check if two models are colliding
		static inline bool check_collision(const Model& a, const Model& b) noexcept {
			return (a.bounding->boxsize.max.x >= b.bounding->boxsize.min.x &&
					a.bounding->boxsize.min.x <= b.bounding->boxsize.max.x &&
					a.bounding->boxsize.max.y >= b.bounding->boxsize.min.y &&
					a.bounding->boxsize.min.y <= b.bounding->boxsize.max.y &&
					a.bounding->boxsize.max.z >= b.bounding->boxsize.min.z &&
					a.bounding->boxsize.min.z <= b.bounding->boxsize.max.z);
		}

		// SETTERS //

		// This will create a collider for the mesh.
		// NOTE: If your mesh is NOT static, you should also enable the calculation with the method `update_collider(bool)`
		// WARNING: Call this method ONLY AFTER configuring the model
		virtual inline void make_collider() noexcept override {
			this->update_model_matrix();
			this->bounding = new BoundingBox(this->boxsize, this->model);
			this->bounding->update_world_bounding_box(this->model);
		}

		// This will enable the collider to update every time the model is updated.
		inline void update_collider(const bool enable) noexcept {
			if(this->bounding == nullptr) {
				LOG_ERROR("Make the collider to be able to calculate it");
			}

			this->calc_enabled = enable;
		}

		// Show or hide the collider
		inline void show_collider(const bool draw_box) noexcept {
			if(this->bounding == nullptr) {
				LOG_ERROR("Please make the collider to be able to show it");
			}

			this->draw_box = draw_box;
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

		// Collission
		BoundingBox* bounding = nullptr;
		bool draw_box         = false;
		bool calc_enabled     = false;

		void update_model_matrix() noexcept;
};

