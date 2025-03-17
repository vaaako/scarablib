#pragma once

#include "scarablib/opengl/shader.hpp"
#include <string_view>
#include <unordered_map>

class ShaderManager {
	public:
		static ShaderManager& get_instance() {
			static ShaderManager instance;
			return instance;
		}

		// Returns a reference to a existing shader or make a new one if it doesn't exist.
		// This tracks the shader existence by its name.
		// is_file_path is true if vertex and fragment are file paths.
		// NOTE: This should be used when overriding Model::get_shader()
		Shader* get_or_load_shader(const std::string_view& name, const char* vertex_shader, const char* fragment_shader, const bool is_file_path = false) noexcept;

		// Get a existing shader by its name.
		// If the shader doesn't exist, it will return nullptr
		Shader* get_shader(const std::string_view& name) const noexcept;


		// Clean up all Shaders.
		// WARNING: This is called when a window is destroyed, don't call it manually
		void cleanup() noexcept;

	private:
		// If I use char* here, it will look up the memory address of the string instead of the string itself
		std::unordered_map<std::string_view, Shader*> shader_map;
};
