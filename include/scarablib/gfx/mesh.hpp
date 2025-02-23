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
		inline void set_texture(Texture* texture) noexcept {
			this->texture = (texture != nullptr) ? texture : &this->get_deftex();
		}

		inline Texture& get_texture() const noexcept {
			return *this->texture;
		}

		// Removes the shape's texture
		inline void remove_texture() noexcept {
			this->texture = &this->get_deftex();
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
		size_t indices_length;         // For drawing
		uint32 vao_id;                 // This is used for wavefront .obj files only
		std::vector<Vertex> vertices;  // Used for calculate the bounding box (cleared after that)

		// Current texture being used
		// This will always be a shared pointer to other texture
		Texture* texture = &this->get_deftex(); // Default texture

		// NOTE: Dont want this here, but i was forced since it needs vertices
		// and i dont want to store vertices just for this

		// NOTE: I wish this was a member, but i would have to initialize in window.hpp
		// because i cant make a texture if the opengl context isnt ready yet
		inline Texture& get_deftex() const noexcept {
			// I don't like data being statically allocated but whatever
			static Texture def_tex = Texture(Colors::WHITE); // Make solid white texture
			return def_tex;
		}
};
