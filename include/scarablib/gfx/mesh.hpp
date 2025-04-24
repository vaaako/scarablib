#pragma once

#include "scarablib/gfx/3d/boundingbox.hpp"
#include "scarablib/opengl/bufferbundle.hpp"
#include "scarablib/typedef.hpp"
#include "scarablib/types/texture.hpp"
#include "scarablib/types/vertex.hpp"
#include <GL/glew.h>
#include <vector>

// This is a class used to store the basic data for a 3D object
class Mesh {
	public:
		// Build Mesh using object's vertices and indices
		// This is used for custom models
		template <typename T>
		Mesh(const std::vector<Vertex>& vertices, const std::vector<T>& indices) noexcept;
		// Build Mesh using only vertices.
		// This is manly used for 2D Shapes. It will not make a collider
		Mesh(const std::vector<Vertex>& vertices) noexcept;
		// Build Mesh using a wavefront .obj file
		Mesh(const char* path);

		virtual ~Mesh() noexcept = default;

		// Set a new texture to the mesh
		inline void set_texture(Texture* texture) noexcept {
			this->texture = (texture != nullptr) ? texture : &Texture::default_texture();
		}

		// Get texture currently in use
		inline Texture& get_texture() const noexcept {
			return *this->texture;
		}

		// Removes the shape's texture
		inline void remove_texture() noexcept {
			this->texture = &Texture::default_texture();
		}

		// Get VAO bundle
		inline BufferBundle& get_bundle() noexcept {
			return this->bundle;
		}

	protected:
		// OpenGL
		BufferBundle bundle;    // Bundle of VAO, VBO and EBO
		GLsizei indices_length; // For drawing (in 2D this is not the indices size, but i dont know how to call it)
		GLenum indices_type;    // For drawing (not used for 2D shapes)

		// Bounding box
		// I wish this wasnt in this class, but i need vertices and dont want to store it, because is a waste of memory
		// This makes a little more "processor expensive" for creating a model, but, i wont have to store vertices for each model which consumes much memory
		BoundingBox bbox;

		// Current texture being used
		// This will always be a shared pointer to other texture
		Texture* texture = &Texture::default_texture(); // Default texture
};


template <typename T>
Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<T>& indices) noexcept
	: indices_length(static_cast<GLsizei>(indices.size())), indices_type(
		(std::is_same_v<T, uint32>) ? GL_UNSIGNED_INT :
		(std::is_same_v<T, uint16>) ? GL_UNSIGNED_SHORT : GL_UNSIGNED_BYTE),
	  bbox(vertices) {

	this->bundle.make_vao_with_manager(vertices, indices);
}
