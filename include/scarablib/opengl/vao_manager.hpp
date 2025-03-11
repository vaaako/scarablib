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

		// Check if the vertices and indices (optional) alredy has a VAO.
		// This will make a unique hash based on the vertices and indices.
		// If has is not found in the map, it will create a new VAO and an entry.
		// At the end, it will return the VAO's ID.
		// WARNING: Use this method only one time per Mesh, since it keep track of how many Meshes are using the same VAO, and using it again will mess up the reference count
		template<typename T>
		GLuint make_vao(const std::vector<T>& vertices, const std::vector<uint32_t>& indices = {}) noexcept;

		// Generate a new unique hash based on the vertices and indices (optional)
		template<typename T>
		size_t compute_hash(const std::vector<T>& vertices, const std::vector<uint32_t>& indices = {}) const noexcept;

		// Get the VAO's ID from its hash
		GLuint get_vao(const size_t hash) const noexcept;

		// Remove a VAO from the map.
		// The method will check if any other model is using the VAO and then release
		void release_vao(const size_t hash) noexcept;

		// Clean up all VAOs.
		// WARNING: This is called when a window is destroyed, don't call it manually
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
			VAOManager::VAOData data;
			uint32 ref_count;
		};

		std::unordered_map<size_t, VAOEntry> vao_map;

		// Make the VAO, VBO and EBO.
		// Indices is optional
		template<typename T>
		VAOManager::VAOData create_vao(const std::vector<T>& vertices, const std::vector<uint32>& indices) const noexcept;
};

template<typename T>
GLuint VAOManager::make_vao(const std::vector<T>& vertices, const std::vector<uint32_t>& indices) noexcept {
	static_assert(std::is_same<T, vec3<float>>::value || std::is_same<T, Vertex>::value,
			"Only vec3<float> and Vertex types for vertices are accepted");

	const size_t hash = this->compute_hash(vertices, indices);

	// If not found, make a new VAO and entry
	auto it = this->vao_map.find(hash);
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
template <typename T>
size_t VAOManager::compute_hash(const std::vector<T>& vertices, const std::vector<uint32_t>& indices) const noexcept {
	static_assert(std::is_same<T, vec3<float>>::value || std::is_same<T, Vertex>::value,
			"Only vec3<float> and Vertex types for vertices are accepted");

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

	// Hash index data (only if indices are provided)
	if (!indices.empty()) {
		const auto* index_data = reinterpret_cast<const char*>(indices.data());
		const size_t index_bytes = indices.size() * sizeof(uint32_t);
		
		for (size_t i = 0; i < index_bytes; i++) {
			hash ^= static_cast<size_t>(index_data[i]);
			hash *= FNV_PRIME;
		}
	}

	return hash;
}


template<typename T>
VAOManager::VAOData VAOManager::create_vao(const std::vector<T>& vertices, const std::vector<uint32>& indices) const noexcept {
	// dont need static assert here, since it was already checked in make_vao

	VAOData data;

	// Gen VAO
	glGenVertexArrays(1, &data.vao_id);
	glBindVertexArray(data.vao_id);

	// Gen and bind EBO
	if(!indices.empty()) {
		glGenBuffers(1, &data.ebo_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.ebo_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizei>(indices.size() * sizeof(uint32)), indices.data(), GL_STATIC_DRAW);
	}

	// Gen and bind VBO
	glGenBuffers(1, &data.vbo_id);
	glBindBuffer(GL_ARRAY_BUFFER, data.vbo_id);
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(T)), vertices.data(), GL_STATIC_DRAW);
	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(T), (void*)0);
	glEnableVertexAttribArray(0);

	// T is only a Vertex or a vec3<float>
	// if(sizeof(T) == sizeof(Vertex)) {
	if constexpr (std::is_same<T, Vertex>::value) {
		// Normal
		// glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		// glEnableVertexAttribArray(1);
		// TexUV
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(T), (void*)offsetof(T, texuv));
		glEnableVertexAttribArray(1);
	}

	// Unbind all
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return data;
}
