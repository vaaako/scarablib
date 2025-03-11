#include "scarablib/opengl/shader_manager.hpp"


Shader* ShaderManager::get_or_load_shader(const char* name, const char* vs_path, const char* fs_path) noexcept {
	// Return existing shader if already loaded
	if(this->shader_map.find(name) != this->shader_map.end()) {
		return this->shader_map.at(name);
	}

	// NOTE: I could use unique_ptr, but i fell more safe using raw pointer here

	// Load new shader
	Shader* uniq_shader = new Shader(vs_path, fs_path);
	this->shader_map[name] = std::move(uniq_shader); // unique_ptr move ownership

	// Return shared pointer
	Shader* shader_ptr = uniq_shader;
	return shader_ptr;
}

Shader* ShaderManager::get_shader(const char* name) const noexcept {
	if(this->shader_map.find(name) != this->shader_map.end()) {
		return this->shader_map.at(name);
	}
	return nullptr;
}


void ShaderManager::cleanup() noexcept {
	for(auto& [hash, entry] : this->shader_map) {
		delete entry;
	}
	this->shader_map.clear();
}
