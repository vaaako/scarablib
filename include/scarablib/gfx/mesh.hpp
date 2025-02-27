#pragma once

#include "scarablib/gfx/3d/boundingbox.hpp"
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
		Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32>& indices) noexcept;
		// Build Mesh using only vertices.
		// This is manly used for 2D Shapes. It will not make a collider
		Mesh(const std::vector<Vertex>& vertices) noexcept;
		// Build Mesh using a wavefront .obj file
		Mesh(const char* path);

		virtual ~Mesh() noexcept;

		// Set a new texture to the mesh
		inline void set_texture(Texture* texture) noexcept {
			this->texture = (texture != nullptr) ? texture : &Texture::default_texture();
		}

		inline Texture& get_texture() const noexcept {
			return *this->texture;
		}

		// Removes the shape's texture
		inline void remove_texture() noexcept {
			this->texture = &Texture::default_texture();
		}

		inline GLuint get_vaoid() const noexcept {
			return this->vao_id;
		}

		// This will create a collider for the mesh.
		// If your mesh is not static, you should also enable the calculation with the method `calc_collision(bool)`
		// WARNING: Call this method ONLY AFTER configuring the model
		virtual inline void make_collider() noexcept = 0;

	protected:
		// OpenGL
		size_t vao_hash;               // Keep track of the hash being used
		GLsizei indices_length;        // For drawing (in 2D this is not the indices size, but i dont know how to call it)
		uint32 vao_id;                 // This is used for wavefront .obj files only
		// std::vector<Vertex> vertices;  // Used for calculate the bounding box (cleared after that)

		// Bounding box
		// I wish this wasnt in this class, but i need vertices and dont want to store it, because is a waste of memory
		// This makes a little more "processor expensive" for creating a model, but, i wont have to store vertices for each model which consumes much memory
		BoundingBox::Size boxsize = {};

		// Current texture being used
		// This will always be a shared pointer to other texture
		Texture* texture = &Texture::default_texture(); // Default texture
};
