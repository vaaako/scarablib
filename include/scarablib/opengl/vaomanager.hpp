#pragma once

#include "scarablib/geometry/vertex.hpp"
#include "scarablib/opengl/vertexarray.hpp"
#include "scarablib/proper/error.hpp"
#include "scarablib/proper/log.hpp"
#include <memory>
#include <unordered_map>

// This is a class that handles the creation and management of unique Vertex Array Objects (VAO).
// This class prevents duplicate VAOs from being created.
//
// Previously I was keeping track of how many different meshes were using the same VAO.
// if this count dropped to zero, then i would delete that VAO.
// But now that i implemented VertexArray, with shared_ptr this is not necessary anymore
class VAOManager {
	public:
		struct VertexAttribute {
			// How many components per attribute.
			// Example: 2 for a vec2
			const uint32 component_count;
			// Type of data
			// Example: GL_FLOAT
			const GLenum type;
			// Whether to normalize the data
			const bool normalized;
			// Offset of the attribute
			const size_t offset;
		};

		// Get the singleton instance of the VAOManager reference
		static VAOManager& get_instance() {
			static VAOManager instance;
			return instance;
		}

		// Computes a unique hash based on the provided vertices and indices.
		// This hash should be created and stored before making the VAO. So it's possible to delete it.
		// - `vertices`: The vertex data.
		// - `indices`: (Optional) The index data.
		// Returns: The entry for this VAO
		template <typename T>
		std::pair<size_t, std::shared_ptr<VertexArray>> acquire_vao(
			const std::vector<Vertex>& vertices, const std::vector<T>& indices,
			const std::vector<VertexAttribute>& attributes);

		// Returns an entry of a VAO using its hash.
		// Returns nullptr if not found
		std::shared_ptr<VertexArray> get_vao_entry(const size_t hash) const;

		// Releases a VAO from the manager, using its hash.
		// Does nothing if not found
		void release_vao(const size_t hash) noexcept;

		// Cleans up all VAOs.
		// WARNING: This is called inside Window destructor.
		// DO NOT call it manually
		void cleanup() noexcept;

	private:
		std::unordered_map<size_t, std::shared_ptr<VertexArray>> vao_map;

		template<typename T>
		size_t compute_hash(const std::vector<Vertex>& vertices, const std::vector<T>& indices = {}) const noexcept;
};

template <typename T>
std::pair<size_t, std::shared_ptr<VertexArray>> VAOManager::acquire_vao(
	const std::vector<Vertex>& vertices, const std::vector<T>& indices,
	const std::vector<VertexAttribute>& attributes) {

	static_assert(std::is_unsigned_v<T>, "Only unsigned types for indices are accepted");
	if(vertices.empty()) {
		throw ScarabError("Vertices vector is empty for VAO creation");
	}

	// -- COMPUTE HASH
	const size_t hash = this->compute_hash(vertices, indices);

	// -- CHECK IF CACHED
	std::shared_ptr<VertexArray> vertexarray = this->get_vao_entry(hash);
	if(vertexarray != nullptr) {
		#ifdef SCARAB_DEBUG_VAO_MANAGER
		LOG_DEBUG("Hash %zu found! Reusing VAO.", hash);
		#endif
		// Return the existing entry
		return {
			hash,
			vertexarray
		};
	}

	#ifdef SCARAB_DEBUG_VAO_MANAGER
	LOG_DEBUG("Hash %zu not found. Creating new VAO.", hash);
	#endif

	// Make buffers
	if(!indices.empty()) {
		vertexarray = std::make_shared<VertexArray>(indices);
	} else {
		vertexarray = std::make_shared<VertexArray>(std::vector<T>{});
	}

	#ifdef SCARAB_DEBUG_VAO_MANAGER
	LOG_DEBUG("VAO ID made: %zu", vertexarray->get_vaoid());
	#endif

	// Alloc data
	vertexarray->alloc_data(vertices.size() * sizeof(Vertex), vertices.data());
	// Put position attribute
	vertexarray->link_attrib<float>(0, 3, sizeof(Vertex), 0);

	// Add all other attributes
	for(size_t i = 0; i < attributes.size(); i++) {
		const VertexAttribute attrib = attributes[i];
		// +1 to jump position which is always added and already added
		vertexarray->link_attrib(i + 1, attrib.component_count,
				attrib.type, sizeof(Vertex), attrib.offset, attrib.normalized);

		LOG_DEBUG("adding attribute");
	}
	GL_CHECK();

	this->vao_map[hash] = vertexarray;
	return {
		hash,
		vertexarray
	};
}

// Specialized hash function for better distribution

// TODO: Use ScarabMath::hash_combine
template <typename T>
size_t VAOManager::compute_hash(const std::vector<Vertex>& vertices, const std::vector<T>& indices) const noexcept {
	static_assert(std::is_unsigned_v<T>, "Only unsigned types for indices are accepted");

	// Use FNV-1a hash for better distribution
	constexpr size_t FNV_PRIME = 1099511628211ULL;
	constexpr size_t FNV_OFFSET = 14695981039346656037ULL;

	size_t hash = FNV_OFFSET;

	// Hash vertex data
	const char* data = reinterpret_cast<const char*>(vertices.data());
	const size_t vertex_bytes = vertices.size() * sizeof(Vertex);

	for(size_t i = 0; i < vertex_bytes; i++) {
		hash ^= static_cast<size_t>(data[i]);
		hash *= FNV_PRIME;
	}

	// Hash index data (only if indices are provided)
	if(!indices.empty()) {
		const char* index_data = reinterpret_cast<const char*>(indices.data());
		const size_t index_bytes = indices.size() * sizeof(T);

		for(size_t i = 0; i < index_bytes; i++) {
			hash ^= static_cast<size_t>(index_data[i]);
			hash *= FNV_PRIME;
		}
	}

	return hash;
}

