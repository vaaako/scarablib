#pragma once

#include "scarablib/components/boundingbox.hpp"
#include "scarablib/components/materialcomponent.hpp"
#include "scarablib/components/physicscomponent.hpp"
#include "scarablib/opengl/shader.hpp"
#include "scarablib/opengl/vertexbuffercomponent.hpp"
#include "scarablib/typedef.hpp"
#include <GL/glew.h>

// Basic data for 3D and 2D shapes
class Mesh {
	public:
		// Color and texture of the mesh
		MaterialComponent material;

		// Bundle for VAO, VBO and EBO
		VertexBufferComponent bundle;
		// This is kinda wrong, because each instance of a Mesh will have copied bundle (but same VAO at least)

		// Bounding box
		BoundingBox bbox;
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
		// Build Mesh using a wavefront .obj file
		Mesh(const char* path);

		virtual ~Mesh() noexcept;

		// Build Mesh using vertices and indices
		template <typename T>
		void set_geometry(const std::vector<Vertex>& vertices, const std::vector<T>& indices);


		// Enables physics components for the mesh.
		// Both values are optional.
		// `Mass` (1.0f) is in kilograms.
		// `isstatic` (false) is whether the mesh is affected by gravity or not
		inline void enable_physics(const float mass = 1.0f, const bool isstatic = false) noexcept {
			this->physics = new PhysicsComponent();
			this->physics->mass = mass;
			this->physics->isstatic = isstatic;
		}

		// Updates the bounding box based on the transformations of the Model.
		// For dynamic transformations, use `set_dynamic_bbox_update(bool)`
		void update_bbox() {
			this->update_model_matrix(); // Needs updated matrix
			this->bbox.update_world_bounds(this->model);
		}

		// Sets whether the bounding box should be dynamically transformed
		void set_dynamic_bbox_update(const bool value) noexcept {
			this->dynamic_bounding = value;
		}

		virtual void update_model_matrix() noexcept = 0;

		// Returns nullptr, as the default shader from Scene2D is used.
		// Overridden by models using custom shaders
		virtual inline Shader* get_shader() const noexcept {
			return nullptr;
		}

	protected:
		// OpenGL
		GLsizei indices_length = 0; // For drawing (in 2D this is vertices size instead, but i dont know how to call it)
		GLenum indices_type    = 0; // For drawing (not used for 2D shapes)

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
	this->bbox = BoundingBox(vertices);
	this->indices_length = static_cast<GLsizei>(indices.size()),
	this->indices_type = (std::is_same_v<T, uint32>) ? GL_UNSIGNED_INT :
		(std::is_same_v<T, uint16>) ? GL_UNSIGNED_SHORT : GL_UNSIGNED_BYTE;

	this->bundle.make_vao(vertices, indices);
}
