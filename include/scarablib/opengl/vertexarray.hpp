#pragma once

#include "scarablib/typedef.hpp"
#include "scarablib/utils/opengl.hpp"
#include <vector>

// Methods for making VAO, VBO and EBO (optional)
struct VertexArray {
	enum Buffers {
		VAO,
		VBO,
		EBO
	};

	// Pass empty indices if you don't want to make EBO
	template <typename T>
	VertexArray(const std::vector<T>& indices = {}) noexcept;
	~VertexArray() noexcept;

	// Activates the VAO buffer in the OpenGL context
	inline void bind_vao() const noexcept {
		glBindVertexArray(this->vao_id);
	}
	// Activates the VBO buffer in the OpenGL context
	inline void bind_vbo() const noexcept {
		glBindBuffer(GL_ARRAY_BUFFER, this->vbo_id);
	}
	// Activates the EBO buffer in the OpenGL context
	inline void bind_ebo() const noexcept {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo_id);
	}

	// Disables the VAO buffer in the OpenGL context
	void unbind_vao() const noexcept {
		glBindVertexArray(0);
	}
	// Disables the VBO buffer in the OpenGL context
	void unbind_vbo() const noexcept {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	// Disables the EBO buffer in the OpenGL context
	void unbind_ebo() const noexcept {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	// Disables all buffers
	void unbind_all() const noexcept;

	// Get id from the VAO buffer
	uint32 get_vaoid() const noexcept {
		return this->vao_id;
	}
	// Get id from the VBO buffer
	uint32 get_vboid() const noexcept {
		return this->vbo_id;
	}
	// Get id from the EBO buffer
	uint32 get_eboid() const noexcept {
		return this->ebo_id;
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
	// normalize only takes effect if type is some floating type
	void link_attrib(const uint32 index, const uint32 size, const GLenum type,
				const uint32 total_byte_size, const uint32 offset, const bool normalized) const;

	// Links a vertex attribute to the VBO, telling OpenGL how to interpret the vertex data.
	// - `T`: attribute type.
	// - `index`: The index of the vertex attribute in the shader (e.g., the 'location' in 'layout(location=index)').
	// - `size`: The number of components per attribute (e.g., 3 for a vec3, 2 for a vec2).
	// - `total_byte_size`: The total size (in bytes) of a single vertex.
	// - `offset`: The offset of this attribute within a vertex.
	// - `normalize`: (Default: false) Whether to normalize the data (e.g., true for [-1, 1] to [0, 1])
	template <typename T>
	void link_attrib(const uint32 index, const uint32 size,
			const uint32 total_byte_size, const uint32 offset, const bool normalized = false) const noexcept;

	private:
		uint32 vao_id;
		uint32 vbo_id;
		uint32 ebo_id = 0;
};

template <typename T>
VertexArray::VertexArray(const std::vector<T>& indices) noexcept {
	glGenVertexArrays(1, &this->vao_id);
	glGenBuffers(1, &this->vbo_id);

	if(!indices.empty()) {
		static_assert(std::is_unsigned_v<T>, "Only unsigned types for indices are accepted");
		glGenBuffers(1, &this->ebo_id);
		
		glBindVertexArray(this->vao_id); // Bind VAO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
					 static_cast<GLsizei>(indices.size() * sizeof(T)),
					 indices.data(),
					 GL_STATIC_DRAW);
		glBindVertexArray(0); // Unbind VAO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

template <typename T>
void VertexArray::link_attrib(const uint32 index, const uint32 size,
		const uint32 total_byte_size, const uint32 offset, const bool normalized) const noexcept {
	
	glBindVertexArray(this->vao_id);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo_id);

	glEnableVertexAttribArray(index);
	if constexpr (std::is_integral_v<T>) {
		glVertexAttribIPointer(index, static_cast<GLint>(size), ScarabOpenGL::gl_type<T>(),
				static_cast<GLsizei>(total_byte_size), reinterpret_cast<void*>(offset));
	} else {
		glVertexAttribPointer(index, static_cast<GLint>(size), ScarabOpenGL::gl_type<T>(), normalized ? GL_TRUE : GL_FALSE,
				static_cast<GLsizei>(total_byte_size), reinterpret_cast<void*>(offset));
	}

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
