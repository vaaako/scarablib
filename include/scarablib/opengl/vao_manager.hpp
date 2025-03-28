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
// This class prevents duplicate VAOs from being created (duplicate vertices and indices).
// It is used a get_instance approach so it can be easily accessed from anywhere
class VAOManager {
	public:
		// Get the instance of the VAOManager so is possible to use it's methods
		static VAOManager& get_instance() {
			static VAOManager instance;
			return instance;
		}

		// Check if the vertices and indices alredy has a VAO.
		// Indices is an optional parameter, you can pass an empty vector.
		// This will make a unique hash based on the vertices and indices.
		// If has is not found in the map, it will create a new VAO and an entry.
		// At the end, it will return the VAO's ID.
		// If T is Vertex it will make a VAO will all Vertex fields.
		// If T is a vec3<float> it will make a VAO with only the position field.
		// usage is an optional parameter, default is GL_STATIC_DRAW, you can pass GL_DYNAMIC_DRAW optionally
		// WARNING: Use this method only one time per Mesh, since it keep track of how many Meshes are using the same VAO, and using it again will mess up the reference count
		template<typename T>
		GLuint get_or_make_vao(const std::vector<T>& vertices, const std::vector<uint32>& indices = {}, const GLenum usage = GL_STATIC_DRAW);

		// Generate a new unique hash based on the vertices and indices.
		// Indices is an optional parameter, you can pass an empty vector.
		template<typename T>
		size_t compute_hash(const std::vector<T>& vertices, const std::vector<uint32>& indices = {}) const noexcept;

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
		template<typename T>
		VAOManager::VAOData create_vao(const std::vector<T>& vertices, const std::vector<uint32>& indices, const GLenum usage) const;
};

template<typename T>
GLuint VAOManager::get_or_make_vao(const std::vector<T>& vertices, const std::vector<uint32>& indices, const GLenum usage) {
	static_assert(std::is_same<T, vec3<float>>::value || std::is_same<T, Vertex>::value,
			"Only vec3<float> and Vertex types for vertices are accepted");
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
template <typename T>
size_t VAOManager::compute_hash(const std::vector<T>& vertices, const std::vector<uint32>& indices) const noexcept {
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


template<typename T>
VAOManager::VAOData VAOManager::create_vao(const std::vector<T>& vertices, const std::vector<uint32>& indices, const GLenum usage) const {
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
