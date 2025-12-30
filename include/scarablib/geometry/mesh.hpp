#pragma once

#include <memory>
#include "scarablib/components/boundingbox.hpp"
#include "scarablib/components/materialcomponent.hpp"
#include "scarablib/components/physicscomponent.hpp"
#include "scarablib/opengl/resourcesmanager.hpp"
#include "scarablib/opengl/vertexarray.hpp"

// Basic data for 3D and 2D shapes
class Mesh {
	public:
		// Material of this mesh
		std::shared_ptr<MaterialComponent> material = std::make_shared<MaterialComponent>();
		// Since material can be shared i need to be a pointer so a double delete is not done

		// Bundle for VAO, VBO and EBO
		std::shared_ptr<VertexArray> vertexarray;
		// This is kinda wrong, because each instance of a Mesh will have copied bundle (but same VAO at least)

		// Bounding box
		BoundingBox* bbox = nullptr;
		// I wish this was in Model class, but i don't want to store vertices
		// and i need it to build a bounding box

		// Physics component
		PhysicsComponent* physics = nullptr;

		// Mesh is not build, you should provide vertices and indices with `Mesh::set_geometry(...)` method
		Mesh() noexcept = default;
		// Build Mesh using vertices and indices
		template <typename T, typename U>
		Mesh(const std::vector<T>& vertices, const std::vector<U>& indices) noexcept;
		// Build Mesh using only vertices.
		// Mainly used for 2D Shapes. It will not make a collider
		template <typename T>
		Mesh(const std::vector<T>& vertices) noexcept;

		virtual ~Mesh() noexcept;

		// This method does not draw the Mesh to the screen.
		// As it does not bind the VAO, Shader and Texture (batch rendering)
		virtual void draw_logic(const Camera& camera) noexcept = 0;

		// Build Mesh using vertices and indices
		template <typename T, typename U>
		void set_geometry(const std::vector<T>& vertices, const std::vector<U>& indices);

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

	protected:
		// Matrix
		bool isdirty; // Calculate matrix if anything changed
		glm::mat4 model = glm::mat4(1.0f);

		bool dynamic_bounding = false;
};


template <typename T, typename U>
Mesh::Mesh(const std::vector<T>& vertices, const std::vector<U>& indices) noexcept {
	this->set_geometry(vertices, indices);
}

// Indices attributes are not needed to set here
// I will not make Bounding Box here because this is probably a 2D model
template <typename T>
Mesh::Mesh(const std::vector<T>& vertices) noexcept {
	this->vertexarray = ResourcesManager::get_instance()
		.acquire_vertexarray(vertices, std::vector<uint8>{});
}


template <typename T, typename U>
void Mesh::set_geometry(const std::vector<T>& vertices, const std::vector<U>& indices) {
	this->vertexarray = ResourcesManager::get_instance()
		.acquire_vertexarray(vertices, indices);
}
