#pragma once

#include "scarablib/opengl/shader.hpp"
#include <unordered_map>

class ShaderManager {
	public:
		static ShaderManager& get_instance() {
			static ShaderManager instance;
			return instance;
		}

		// Returns a reference to a existing shader or make a new one if it doesn't exist.
		// This tracks the shader existence by its name.
		// This should be used in Model::get_shader()
		Shader* get_or_load_shader(const char* name, const char* vs_path, const char* fs_path) noexcept;

		// Get a existing shader by its name.
		// If the shader doesn't exist, it will return nullptr
		Shader* get_shader(const char* name) const noexcept;


		// Clean up all Shaders.
		// WARNING: This is called when a window is destroyed, don't call it manually
		void cleanup() noexcept;

	private:
		std::unordered_map<const char*, Shader*> shader_map;
};
