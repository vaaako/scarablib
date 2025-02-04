#pragma once

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
		// Build Mesh using a wavefront .obj file
		Mesh(const char* path);

		virtual ~Mesh() noexcept;

		// Set a new texture to the mesh
		void set_texture(Texture* texture) noexcept;

		// Removes the shape's texture
		inline void remove_texture() noexcept {
			this->texture = &this->get_deftex();
		}

		inline GLuint get_vaoid() const noexcept {
			return this->vao_id;
		}

	protected:
		// Texture will always be a "reference" to another existing texture
		Texture* texture = &this->get_deftex(); // Current texture being used

		// OpenGL
		size_t vao_hash;              // Keep track of the hash being used
		size_t indices_length;
		uint32 vao_id;                // This is used for wavefront .obj files only
		std::vector<Vertex> vertices; // This is only used for calculating size on Model

		// Default texture (solid white)
		inline Texture& get_deftex() const noexcept {
			// I don't like data being statically allocated but whatever
			static Texture def_tex = Texture(); // Make solid white texture
			return def_tex;
		}
};
