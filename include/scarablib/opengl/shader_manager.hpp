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

		// Uploads vertex and fragment shader code to the manager.
		// - `name`: The name of the shader. Used for tracking shader existence.
		// - `vertex_shader`: The source code for the vertex shader.
		// - `fragment_shader`: The source code for the fragment shader.
		// Returns: A pointer to the existing shader, or a pointer to a newly created shader if it didn't exist.
		// NOTE:  Use this when overriding `Mesh::get_shader()`
		Shader* get_or_load_shader(const std::string_view& name, const char* vertex_shader, const char* fragment_shader) noexcept;

		// Retrieves an existing shader by its name.
		Shader* get_shader(const std::string_view& name) const noexcept;

		// Cleans up all shaders.
		// WARNING: This is called when a window is destroyed. DO NOT call it manually.
		void cleanup() noexcept;

	private:
		// If I use char* here, it will look up the memory address of the string instead of the string itself
		std::unordered_map<std::string_view, Shader*> shader_map;
};
