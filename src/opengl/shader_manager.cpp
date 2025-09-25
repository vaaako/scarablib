#include "scarablib/opengl/shader_manager.hpp"
#include "scarablib/opengl/shaders.hpp"

#include "scarablib/proper/log.hpp"
#include "scarablib/window/window.hpp" // SDL_GL_GetCurrentContext
// Please keep this so in the future if i want to change SDL version i will just need to rename in one file

std::shared_ptr<ShaderProgram> ShaderManager::load_shader(const std::string_view& name, const char* vertex_shader, const char* fragment_shader) noexcept {
	// Return existing shader if already loaded
	std::shared_ptr<ShaderProgram> shader = this->get_shader(name);
	if(shader != nullptr) {
		return shader;
	}

	// Make new shader
	std::shared_ptr<ShaderProgram> new_shader = std::make_shared<ShaderProgram>(vertex_shader, fragment_shader);
	this->shader_map[name] = new_shader;
	return new_shader;
}

std::shared_ptr<ShaderProgram> ShaderManager::load_custom_shader(const std::string_view& name, uint32 vertexid, const char* user_shader) noexcept {
	// Return existing shader if already loaded
	std::shared_ptr<ShaderProgram> shader = this->get_shader(name);
	if(shader != nullptr) {
		return shader;
	}

	// Inject user shader
	std::string sfragment   = std::string(Shaders::DEFAULT_FRAGMENT);
	std::string placeholder = "// {{USER_CODE}}";

	sfragment.replace(
		// Find substring
		sfragment.find(placeholder),
		// Size
		placeholder.length(),
		// Replace with
		"#define HAS_USER_SHADER\n" + std::string(user_shader)
	);

	LOG_DEBUG("final shader: %s", sfragment.c_str());

	// Make new shader
	std::shared_ptr<ShaderProgram> new_shader = std::make_shared<ShaderProgram>(Shaders::DEFAULT_VERTEX, sfragment.c_str());
	this->shader_map[name] = new_shader;
	return new_shader;
}

std::shared_ptr<ShaderProgram> ShaderManager::get_shader(const std::string_view& name) const noexcept {
	auto it = shader_map.find(name);
	if(it != this->shader_map.end()) {
		return it->second;
	}
	return nullptr;
}


void ShaderManager::release_vao(const std::string_view& name) noexcept {
	auto it = this->shader_map.find(name);
	if(it == this->shader_map.end()) {
		return;
	}
	this->shader_map.erase(it);
}


void ShaderManager::cleanup() noexcept {
	// Unable to relase VAOs correctly
	if(SDL_GL_GetCurrentContext() == NULL) {
		LOG_WARNING_FN("Called without a valid OpenGL context. Leaking GPU resources");
	}
	this->shader_map.clear();
}
