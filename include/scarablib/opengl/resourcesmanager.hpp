#pragma once

#include "scarablib/opengl/shader.hpp"
#include "scarablib/opengl/shader_program.hpp"
#include "scarablib/opengl/vertexarray.hpp"
#include "scarablib/proper/error.hpp"
#include "scarablib/proper/log.hpp"
#include <algorithm>
#include <memory>
#include <unordered_map>

// Manages Vertex Array Buffer, Shader Program and Shaders.
// Granting that none of these are copied
class ResourcesManager {
	public:
		// Shader information to pass to program.
		// If you want to re-use an existing shader just set the same `source`
		struct ShaderInfo {
			// Shader's source
			const char* source = nullptr;
			// Type of this shader
			Shader::Type type = Shader::Type::None;
			// Set to true if the source provided is a custom shader.
			// This DOES NOT necessary means "the shader was created by you"
			bool iscustom = false;
		};

		static ResourcesManager& get_instance() {
			static ResourcesManager instance;
			return instance;
		}

		// -- VERTEX ARRAY

		// Creates a new VertexArray or returns an existing one, based on the vertices and indices.
		// - `vertices`: The vertex data.
		// - `indices`: (Optional) The index data.
		// Returns: The entry for this VAO
		template <typename T, typename U>
		std::shared_ptr<VertexArray> acquire_vertexarray(const std::vector<T>& vertices, const std::vector<U>& indices);

		// Returns an entry of a VAO using its hash.
		// Returns nullptr if not found
		std::shared_ptr<VertexArray> get_vertexarray(const size_t hash) noexcept;

		// -- SHADERS

		// Uploads vertex and fragment shader code to the manager.
		// Returns: A pointer to the existing shader, or a pointer to a newly created shader if it didn't exist.
		std::shared_ptr<ShaderProgram> load_shader_program(const std::vector<ResourcesManager::ShaderInfo>& infos);

		// Returns an existing or new compiled shader
		std::shared_ptr<Shader> get_or_compile_shader(const char* source, Shader::Type type);

		// Retrieves an existing Shader using its hash.
		// Returns nullptr if not found
		std::shared_ptr<Shader> get_shader(const size_t hash) noexcept;

		// Retrieves an existing Shader Program using its hash.
		// Returns nullptr if not found
		std::shared_ptr<ShaderProgram> get_program(const size_t hash) noexcept;


		// Cleans up all maps;
		// WARNING: This is called inside Window destructor, DO NOT call it manually
		void cleanup() noexcept;

	private:
		std::unordered_map<size_t, std::weak_ptr<VertexArray>> vertexarray_cache;
		std::unordered_map<size_t, std::weak_ptr<Shader>> shader_cache;
		std::unordered_map<size_t, std::weak_ptr<ShaderProgram>> program_cache;

		// Helper method for making a single hash out of the vectors for vertices and indices
		template <typename T, typename U>
		size_t compute_hash(const std::vector<T>& vertices, const std::vector<U>& indices = {}) const noexcept;

		// Helper method for making a single hash out of mulitple shader hashes
		size_t combine_shader_hashes(const std::vector<std::shared_ptr<Shader>>& shaders) const noexcept;
};


template <typename T, typename U>
std::shared_ptr<VertexArray> ResourcesManager::acquire_vertexarray(const std::vector<T>& vertices, const std::vector<U>& indices) {
	static_assert(std::is_base_of_v<Vertex, T>, "T must derive from Vertex");
	static_assert(std::is_unsigned_v<U>, "U must be an unsigned integer type");

	if(vertices.empty()) {
		throw ScarabError("Vertices vector is empty for VAO creation");
	}

	// -- CHECK IF CACHED
	const size_t hash = this->compute_hash(vertices, indices);
	std::shared_ptr<VertexArray> vertexarray = this->get_vertexarray(hash);
	if(vertexarray != nullptr) {
	#if defined(SCARAB_DEBUG_VAO_MANAGER)
		LOG_DEBUG("Hash %zu found! Reusing VAO.", hash);
	#endif
		return vertexarray; // Return the existing entry
	}

#if defined(SCARAB_DEBUG_VAO_MANAGER)
	LOG_DEBUG("Hash %zu not found. Creating new VAO.", hash);
#endif

	if(!indices.empty()) {
		// -- CREATE VAO WITH THE SMALLEST POSSIBLE TYPE FOR INDICES
		
		// Use uint64 to be safe
		const uint64 max_val = *std::max_element(indices.begin(), indices.end());
		if(max_val <= UINT8_MAX) {
			vertexarray = std::make_shared<VertexArray>(vertices, ScarabOpenGL::convert_to<uint8>(indices));
		#if defined(SCARAB_DEBUG_VAO_MANAGER)
			LOG_DEBUG("Converted indices to uint8");
		#endif
		} else if(max_val <= UINT16_MAX) {
			vertexarray = std::make_shared<VertexArray>(vertices, ScarabOpenGL::convert_to<uint16>(indices));
		#if defined(SCARAB_DEBUG_VAO_MANAGER)
			LOG_DEBUG("Converted indices to uint16");
		#endif
		// Is uint32, use existing indices
		} else if(max_val <= UINT32_MAX){
			vertexarray = std::make_shared<VertexArray>(vertices, ScarabOpenGL::convert_to<uint32>(indices));
		#if defined(SCARAB_DEBUG_VAO_MANAGER)
			LOG_DEBUG("Converted indices to uint32");
		#endif
		// Is uint64, use existing indices
		} else {
		#if defined(SCARAB_DEBUG_VAO_MANAGER)
			LOG_DEBUG("Creating indices as uint64");
		#endif
			vertexarray = std::make_shared<VertexArray>(vertices, indices);
		}
	} else {
		vertexarray = std::make_shared<VertexArray>(vertices, std::vector<uint8>{});
	}

#if defined(SCARAB_DEBUG_VAO_MANAGER)
	LOG_DEBUG("VAO ID made: %zu", vertexarray->get_vaoid());
#endif

	GL_CHECK();

	vertexarray->hash = hash;
	this->vertexarray_cache[hash] = vertexarray;
	return vertexarray;
}

template <typename T, typename U>
std::size_t ResourcesManager::compute_hash(const std::vector<T>& vertices, const std::vector<U>& indices) const noexcept {
	size_t hash = 0;
	for(const T& vertex : vertices) {
		ScarabHash::hash_combine(hash, vertex);
	}
	for(const U& index : indices) {
		ScarabHash::hash_combine(hash, index);
	}
	return hash;
}

