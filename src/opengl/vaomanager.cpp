#include "scarablib/opengl/vaomanager.hpp"
#include "scarablib/proper/error.hpp"
#include "scarablib/proper/log.hpp"
#include "scarablib/window/window.hpp"

VAOManager::VAOEntry VAOManager::get_vao_entry(const size_t hash) const {
	auto it = vao_map.find(hash);
	if(it == this->vao_map.end()) {
		throw ScarabError("Hash %zu not found", hash);
	}
	return it->second;
}

void VAOManager::new_entry(const size_t hash, VAOEntry entry) noexcept {
	// If not found, make a new VAO and entry
	auto it = this->vao_map.find(hash);
	if(it == this->vao_map.end()) {
		entry.ref_count = 1;
		this->vao_map[hash] = entry;

		#ifdef SCARAB_DEBUG_VAO_MANAGER
		LOG_DEBUG("Hash %zu created succesfully", hash);
		#endif
		return;
	}

	#ifdef SCARAB_DEBUG_VAO_MANAGER
	LOG_DEBUG("Hash %zu found!", hash);
	#endif

	it->second.ref_count++;
}

void VAOManager::increase_ref_count(const size_t hash) noexcept {
	auto it = this->vao_map.find(hash);
	if(it != this->vao_map.end()) {
		it->second.ref_count++;
	}
}

void VAOManager::release_vao(const size_t hash) noexcept {
	auto it = vao_map.find(hash);

	// Does not exist
	if (it == this->vao_map.end()) {
		return;
	}

	// Decrement reference count using the iterator
	it->second.ref_count--;

	// If the VAO is still in use by other objects
	if (it->second.ref_count > 0) {
		return;
	}

	// Remove from map if count is 0
	// NOTE: Not deleting here. Just cleaning up CPU memory.
	// The GPU memory will be cleaned up later in the main `cleanup()` call
	vao_map.erase(it);

	#ifdef SCARAB_DEBUG_VAO_MANAGER
	LOG_DEBUG("Hash %zu released. No longer referenced.", hash);
	#endif
}

void VAOManager::cleanup() noexcept {
	// Unable to relase VAOs correctly
	if(SDL_GL_GetCurrentContext() == NULL) {
		LOG_WARNING_FN("Called without a valid OpenGL context. Leaking GPU resources");
		this->vao_map.clear();
	}

	for(auto& [hash, entry] : this->vao_map) {
		// Delete all buffers at once
		glDeleteBuffers(1, &entry.vbo_id);
		glDeleteBuffers(1, &entry.ebo_id);
		glDeleteVertexArrays(1, &entry.vao_id);
	}
	this->vao_map.clear();
}

