#pragma once

#include "scarablib/types/vertex.hpp"
#include <GL/glew.h>
#include <unordered_map>

// This is a class that handles the creation and management of unique Vertex Array Objects (VAO)
// This class prevents duplicate VAOs from being created (duplicate vertices and indices)
// It is used a get_instance approach so it can be easily accessed from anywhere
class VAOManager {
	public:
		// Get the instance of the VAOManager so is possible to use it's methods
		static VAOManager& get_instance() {
			static VAOManager instance;
			return instance;
		}

		// Check if the VAO of the vertices and indices exists and get its ID
		// If not found, it will create a new entry
		// WARNING: Only use this function one time per mesh
		GLuint make_vao(const std::vector<Vertex>& vertices, const std::vector<uint32>& indices) noexcept;

		// Get a VAO ID from its hash
		GLuint get_vao(const size_t hash) const noexcept;

		// Remove a VAO from the map
		// The method will check if any other model is using the VAO and then release
		void release_vao(const size_t hash) noexcept;

		// Clean up all VAOs
		void cleanup() noexcept;

		// Get an hash based on the vertices and indices
		size_t compute_hash(const std::vector<Vertex>& vertices, const std::vector<uint32>& indices) const noexcept;
	private:
		struct VAOData {
			GLuint vao_id;
			GLuint vbo_id;
			GLuint ebo_id;
		};

		// Keep track of how many models are using the same VAO
		// If reach 0, delete VAO
		struct VAOEntry {
			VAOData data;
			uint32 ref_count;
		};

		std::unordered_map<std::size_t, VAOEntry> vao_map;

		VAOManager() noexcept = default;
		VAOManager(const VAOManager&) noexcept = delete;
		VAOManager& operator=(const VAOManager&) noexcept = delete;

		// Make the VAO, VBO and EBO
		VAOData create_vao(const std::vector<Vertex>& vertices, const std::vector<uint32>& indices) const noexcept;
};
