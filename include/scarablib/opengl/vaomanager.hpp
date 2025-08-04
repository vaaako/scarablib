#pragma once

#include "scarablib/geometry/vertex.hpp"
#include <unordered_map>

// This is a class that handles the creation and management of unique Vertex Array Objects (VAO).
// This class prevents duplicate VAOs from being created
class VAOManager {
	public:
		// Struct used as entry in VAOManager
		struct VAOEntry {
			uint32 vao_id;
			uint32 vbo_id;
			uint32 ebo_id;
			// Tracks how many hashes are using this VAO.
			// This is used because when deleting a entry, it checks if any more hashes are using it.
			// If not, it removes the entry from the map.
			// This prevents from objects deleting VAO that other objects are using.
			// Keep it zero when creating a new entry
			uint32 ref_count = 0;
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
		// Returns: A unique size_t hash
		template<typename T, typename U>
		size_t compute_hash(const std::vector<T>& vertices, const std::vector<U>& indices = {}) const noexcept;

		// Creates a new entry to the manager
		void new_entry(const size_t hash, VAOEntry entry) noexcept;

		// Increases reference count of a hash
		void increase_ref_count(const size_t hash) noexcept;

		// Returns true if the hash is found and VAO already exists
		inline bool vao_exists(const size_t hash) const noexcept {
			return this->vao_map.find(hash) != this->vao_map.end();
		}

		// Returns an entry of a VAO using its hash
		// Error if the hash is not found
		VAOManager::VAOEntry get_vao_entry(const size_t hash) const;

		// Releases a VAO from the manager, using its hash
		void release_vao(const size_t hash) noexcept;

		// Cleans up all VAOs.
		// WARNING: This is called inside Window destructor.
		// DO NOT call it manually
		void cleanup() noexcept;

	private:
		// Hash, VAOEntry
		std::unordered_map<size_t, VAOEntry> vao_map;
};

// Specialized hash function for better distribution
template <typename T, typename U>
size_t VAOManager::compute_hash(const std::vector<T>& vertices, const std::vector<U>& indices) const noexcept {
	static_assert(std::is_same_v<T, vec3<float>> || std::is_base_of_v<Vertex, T>,
			"Only vec3<float> and Vertex types for vertices are accepted");

	static_assert(
		std::is_same_v<U, uint32> ||
		std::is_same_v<U, uint16> ||
		std::is_same_v<U, uint8>,
		"Only uint32, uint16 and uint8 types for indices are accepted");

	// Use FNV-1a hash for better distribution
	constexpr size_t FNV_PRIME = 1099511628211ULL;
	constexpr size_t FNV_OFFSET = 14695981039346656037ULL;

	size_t hash = FNV_OFFSET;

	// Hash vertex data
	const auto* data = reinterpret_cast<const char*>(vertices.data());
	const size_t vertex_bytes = vertices.size() * sizeof(T);
	
	for(size_t i = 0; i < vertex_bytes; i++) {
		hash ^= static_cast<size_t>(data[i]);
		hash *= FNV_PRIME;
	}

	// Hash index data (only if indices are provided)
	if(!indices.empty()) {
		const auto* index_data = reinterpret_cast<const char*>(indices.data());
		const size_t index_bytes = indices.size() * sizeof(U);
		
		for (size_t i = 0; i < index_bytes; i++) {
			hash ^= static_cast<size_t>(index_data[i]);
			hash *= FNV_PRIME;
		}
	}

	return hash;
}

