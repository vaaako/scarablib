#pragma once

#include <GL/glew.h>
#include "scarablib/gfx/mesh.hpp"
#include "scarablib/opengl/shader.hpp"
#include "scarablib/proper/dirtyproxy.hpp"
#include "scarablib/scenes/camera.hpp"
#include "scarablib/typedef.hpp"
#include "scarablib/types/vertex.hpp"

// An object used for as a base for 3D Shapes
class Model : public Mesh {
	public:
		// Model's position in world space
		DirtyProxy<vec3<float>> position = DirtyProxy(vec3<float>(0.0f), this->isdirty);
		// Model's scale. Default is 1.0f
		DirtyProxy<vec3<float>> scale    = DirtyProxy(vec3<float>(1.0f), this->isdirty);

		// Model is not build, you should provide vertices and indices with `set_geometry` method
		Model() noexcept = default;
		// To make a model use ModelFactory.
		// Init model using custom config and pre-defined VAO.
		// Vertices and Indices will be used to gerate VBO and EBO in this VAO
		template <typename T>
		Model(const std::vector<Vertex>& vertices, const std::vector<T>& indices) noexcept;
		// Make a model using a wavefront .obj file
		Model(const char* path);

		// This method does not draw the model to the screen, as it does not bind the VAO and Shader (batch rendering)
		virtual void draw_logic(const Camera& camera, const Shader& shader) noexcept override;

		// Returns current angle
		inline float get_angle() const noexcept {
			return this->angle;
		}

		inline float get_orientation_angle() const noexcept {
			return this->orient_angle;
		}

		// Sets a new rotation angle.
		// - `angle`: Should be in degrees
		// - `axis`: (must be normalized) Which axis the angle will be applied
		void set_rotation(const float angle, const vec3<float>& axis) noexcept;

		// Sets a new orientation to the model.
		// - `angle`: Should be in degrees
		// - `axis`: (must be normalized) Which axis the angle will be applied
		void set_orientation(const float angle, const vec3<float>& axis) noexcept;


		// TODO: Will be useful later
		// Loads a wavefront .obj file.
		// `indices` is optional, pass nullptr if not needed
		static std::vector<Vertex> load_obj(const char* path, std::vector<uint32>* indices);

	protected:
		// Axis where the orientation angle will be applied
		float orient_angle      = 0.0f;
		// Rotation based on orientation
		vec3<float> orient_axis = vec3<float>(1.0f, 0.0f, 0.0f);
		// Rotation
		float angle             = 0.0f;
		// Need to have at least one axis to work, even if angle is 0.0
		vec3<float> axis        = vec3<float>(1.0f, 0.0f, 0.0f);

		void update_model_matrix() noexcept override;
};


template <typename T>
Model::Model(const std::vector<Vertex>& vertices, const std::vector<T>& indices) noexcept
	: Mesh(vertices, indices) {}
