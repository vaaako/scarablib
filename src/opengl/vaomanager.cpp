#include "scarablib/opengl/vaomanager.hpp"
#include "scarablib/proper/log.hpp"

#include "scarablib/window/window.hpp" // SDL_GL_GetCurrentContext
// Please keep this so in the future if i want to change SDL version i will just need to rename in one file


std::shared_ptr<VertexArray> VAOManager::get_vertexarray(const size_t hash) noexcept {
	auto it = this->vertex_cache.find(hash);
	if(it != this->vertex_cache.end()) {
		if(std::shared_ptr<VertexArray> ptr = it->second.lock()) {
			return ptr;
		}
		this->vertex_cache.erase(it); // expired, remove entry
	}
	return nullptr; // not found or expired
}

// void VAOManager::release_vao(const size_t hash) noexcept {
// 	if(this->vertex_cache.find(hash) == this->vertex_cache.end()) {
// 		return;
// 	}
// 	this->vertex_cache.erase(hash);
//
// 	#ifdef SCARAB_DEBUG_VAO_MANAGER
// 	LOG_DEBUG("Hash %zu released. No longer referenced.", hash);
// 	#endif
// }

void VAOManager::cleanup() noexcept {
	// Unable to relase VAOs correctly
	if(SDL_GL_GetCurrentContext() == NULL) {
		LOG_WARNING_FN("Called without a valid OpenGL context. Leaking GPU resources");
	}

	// Has no effect since its called when window gets destroyed, but i like keep it here
	this->vertex_cache.clear();
}

