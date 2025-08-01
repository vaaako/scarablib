#pragma once

#include <GL/glew.h>
#include <memory>
#include "scarablib/components/boundingbox.hpp"
#include "scarablib/components/materialcomponent.hpp"
#include "scarablib/components/physicscomponent.hpp"
#include "scarablib/opengl/shader.hpp"
#include "scarablib/opengl/vertexbuffercomponent.hpp"

// Basic data for 3D and 2D shapes
class Mesh {
	public:
		// Material of this mesh
		std::shared_ptr<MaterialComponent> material = std::make_shared<MaterialComponent>();
		// Since material can be shared i need to be a pointer so a double delete is not done

		// Bundle for VAO, VBO and EBO
		VertexBufferComponent bundle;
		// This is kinda wrong, because each instance of a Mesh will have copied bundle (but same VAO at least)

		// Bounding box
		BoundingBox* bbox = nullptr;
		// I wish this was in Model class, but i don't want to store vertices
		// and i need it to build a bounding box

		// Physics component
		PhysicsComponent* physics = nullptr;

		// Mesh is not build, you should provide vertices and indices with `set_geometry` method
		Mesh() noexcept = default;
		// Build Mesh using vertices and indices
		template <typename T>
		Mesh(const std::vector<Vertex>& vertices, const std::vector<T>& indices) noexcept;
		// Build Mesh using only vertices.
		// Mainly used for 2D Shapes. It will not make a collider
		Mesh(const std::vector<Vertex>& vertices) noexcept;

		virtual ~Mesh() noexcept;

		// This method does not draw the Mesh to the screen.
		// As it does not bind the VAO, Shader and Texture (batch rendering)
		virtual void draw_logic(const Camera& camera, const Shader& shader) noexcept = 0;

		// Build Mesh using vertices and indices
		template <typename T>
		void set_geometry(const std::vector<Vertex>& vertices, const std::vector<T>& indices);

		inline glm::mat4 get_model_matrix() const noexcept {
			return this->model;
		}

		// BoundingBox must be created before calling this function.
		// Updates the bounding box based on the transformations of the Model.
		// For dynamic transformations, use `set_dynamic_bbox_update(bool)`
		void update_bbox() {
			if(this->bbox == nullptr) {
				return;
			}
			this->update_model_matrix(); // Needs updated matrix
			this->bbox->update_world_bounds(this->model);
		}

		// Sets whether the bounding box should be dynamically transformed
		void set_dynamic_bbox_update(const bool value) noexcept {
			if(this->bbox == nullptr) {
				return;
			}
			this->dynamic_bounding = value;
		}

		virtual void update_model_matrix() noexcept = 0;

		// Returns nullptr, as the default shader from Scene2D is used.
		// Overridden by models using custom shaders
		virtual inline Shader* get_shader() const noexcept {
			return nullptr;
		}

	protected:
		// Matrix
		bool isdirty; // Calculate matrix if anything changed
		glm::mat4 model = glm::mat4(1.0f);

		bool dynamic_bounding = false;
};


template <typename T>
Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<T>& indices) noexcept {
	this->set_geometry(vertices, indices);
}


template <typename T>
void Mesh::set_geometry(const std::vector<Vertex>& vertices, const std::vector<T>& indices) {
	// add texuv attribute
	this->bundle.add_attribute(2, GL_FLOAT, false);
	// make vao
	this->bundle.make_vao(vertices, indices);
}
