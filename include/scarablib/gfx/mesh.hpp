#pragma once

#include "scarablib/gfx/3d/boundingbox.hpp"
#include "scarablib/opengl/bufferbundle.hpp"
#include "scarablib/opengl/shader.hpp"
#include "scarablib/typedef.hpp"
#include "scarablib/types/texture.hpp"
#include "scarablib/types/vertex.hpp"
#include <GL/glew.h>
#include <vector>

// Basic data for 3D and 2D shapes
class Mesh {
	public:
		// Build Mesh using vertices and indices
		template <typename T>
		Mesh(const std::vector<Vertex>& vertices, const std::vector<T>& indices) noexcept;
		// Build Mesh using only vertices.
		// Mainly used for 2D Shapes. It will not make a collider
		Mesh(const std::vector<Vertex>& vertices) noexcept;
		// Build Mesh using a wavefront .obj file
		Mesh(const char* path);

		virtual ~Mesh() noexcept = default;

		// Returns a reference to the bounding box
		inline const BoundingBox& get_bounding_box() const noexcept {
			return this->bbox;
		}

		// Returns a reference to the BufferBundle
		inline BufferBundle& get_bundle() noexcept {
			return this->bundle;
		}

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

	protected:
		// OpenGL
		BufferBundle bundle;    // Bundle for VAO, VBO and EBO
								// This is kinda wrong, because each instance of a Mesh will have copied bundle (but same VAO at least)
		GLsizei indices_length; // For drawing (in 2D this is vertices size instead, but i dont know how to call it)
		GLenum indices_type;    // For drawing (not used for 2D shapes)

		// Bounding box
		// I wish this was in Model class, but i don't want vertices and i need it to build a bounding box
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
