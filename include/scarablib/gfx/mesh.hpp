#pragma once

#include "scarablib/opengl/vao.hpp"
#include "scarablib/typedef.hpp"
#include "scarablib/types/texture.hpp"
#include "scarablib/types/vertex.hpp"
#include <GL/glew.h>
#include <vector>

// This is a class used to store the basic data for a 3D object
class Mesh {
	public:
		// Build Mesh using a wavefront .obj file
		Mesh(const char* path);
		// Build Mesh using object's vertices and indices
		Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32>& indices);
		// Build Mesh using an existing VAO, then build VBO and EBO using pre-defined vertics and indices (generally used for custom models)
		Mesh(const GLuint& vao_id, const std::vector<Vertex>& vertices, const std::vector<uint32>& indices);
		~Mesh();

		// Set a new texture to the mesh
		void set_texture(Texture* texture);
		// std::vector<Texture&> textures; // For when multiple textures are supported

		// Removes the shape's texture
		inline void remove_texture() {
			this->texture = &this->get_deftex();
		}

		bool operator==(const Mesh& other) const noexcept {
			return this->vao->get_id() == other.vao->get_id();
		}

		// Generate VAO, VBO and EBO.
		// Use this just if you used the blank constructor
		void make_buffers(const std::vector<Vertex>& vertices, const std::vector<uint32>& indices);

		// Generate VAO, VBO and EBO to a existing VAO.
		// This is used for custom models
		void make_buffers(const GLuint& vao_id, const std::vector<Vertex>& vertices, const std::vector<uint32>& indices);

		// Get current Model VAO.
		// If you are using this in a struct of a custom model, make it return a static VAO (like in cube.hpp).
		// If not, it will return by default the VAO of Mesh class (used when loading external obj).
		// This system exist to avoid making multiple VAOs of a single model
		virtual inline const VAO& get_vao() const {
			return *this->vao;
		}

	protected:
		// Texture will always be a "reference" to another existing texture
		Texture* texture = &this->get_deftex(); // Current texture being used

		// OpenGL
		std::vector<Vertex> vertices; // This is only used for calculating size on Model
		VAO* vao = new VAO();
		GLuint vbo_id;
		GLuint ebo_id;
		size_t indices_length;

		// Default texture (solid white)
		inline Texture& get_deftex() const {
			// I don't like data being statically allocated but whatever
			static Texture def_tex = Texture(); // Make solid white texture
			return def_tex;
		}
};
