#pragma once

#include "scarablib/geometry/vertex.hpp"
#include "scarablib/typedef.hpp"
#include "scarablib/utils/opengl.hpp"
#include <type_traits>
#include <vector>

// Methods for making VAO, VBO and EBO (optional)
class VertexArray {
	friend class ResourcesManager;
	public:
		enum Buffers {
			VAO,
			VBO,
			EBO
		};

		// Pass empty indices if you don't want to make EBO
		template <typename T, typename U>
		VertexArray(const std::vector<T>& vertices, const std::vector<U>& indices = {}, const bool dynamic = false) noexcept;
		// Manually creates a Vertex Array
		VertexArray(const void* data, const size_t capacity, const size_t vertex_size, const bool dynamic = false) noexcept;
		~VertexArray() noexcept;

		// Delete copy
		VertexArray(const VertexArray&) = delete;
		VertexArray& operator=(const VertexArray&) = delete;
		// Delete move
		VertexArray(const VertexArray&&) = delete;
		VertexArray& operator=(const VertexArray&&) = delete;

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
		inline void unbind_vao() const noexcept {
			glBindVertexArray(0);
		}
		// Disables the VBO buffer in the OpenGL context
		inline void unbind_vbo() const noexcept {
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		// Disables the EBO buffer in the OpenGL context
		inline void unbind_ebo() const noexcept {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		// Disables all buffers
		void unbind_all() const noexcept;

		// Get id from the VAO buffer
		inline uint32 get_vaoid() const noexcept {
			return this->vao_id;
		}
		// Get id from the VBO buffer
		inline uint32 get_vboid() const noexcept {
			return this->vbo_id;
		}
		// Get id from the EBO buffer
		inline uint32 get_eboid() const noexcept {
			return this->ebo_id;
		}

		// Length of indices or length of vertices, if not using indices
		inline uint32 get_length() const noexcept {
			return this->length;
		}

		// The type of indices as GLenum
		inline GLenum get_indices_type() const noexcept {
			return this->indices_type;
		}

		// The size of the Vertices type.
		// sizeof(T)
		inline uint32 get_vertex_size() const noexcept {
			return this->vsize;
		}

		// The size of the Indices type.
		// sizeof(U)
		inline uint32 get_index_stride() const noexcept {
			return this->indexstride;
		}

		// Calculates the Indices offset using a base_index
		inline void* index_offset(const uint32 base_index) const {
			return (void*)(uintptr_t)(base_index * this->indexstride);
		}

		// Manually creates a Vertex Array
		// - `data`: Vertex Array data.
		// - `capacity`: Vertex Array total capacity.
		// - `hash`: The hash identification.
		// - `dynamic`: (Default: false) Set the Vertex Array to be changeable after creation.
		// Usage example: `VertexArray::alloc_data(nullptr, 6 * this->buffer_capacity, true)`
		void alloc_data(const void* data, const size_t capacity, const bool dynamic = false) const noexcept;

		// Allocates and initializes the VBO's data store.
		// - `data`: The vector to initialize the VBO. Usually the vertices
		template <typename T>
		inline void alloc_data(const std::vector<T>& data, const bool dynamic = false) const noexcept {
			this->alloc_data(data.data(), data.size(), dynamic);
		}

		// Allocate and initialize the data store for the VBO using a vector of Vertex
		// void alloc_data(const std::vector<Vertex>& data, const GLenum drawtype = GL_STATIC_DRAW) noexcept;

		// Automatically deals with offset and other values when adding new attributes to the VBO.
		// You can use this method instead of `link_attrib`.
		// but you still need to alloc data with the `alloc_data` method before the usage of this method.
		// - `count`: The number of components per attribute (e.g., 3 for a vec3, 2 for a vec2).
		// - `normalize`: (Default: false) Whether to normalize the data (e.g., true for [-1, 1] to [0, 1])
		// normalize only takes effect if type is some floating type
		template <typename T>
		void add_attribute(const uint32 count, const bool normalized) noexcept;

		// Links a vertex attribute to the VBO, telling OpenGL how to interpret the vertex data.
		// - `T`: attribute type.
		// - `index`: The index of the vertex attribute in the shader (e.g., the 'location' in 'layout(location=index)').
		// - `count`: The number of components per attribute (e.g., 3 for a vec3, 2 for a vec2).
		// - `stride`: The total size (in bytes) of a single vertex.
		// - `offset`: The offset of this attribute within a vertex.
		// - `normalize`: (Default: false) Whether to normalize the data (e.g., true for [-1, 1] to [0, 1])
		template <typename T>
		void link_attrib(const uint32 index, const uint32 count,
				const uint32 stride, const uint32 offset, const bool normalized = false) const noexcept;

		// Update the data inside the VertexArray using Sub Data.
		// VertexArray must have be created with `dynamic` set to true
		void update_data(const void* data, size_t size) noexcept;

	private:
		size_t hash = 0; // Only VertexManager changes this value
		// Used for adding attributes to VBO
		size_t stride = 0;

		GLuint vao_id;
		GLuint vbo_id;
		GLuint ebo_id = 0;

		// Either vertices size (if vertices only) or indices size
		int length;
		// Not used for 2D shapes
		GLenum indices_type = GL_UNSIGNED_INT;
		uint32 index  = 0;
		// Vertex type size
		uint32 vsize;
		// Indice type size
		uint32 indexstride = 0;
};

template <typename T, typename U>
VertexArray::VertexArray(const std::vector<T>& vertices, const std::vector<U>& indices, const bool dynamic) noexcept
	: length(vertices.size()), vsize(sizeof(T)), indexstride(sizeof(U)) {

	static_assert(std::is_base_of_v<Vertex, T> || std::is_base_of_v<Vertex2D, T>, "T must derive from Vertex or Vertex2D");
	static_assert(std::is_unsigned_v<U>, "U must be an unsigned integer type");

#if !defined(BUILD_OPGL30)
	glCreateVertexArrays(1, &this->vao_id);
	glCreateBuffers(1, &this->vbo_id);
	// Alloc data on VBO
	this->alloc_data(vertices, dynamic);

	if(!indices.empty()) {
		glCreateBuffers(1, &this->ebo_id);
		glNamedBufferStorage(this->ebo_id, static_cast<GLsizeiptr>(indices.size() * sizeof(U)),
			indices.data(), 0);

		// Attach EBO to VAO
		glVertexArrayElementBuffer(this->vao_id, this->ebo_id);
		this->length = indices.size();
		this->indices_type = ScarabOpenGL::gl_type<U>();
	}
#else
	glGenVertexArrays(1, &this->vao_id);
	glGenBuffers(1, &this->vbo_id);
	// Alloc data on VBO
	this->alloc_data(vertices);

	if(!indices.empty()) {
		glGenBuffers(1, &this->ebo_id);
		glBindVertexArray(this->vao_id); // Bind VAO

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			static_cast<GLsizei>(indices.size() * sizeof(U)),
			indices.data(),
			GL_STATIC_DRAW
		);

		glBindVertexArray(0); // Unbind VAO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		this->length = indices.size();
		this->indices_type = ScarabOpenGL::gl_type<U>();
	}
#endif

	GL_CHECK();
}


template <typename T>
void VertexArray::link_attrib(const uint32 index, const uint32 count,
		const uint32 stride, const uint32 offset, const bool normalized) const noexcept {

#if !defined(BUILD_OPGL30)
	// Initialize
	glVertexArrayVertexBuffer(this->vao_id, index, this->vbo_id, 0, stride);
	glEnableVertexArrayAttrib(this->vao_id, index); // Enable index
	if constexpr (std::is_integral_v<T>) {
		glVertexArrayAttribIFormat(this->vao_id, index, static_cast<GLint>(count),
			ScarabOpenGL::gl_type<T>(), static_cast<GLuint>(offset)
		);
	} else {
		glVertexArrayAttribFormat(this->vao_id, index, static_cast<GLint>(count),
			ScarabOpenGL::gl_type<T>(), normalized, static_cast<GLuint>(offset)
		);
	}
	// Attach VBO to VAO
	glVertexArrayAttribBinding(this->vao_id, index, index);

#else
	glBindVertexArray(this->vao_id);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo_id);

	glEnableVertexAttribArray(index);
	if constexpr (std::is_integral_v<T>) {
		glVertexAttribIPointer(index, static_cast<GLint>(count), ScarabOpenGL::gl_type<T>(),
				static_cast<GLsizei>(stride), reinterpret_cast<void*>(offset));
	} else {
		glVertexAttribPointer(index, static_cast<GLint>(count), ScarabOpenGL::gl_type<T>(), normalized,
				static_cast<GLsizei>(stride), reinterpret_cast<void*>(offset));
	}

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif
}

template <typename T>
void VertexArray::add_attribute(const uint32 count, const bool normalized) noexcept {
	static_assert(std::is_arithmetic_v<T>, "Type must be an arithmetic value");
	this->link_attrib<T>(this->index++, count, this->vsize, this->stride, normalized);
	this->stride += count * sizeof(T);
}
