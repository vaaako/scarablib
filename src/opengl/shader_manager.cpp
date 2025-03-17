#include "scarablib/opengl/shader_manager.hpp"

Shader* ShaderManager::get_or_load_shader(const std::string_view& name, const char* vertex_shader, const char* fragment_shader, const bool is_file_path) noexcept {
	// Return existing shader if already loaded
	if(this->shader_map.find(name) != this->shader_map.end()) {
		return this->shader_map.at(name);
	}

	// NOTE: I could use unique_ptr, but i fell more safe using raw pointer here

	// Load new shader
	Shader* uniq_shader = new Shader(vertex_shader, fragment_shader, is_file_path);
	this->shader_map[name] = std::move(uniq_shader); // move ownerhip to map

	// Return shared pointer
	Shader* shader_ptr = uniq_shader;
	return shader_ptr;
}

Shader* ShaderManager::get_shader(const std::string_view& name) const noexcept {
	if(this->shader_map.find(name) != this->shader_map.end()) {
		return this->shader_map.at(name);
	}
	return nullptr;
}


void ShaderManager::cleanup() noexcept {
	for(auto& [_, entry] : this->shader_map) {
		delete entry;
	}
	this->shader_map.clear();
}
