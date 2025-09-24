#pragma once

#include "scarablib/opengl/vaomanager.hpp"
#include "scarablib/opengl/vertexarray.hpp"
#include "scarablib/typedef.hpp"
#include <type_traits>

// A class that bundles the VAO, VBO and EBO buffers.
// Also stores information necessary for rendering like indices size and indices type.
// With utilitary methods for creating the Vertex Buffer
class VertexBufferComponent {
	public:
		// Must be initialized with some constructor
		std::shared_ptr<VertexArray> vertexarray;

		VertexBufferComponent() noexcept = default;
		~VertexBufferComponent() noexcept;

		// Returns unique hash used in VAOManager to get this VAO
		inline constexpr size_t get_hash() const noexcept {
			return this->hash;
		}

		// Returns the length of this VAO.
		// This is used for rendering
		// For 3D shapes this is the length of indices.
		// For 2D shapes this is the length of vertices.
		inline constexpr int get_length() const noexcept {
			return this->length;
		}

		// Returns the type of indices.
		// This is used for 3D rendering
		inline constexpr GLenum get_indices_type() const noexcept {
			return this->indices_type;
		}

		// Configurates additional attributes for data.
		// This will only be used if data is a vector of Vertex (or child of).
		// - `count`: The number of components per attribute (e.g., 3 for a vec3, 2 for a vec2).
		// - `normalized`: (Default: false) Whether to normalize the data (e.g., true for [-1, 1] to [0, 1])
		// void add_attribute(const uint32 count, const GLenum type = GL_FLOAT, const bool normalized = false);
		template <typename T>
		void add_attribute(const uint32 count, const bool normalized = false) noexcept;
		// - `type`: (Default: GL_FLOAT) Type of data (e.g., GL_FLOAT).

		// Automatically computes Vertex Buffer and stores in VAOManager
		template <typename T>
		void make_vao(const std::vector<Vertex>& vertices, const std::vector<T>& indices = {});

	private:
		std::vector<VAOManager::VertexAttribute> attributes;

		// Current offset of attributes
		size_t stride = sizeof(vec3<float>); // position is always presented
		size_t hash;
		// For rendering
		// Either vertices (if vertices only) size or indices size
		int length = 0;
		// For rendering
		// not used for 2D shapes
		GLenum indices_type = GL_UNSIGNED_INT;

		inline void cleanup_attributes() noexcept {
			// Clear attributes
			this->attributes.clear();
			this->attributes.shrink_to_fit(); // Actually releases the allocated memory
			this->stride = sizeof(vec3<float>);
		}
};

template <typename T>
void VertexBufferComponent::add_attribute(const uint32 count, const bool normalized) noexcept {
	this->attributes.push_back(VAOManager::VertexAttribute {
		.component_count = count,
		.type            = ScarabOpenGL::gl_type<T>(),
		.normalized      = std::is_integral_v<T> ? false : normalized,
		.offset          = this->stride
	});
	this->stride += count * sizeof(T);
}

// TODO: Support to vec3<float> again
template <typename T>
void VertexBufferComponent::make_vao(const std::vector<Vertex>& vertices, const std::vector<T>& indices) {
	// Get the manager once
	VAOManager& manager = VAOManager::get_instance();

	// The component's only job is to ask the manager for a VAO.
	// It passes the data and the attribute layout it requires.
	std::pair<size_t, std::shared_ptr<VertexArray>> entry
		= manager.acquire_vao(vertices, indices, this->attributes);

	// Store the results
	this->hash = entry.first;
	this->vertexarray = entry.second;

	// Set draw call parameters
	if(!indices.empty()) {
		this->length = indices.size();
		this->indices_type = ScarabOpenGL::gl_type<T>();
	} else {
		this->length = vertices.size();
	}

	// The component no longer owns a VertexArray object.
	// It just holds the IDs and the hash.
	// This avoids raw pointers and potential memory leaks.
	this->cleanup_attributes(); // You can now free the CPU-side attribute definitions
}
