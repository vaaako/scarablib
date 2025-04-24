#pragma once

#include "scarablib/proper/error.hpp"
#include "scarablib/types/vertex.hpp"
#include <GL/glew.h>
#include <algorithm>
#include <unordered_map>

#ifdef SCARAB_DEBUG_VAO_MANAGER 
#include "scarablib/proper/log.hpp"
#endif

// This is a class that handles the creation and management of unique Vertex Array Objects (VAO).
// This class prevents duplicate VAOs from being created
class VAOManager {
	public:
		// Get the singleton instance of the VAOManager reference
		static VAOManager& get_instance() {
			static VAOManager instance;
			return instance;
		}

		// Retrieves or creates a VAO for the given vertices and indices.
		// - `vertices`: The vertex data. Uses all vertex fields if T is Vertex, or only position if T is vec3<float>.
		// - `indices`: (Optional) The index data. Can be an empty vector.
		// - `usage`: (Optional) The buffer usage (e.g., GL_STATIC_DRAW, GL_DYNAMIC_DRAW). Default is GL_STATIC_DRAW.
		// Returns: The ID of the VAO. Creates a new VAO if one doesn't exist
		template<typename T, typename U>
		GLuint get_or_make_vao(const std::vector<T>& vertices, const std::vector<U>& indices = {}, const GLenum usage = GL_STATIC_DRAW);

		// Computes a unique hash based on the provided vertices and indices.
		// This hash should be created and stored before making the VAO. So it's possible to delete it.
		// - `vertices`: The vertex data.
		// - `indices`: (Optional) The index data.
		// Returns: A unique size_t hash
		template<typename T, typename U>
		size_t compute_hash(const std::vector<T>& vertices, const std::vector<U>& indices = {}) const noexcept;

		// Returns a VAO's ID by its hash
		GLuint get_vao(const size_t hash) const noexcept;

		// Releases a VAO from the manager, using its hash.
		void release_vao(const size_t hash) noexcept;

		// Cleans up all VAOs.
		// WARNING: This is called when a window is destroyed. DO NOT call it manually
		void cleanup() noexcept;
	private:
		struct VAOData {
			GLuint vao_id;
			GLuint vbo_id;
			GLuint ebo_id = 0; // Sometimes ebo is not used, pass a default value so when is deleted it won't cause any warning
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
		template<typename T, typename U>
		VAOManager::VAOData create_vao(const std::vector<T>& vertices, const std::vector<U>& indices, const GLenum usage) const;
};

template<typename T, typename U>
GLuint VAOManager::get_or_make_vao(const std::vector<T>& vertices, const std::vector<U>& indices, const GLenum usage) {
	static_assert(std::is_same<T, vec3<float>>::value || std::is_same<T, Vertex>::value,
			"Only vec3<float> and Vertex types for vertices are accepted");

	static_assert(
		std::is_same_v<U, uint32> ||
		std::is_same_v<U, uint16> ||
		std::is_same_v<U, uint8>,
		"Only uint32, uint16 and uint8 types for indices are accepted");

	if(vertices.empty()) {
		throw ScarabError("Vertices vector is empty");
	}

	const size_t hash = this->compute_hash(vertices, indices);

	// If not found, make a new VAO and entry
	auto it = this->vao_map.find(hash);
	if (it == this->vao_map.end()) {
		VAOEntry entry = { .data = this->create_vao(vertices, indices, usage), .ref_count = 1 };
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
template <typename T, typename U>
size_t VAOManager::compute_hash(const std::vector<T>& vertices, const std::vector<U>& indices) const noexcept {
	static_assert(std::is_same<T, vec3<float>>::value || std::is_same<T, Vertex>::value,
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
		const size_t index_bytes = indices.size() * sizeof(uint32);
		
		for (size_t i = 0; i < index_bytes; i++) {
			hash ^= static_cast<size_t>(index_data[i]);
			hash *= FNV_PRIME;
		}
	}

	return hash;
}


template<typename T, typename U>
VAOManager::VAOData VAOManager::create_vao(const std::vector<T>& vertices, const std::vector<U>& indices, const GLenum usage) const {
	// dont need static assert here, since it was already checked in make_vao

	VAOData data;

	// Gen VAO
	glGenVertexArrays(1, &data.vao_id);
	glBindVertexArray(data.vao_id);

	// Gen and bind EBO
	if(!indices.empty()) {
		glGenBuffers(1, &data.ebo_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.ebo_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizei>(indices.size() * sizeof(U)), indices.data(), GL_STATIC_DRAW);
	}

	// Gen and bind VBO
	glGenBuffers(1, &data.vbo_id);
	glBindBuffer(GL_ARRAY_BUFFER, data.vbo_id);
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(T)), vertices.data(), usage);
	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(T), (void*)0);
	glEnableVertexAttribArray(0);

	// If T is a Vertex, add the other attributes
	if constexpr (std::is_same<T, Vertex>::value) {
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(T), (void*)offsetof(T, texuv));
		glEnableVertexAttribArray(1);

		// Format texid
		bool can_upload_tex_id = std::all_of(vertices.begin(), vertices.end(), [](const T& v) {
			return v.texid > -1 && v.texid <= 255;
		});

		if(can_upload_tex_id) {
			glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(T), (void*)offsetof(T, texid));
			glEnableVertexAttribArray(2);
		}

		// Format shading
		bool can_upload_shading = std::all_of(vertices.begin(), vertices.end(), [](const T& v) {
			return v.shading > -1.0f && v.shading <= 1.0f;
		});

		if(can_upload_shading) {
			// If texid was uploaded, shading should be at index 3
			uint8 vertex_id = (can_upload_tex_id) ? 3 : 2;
			glVertexAttribPointer(vertex_id, 1, GL_FLOAT, GL_FALSE, sizeof(T), (void*)offsetof(T, shading));
			glEnableVertexAttribArray(vertex_id);
		}

		// Normal
		// glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		// glEnableVertexAttribArray(2);
		// TexUV
	}

	// Unbind all
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return data;
}
