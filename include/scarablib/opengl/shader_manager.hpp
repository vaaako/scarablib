#pragma once

#include "scarablib/opengl/shader_program.hpp"
#include <memory>
#include <unordered_map>

// Manages memory of all Shaders and Shader Programs created.
// This exists so if a Mesh wants to use the same Shader or Shader Program and this
// already exists, the manager gives the existing memory
class ShaderManager {
	public:
		// Shader information to pass to program.
		// If you want to re-use an existing shader you can either use its source
		// or set `ShaderInfo::shader` to it
		struct ShaderInfo {
			// Shader's source
			const char* source = nullptr;
			// Type of this shader
			Shader::Type type = Shader::Type::None;
			// Set to true if the source provided is a custom shader
			// This DOES NOT mean that the shader was created by you
			bool iscustom = false;
		};

		static ShaderManager& get_instance() {
			static ShaderManager instance;
			return instance;
		}

		// Uploads vertex and fragment shader code to the manager.
		// Returns: A pointer to the existing shader, or a pointer to a newly created shader if it didn't exist.
		std::shared_ptr<ShaderProgram> load_shader_program(const std::vector<ShaderManager::ShaderInfo>& infos);

		// Returns an existing or new compiled shader
		std::shared_ptr<Shader> get_or_compile_shader(const char* source, Shader::Type type);

		// Retrieves an existing Shader using its hash.
		// Returns nullptr if not found
		std::shared_ptr<Shader> get_shader(const size_t hash) noexcept;

		// Retrieves an existing Shader Program using its hash.
		// Returns nullptr if not found
		std::shared_ptr<ShaderProgram> get_program(const size_t hash) noexcept;

		// Releases a Shader from the manager, using its name
		// void release_shader(const size_t hash) noexcept;

		// Cleans up all shaders.
		// WARNING: This is called when a window is destroyed. DO NOT call it manually.
		void cleanup() noexcept;

	private:
		std::unordered_map<size_t, std::weak_ptr<Shader>> shader_cache;
		std::unordered_map<size_t, std::weak_ptr<ShaderProgram>> program_cache;

		// Helper method for making a single hash out of mulitple shader hashes
		size_t combine_shader_hashes(const std::vector<std::shared_ptr<Shader>>& shaders) const noexcept;
};

// I had to make a whole new system for managing shaders and not only Shader Programs
// as i was doing before
//
// But i think its worth it. I used weak_ptr so if no other object is using the Shader/Shader Program
// it will get expired and therefore freeing some unused memory
//
// I know the expired weak_ptr still consumes memory, but is less than ShaderProgram and uint32
