#pragma once

#include "scarablib/types/vertex.hpp"
#include <GL/glew.h>
#include <unordered_map>

#ifdef SCARAB_DEBUG_VAO_MANAGER 
#include "scarablib/proper/log.hpp"
#endif

// This is a class that handles the creation and management of unique Vertex Array Objects (VAO).
// This class prevents duplicate VAOs from being created (duplicate vertices and indices).
// It is used a get_instance approach so it can be easily accessed from anywhere
class VAOManager {
	public:
		// Get the instance of the VAOManager so is possible to use it's methods
		static VAOManager& get_instance() {
			static VAOManager instance;
			return instance;
		}

		// Check if the VAO of the vertices (std::vector<vec3<float>>) and indices exists and get its ID.
		// If not found, it will create a new entry.
		// WARNING: Only use this function one time per mesh
		template<typename T>
		GLuint make_vao(const std::vector<T>& vertices, const std::vector<uint32_t>& indices) noexcept;

		// Get an hash based on the vertices (std::vector<T>) and indices
		template<typename T>
		size_t compute_hash(const std::vector<T>& vertices, const std::vector<uint32_t>& indices) const noexcept;

		// Get a VAO ID from its hash
		GLuint get_vao(const size_t hash) const noexcept;

		// Remove a VAO from the map.
		// The method will check if any other model is using the VAO and then release
		void release_vao(const size_t hash) noexcept;

		// Clean up all VAOs
		void cleanup() noexcept;
	private:
		struct VAOData {
			GLuint vao_id;
			GLuint vbo_id;
			GLuint ebo_id;
		};

		// Keep track of how many models are using the same VAO.
		// If reach 0, delete VAO
		struct VAOEntry {
			VAOData data;
			uint32 ref_count;
		};

		std::unordered_map<size_t, VAOEntry> vao_map;

		VAOManager() noexcept = default;
		VAOManager(const VAOManager&) noexcept = delete;
		VAOManager& operator=(const VAOManager&) noexcept = delete;

		// Make the VAO, VBO and EBO
		VAOData create_vao(const std::vector<Vertex>& vertices, const std::vector<uint32>& indices) const noexcept;
		// Make the VAO, VBO and EBO (using single pos vertices)
		VAOData create_vao(const std::vector<vec3<float>>& vertices, const std::vector<uint32>& indices) const noexcept;
};

template<typename T>
GLuint VAOManager::make_vao(const std::vector<T>& vertices, const std::vector<uint32_t>& indices) noexcept {
	const size_t hash = compute_hash(vertices, indices);

	auto it = this->vao_map.find(hash);
	// If not found, make a new VAO and entry
	if (it == this->vao_map.end()) {
		VAOEntry entry = { .data = this->create_vao(vertices, indices), .ref_count = 1 };
		this->vao_map[hash] = entry;

		#ifdef SCARAB_DEBUG_VAO_MANAGER
		LOG_DEBUG("Hash %zu created succesfully", hash);
		#endif

		return entry.data.vao_id;
	}

	#ifdef SCARAB_DEBUG_VAO_MANAGER
	LOG_DEBUG("Hash %zu found!", hash);
	#endif

	// Keep track of ref_count (trying to make another VAO of the same data)
	it->second.ref_count++;
	return it->second.data.vao_id;
}

// Specialized hash function for better distribution
template<typename T>
size_t VAOManager::compute_hash(const std::vector<T>& vertices, const std::vector<uint32_t>& indices) const noexcept {
	// Use FNV-1a hash for better distribution
	constexpr size_t FNV_PRIME = 1099511628211ULL;
	constexpr size_t FNV_OFFSET = 14695981039346656037ULL;

	size_t hash = FNV_OFFSET;

	// Hash vertex data
	const auto* data = reinterpret_cast<const char*>(vertices.data());
	const size_t vertex_bytes = vertices.size() * sizeof(T);
	
	for (size_t i = 0; i < vertex_bytes; i++) {
		hash ^= static_cast<size_t>(data[i]);
		hash *= FNV_PRIME;
	}

	// Hash index data
	const auto* index_data = reinterpret_cast<const char*>(indices.data());
	const size_t index_bytes = indices.size() * sizeof(uint32_t);
	
	for (size_t i = 0; i < index_bytes; i++) {
		hash ^= static_cast<size_t>(index_data[i]);
		hash *= FNV_PRIME;
	}

	return hash;
}
