#pragma once

#include <vector>
#include "scarablib/typedef.hpp"

// OpenGL Vertex Buffer Object object
class VBO {
	public:
		// Initializes a VBO and generates an OpenGL buffer ID.
		VBO(const uint32 id = 0) noexcept;
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

		inline uint32 get_id() const noexcept {
			return this->id;
		}

		// Allocates and initializes the VBO's data store.
		// - `size`: The total size (in bytes) of the data to allocate.
		// - `data`: A pointer to the data to copy into the buffer. If nullptr, the buffer is allocated but not initialized.
		// - `usage`: A GLenum specifying how the buffer's data will be used (e.g., GL_STATIC_DRAW, GL_DYNAMIC_DRAW)
		void alloc_data(const uint64 size, const void* data, const GLenum usage = GL_STATIC_DRAW) const noexcept;

		// Allocates and initializes the VBO's data store using a vector of floats.
		// - `data`: The vector of float data to copy into the buffer.
		// - `usage`: A GLenum specifying how the buffer's data will be used
		void alloc_data(const std::vector<float>& data, const GLenum usage) const noexcept;

		// Allocate and initialize the data store for the VBO using a vector of Vertex
		// void alloc_data(const std::vector<Vertex>& data, const GLenum drawtype = GL_STATIC_DRAW) noexcept;

		// Links a vertex attribute to the VBO, telling OpenGL how to interpret the vertex data.
		// - `index`: The index of the vertex attribute in the shader (e.g., the 'location' in 'layout(location=index)').
		// - `count`: The number of components per attribute (e.g., 3 for a vec3, 2 for a vec2).
		// - `total_byte_size`: The total size (in bytes) of a single vertex.
		// - `offset`: The offset (in bytes) of this attribute within a vertex.
		// - `type`: (Default: GL_FLOAT) Type of data (e.g., GL_FLOAT).
		// - `normalize`: (Default: false) Whether to normalize the data (e.g., true for [-1, 1] to [0, 1])
		void link_attrib(const uint32 index, const uint32 count,
				const uint32 total_byte_size, const uint32 offset,
				const GLenum type = GL_FLOAT, const bool normalized = false) const noexcept;

	private:
		uint32 id;
};
