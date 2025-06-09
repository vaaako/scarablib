#pragma once

#include "scarablib/physics/boundingbox.hpp"
#include "scarablib/opengl/bufferbundle.hpp"
#include "scarablib/opengl/shader.hpp"
#include "scarablib/physics/physicscomponent.hpp"
#include "scarablib/typedef.hpp"
#include "scarablib/types/texture.hpp"
#include "scarablib/types/vertex.hpp"
#include <GL/glew.h>
#include <vector>

// Basic data for 3D and 2D shapes
class Mesh {
	public:
		// Bundle for VAO, VBO and EBO
		BufferBundle bundle;
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

		template <typename T>
		void set_geometry(const std::vector<Vertex>& vertices, const std::vector<T>& indices);

		// Gets a reference to the current texture
		inline Texture& get_texture() const noexcept {
			return *this->texture;
		}

		// Sets a new texture.
		// If `texture` is nullptr, the default texture will be used instead
		inline void set_texture(Texture* texture) noexcept {
			this->texture = (texture != nullptr) ? texture : &Texture::default_texture();
		}

		// Removes the mesh's texture, reverting to the default texture.
		// The default texture is a solid white texture
		inline void remove_texture() noexcept {
			this->texture = &Texture::default_texture();
		}

		// Returns nullptr, as the default shader from Scene2D is used.
		// Overridden by models using custom shaders.
		// Scene2D checks for nullptr, if so, uses default shader, otherwise, uses this method's shader.
		virtual inline Shader* get_shader() const noexcept {
			return nullptr;
		}


		// Enables physics components for the mesh.
		// Both values are optional.
		// `Mass` (1.0f) is in kilograms.
		// `isstatic` (false) is whether the mesh is affected by gravity or not
		inline void enable_physics(const float mass = 1.0f, const bool isstatic = false) noexcept {
			this->physics = new PhysicsComponent();
			this->physics->mass = mass;
			this->physics->isstatic = isstatic;
		}

		// Applies the model's transformations (the moment is called) to the bounding box.
		// For dynamic transformations, use `set_dynamic_transform(bool)`
		void set_transform() {
			this->update_model_matrix(); // Needs updated matrix
			this->bbox.update_world_bounds(this->model);
		}

		// Sets whether the bounding box should be dynamically recalculated.
		// This will transform the bounding box with the model matrix
		void set_dynamic_transform(const bool value) noexcept {
			this->dynamic_bounding = value;
			this->set_transform(); // Just in case
		}

		virtual void update_model_matrix() noexcept = 0;

	protected:
		// OpenGL
		GLsizei indices_length = 0; // For drawing (in 2D this is vertices size instead, but i dont know how to call it)
		GLenum indices_type    = 0; // For drawing (not used for 2D shapes)

		// Current texture being used
		// This will always be a shared pointer to other texture
		Texture* texture = &Texture::default_texture(); // Default texture

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

	this->bundle.make_vao_with_manager(vertices, indices);
}
