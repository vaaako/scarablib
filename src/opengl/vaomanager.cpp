#include "scarablib/opengl/vaomanager.hpp"
#include "scarablib/proper/error.hpp"
#include "scarablib/proper/log.hpp"

#include "scarablib/window/window.hpp" // SDL_GL_GetCurrentContext
// Please keep this so in the future if i want to change SDL version i will just need to rename in one file

std::shared_ptr<VertexArray> VAOManager::get_vao_entry(const size_t hash) const {
	auto it = this->vao_map.find(hash);
	if(it == this->vao_map.end()) {
		throw ScarabError("Hash %zu not found", hash);
	}
	return it->second;
}

void VAOManager::release_vao(const size_t hash) noexcept {
	auto it = this->vao_map.find(hash);
	if (it == this->vao_map.end()) {
		return;
	}

	this->vao_map.erase(it);
	#ifdef SCARAB_DEBUG_VAO_MANAGER
	LOG_DEBUG("Hash %zu released. No longer referenced.", hash);
	#endif
}

void VAOManager::cleanup() noexcept {
	// Unable to relase VAOs correctly
	if(SDL_GL_GetCurrentContext() == NULL) {
		LOG_WARNING_FN("Called without a valid OpenGL context. Leaking GPU resources");
	}
	this->vao_map.clear();
}

