#pragma once

#include "scarablib/geometry/vertex.hpp"
#include "scarablib/opengl/vertexarray.hpp"
#include "scarablib/proper/error.hpp"
#include "scarablib/proper/log.hpp"
#include <algorithm>
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
		template <typename T, typename U>
		std::shared_ptr<VertexArray> acquire_vertexarray(const std::vector<T>& vertices, const std::vector<U>& indices);

		// Returns an entry of a VAO using its hash.
		// Returns nullptr if not found
		std::shared_ptr<VertexArray> get_vertexarray(const size_t hash) noexcept;

		// Releases a VAO from the manager, using its hash.
		// Does nothing if not found
		// void release_vao(const size_t hash) noexcept;

		// Cleans up all VAOs.
		// WARNING: This is called inside Window destructor.
		// DO NOT call it manually
		void cleanup() noexcept;

	private:
		std::unordered_map<size_t, std::weak_ptr<VertexArray>> vertex_cache;

		// Specialized hash function for better distribution
		template <typename T, typename U>
		size_t compute_hash(const std::vector<T>& vertices, const std::vector<U>& indices = {}) const noexcept;
};

#define SCARAB_DEBUG_VAO_MANAGER

template <typename T, typename U>
std::shared_ptr<VertexArray> VAOManager::acquire_vertexarray(const std::vector<T>& vertices, const std::vector<U>& indices) {
	if(vertices.empty()) {
		throw ScarabError("Vertices vector is empty for VAO creation");
	}

	// -- CHECK IF CACHED
	const size_t hash = this->compute_hash(vertices, indices);
	std::shared_ptr<VertexArray> vertexarray = this->get_vertexarray(hash);
	if(vertexarray != nullptr) {
		#ifdef SCARAB_DEBUG_VAO_MANAGER
		LOG_DEBUG("Hash %zu found! Reusing VAO.", hash);
		#endif
		return vertexarray; // Return the existing entry
	}

	#ifdef SCARAB_DEBUG_VAO_MANAGER
	LOG_DEBUG("Hash %zu not found. Creating new VAO.", hash);
	#endif

	// -- CREATE BUFFERS
	if(!indices.empty()) {
		// -- CREATE VAO WITH THE SMALLEST POSSIBLE TYPE FOR INDICES
		const uint32 max_val = *std::max_element(indices.begin(), indices.end());
		if(max_val <= UINT8_MAX) {
			vertexarray = std::make_shared<VertexArray>(vertices, ScarabOpenGL::convert_to<uint8>(indices));
		} else if(max_val <= UINT16_MAX) {
			vertexarray = std::make_shared<VertexArray>(vertices, ScarabOpenGL::convert_to<uint16>(indices));
		// Is uint32 or uint64, use existing indices
		} else {
			vertexarray = std::make_shared<VertexArray>(vertices, indices);
		}
	} else {
		vertexarray = std::make_shared<VertexArray>(vertices, std::vector<uint8>{});
	}

	#ifdef SCARAB_DEBUG_VAO_MANAGER
	LOG_DEBUG("VAO ID made: %zu", vertexarray->get_vaoid());
	#endif

	GL_CHECK();

	vertexarray->hash = hash;
	this->vertex_cache[hash] = vertexarray;
	return vertexarray;
}

template <typename T, typename U>
std::size_t VAOManager::compute_hash(const std::vector<T>& vertices, const std::vector<U>& indices) const noexcept {
	size_t hash = 0;
	for(const T& vertex : vertices) {
		ScarabHash::hash_combine(hash, vertex);
	}
	for(const U& index : indices) {
		ScarabHash::hash_combine(hash, index);
	}
	return hash;
}

