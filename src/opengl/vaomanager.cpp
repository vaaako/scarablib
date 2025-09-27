#include "scarablib/opengl/vaomanager.hpp"
#include "scarablib/proper/log.hpp"

#include "scarablib/window/window.hpp" // SDL_GL_GetCurrentContext
// Please keep this so in the future if i want to change SDL version i will just need to rename in one file

std::shared_ptr<VertexArray> VAOManager::get_vao_entry(const size_t hash) const {
	if(this->vao_map.find(hash) == this->vao_map.end()) {
		return nullptr;
	}
	return this->vao_map.at(hash);
}

void VAOManager::release_vao(const size_t hash) noexcept {
	if(this->vao_map.find(hash) == this->vao_map.end()) {
		return;
	}
	this->vao_map.erase(hash);

	#ifdef SCARAB_DEBUG_VAO_MANAGER
	LOG_DEBUG("Hash %zu released. No longer referenced.", hash);
	#endif
}

void VAOManager::cleanup() noexcept {
	// Unable to relase VAOs correctly
	if(SDL_GL_GetCurrentContext() == NULL) {
		LOG_WARNING_FN("Called without a valid OpenGL context. Leaking GPU resources");
	}

	// Has no effect since its called when window gets destroyed, but i like keep it here
	this->vao_map.clear();
}

