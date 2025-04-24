#pragma once

#include <GL/glew.h>
#include "scarablib/typedef.hpp"
#include "scarablib/types/vertex.hpp"

// OpenGL Vertex Buffer Object object
class VBO {
	public:
		// Initializes a VBO and generates an OpenGL buffer ID.
		VBO() noexcept;
		~VBO() noexcept;

		// Disable copy and moving
		VBO(const VBO&) noexcept = delete;
		VBO& operator=(const VBO&) noexcept = delete;
		VBO(VBO&&) noexcept = delete;
		VBO& operator=(VBO&&) noexcept = delete;

		// Activates the VBO in the OpenGL context
		inline void bind() const noexcept {
			glBindBuffer(GL_ARRAY_BUFFER, this->id);
		}

		// Deactivates the VBO in the OpenGL context
		inline void unbind() const noexcept {
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		// Allocates and initializes the VBO's data store.
		// - `size`: The total size (in bytes) of the data to allocate.
		// - `data`: A pointer to the data to copy into the buffer. If nullptr, the buffer is allocated but not initialized.
		// - `usage`: A GLenum specifying how the buffer's data will be used (e.g., GL_STATIC_DRAW, GL_DYNAMIC_DRAW)
		void alloc_data(const uint64 size, const void* data, const GLenum usage) const noexcept;

		// Allocates and initializes the VBO's data store using a vector of floats.
		// - `data`: The vector of float data to copy into the buffer.
		// - `usage`: A GLenum specifying how the buffer's data will be used
		void alloc_data(const std::vector<float>& data, const GLenum usage) const noexcept;

		// Allocate and initialize the data store for the VBO using a vector of Vertex
		// void alloc_data(const std::vector<Vertex>& data, const GLenum drawtype = GL_STATIC_DRAW) noexcept;

		// Links a vertex attribute to the VBO, telling OpenGL how to interpret the vertex data.
		// - `index`: The index of the vertex attribute in the shader (e.g., the 'location' in 'layout(location=index)').
		// - `size`: The number of components per attribute (e.g., 3 for a vec3, 2 for a vec2).
		// - `total_byte_size`: The total size (in bytes) of a single vertex.
		// - `offset`: The offset (in bytes) of this attribute within a vertex
		void link_attrib(const uint32 index, const uint32 size, const uint32 total_byte_size, const uint32 offset) const noexcept;

		// Automatically creates the VBO from a vector of Vertex structs.
		// - `data`:  The vector of Vertex data.
		// - `size`: The size of the data.
		// - `usage`: The usage type
		void make_from_vertex(const std::vector<Vertex>& data, const uint32 size, const GLenum usage) const noexcept;

		// Automatically creates the VBO from a vector of vertex coordinates (floats).
		// - `data`: The vector of vertex coordinates.
		// - `size`: The size of the data.
		// - `usage`: The usage type
		void make_from_vertices(const std::vector<float>& data, const uint32 size, const GLenum usatge) const noexcept;

		// Updates the data stored in the VBO.
		// - `size`: The size (in bytes) of the data to update.
		// - `data`: A pointer to the new data.
		// - `usage`: A GLenum specifying how the buffer's data will be used
		void update_data(const uint64 size, const void* data, const GLenum usage) const noexcept;

		// Automatically make the VBO from a vector of 2D Vertex.
		// Don't use this with `alloc_data` or `link_attrib`
		// void make_from_vertex2d(const std::vector<Vertex2D>& data) noexcept;

		// Generates a VBO attribute using index, size and the data itself.
		// This is used when each coordinate is splited in different arrays.
		// Don't use this with `alloc_data` or `link_attrib`
		// void store_data(const uint32 index, const uint32 size, void* data) noexcept;
	private:
		GLuint id;
};
