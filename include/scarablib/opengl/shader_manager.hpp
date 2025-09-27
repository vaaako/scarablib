#pragma once

#include "scarablib/opengl/shader_program.hpp"
#include <memory>
#include <unordered_map>

// Manages memory of all Shaders and Shader Programs created.
// This exists so if a Mesh wants to use the same Shader or Shader Program and this
// already exists, the manager gives the existing memory
class ShaderManager {
	public:
		static ShaderManager& get_instance() {
			static ShaderManager instance;
			return instance;
		}

		// Uploads vertex and fragment shader code to the manager.
		// - `vertex_souce`: The source code for the vertex shader.
		// - `fragment_source`: The source code for the fragment shader.
		// Returns: A pointer to the existing shader, or a pointer to a newly created shader if it didn't exist.
		std::shared_ptr<ShaderProgram> load_shader_program(const std::vector<ShaderProgram::ShaderInfo>& infos);

		// Returns an existing or new compiled shader
		std::shared_ptr<uint32> get_or_compile_shader(const char* source, ShaderProgram::Type type);

		// std::shared_ptr<ShaderProgram> load_custom_shader(const ShaderManager::Type shader_type, const char* user_shader) noexcept;

		// Retrieves an existing Shader using its hash.
		// Returns nullptr if not found
		std::shared_ptr<uint32> get_shader(const size_t hash) noexcept;

		// Retrieves an existing Shader Program using its hash.
		// Returns nullptr if not found
		std::shared_ptr<ShaderProgram> get_program(const size_t hash) noexcept;

		// Releases a Shader from the manager, using its name
		// void release_shader(const size_t hash) noexcept;

		// Cleans up all shaders.
		// WARNING: This is called when a window is destroyed. DO NOT call it manually.
		void cleanup() noexcept;

	private:
		std::unordered_map<size_t, std::weak_ptr<uint32>> shader_cache;
		std::unordered_map<size_t, std::weak_ptr<ShaderProgram>> program_cache;
};

// I had to make a whole new system for managing shaders and not only Shader Programs
// as i was doing before
//
// But i think its worth it. I used weak_ptr so if no other object is using the Shader/Shader Program
// it will get expired and therefore freeing some unused memory
//
// I know the expired weak_ptr still consumes memory, but is less than ShaderProgram and uint32
