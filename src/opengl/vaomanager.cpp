#include "scarablib/opengl/vaomanager.hpp"
#include "scarablib/proper/error.hpp"
#include "scarablib/proper/log.hpp"

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

void VAOManager::release_vao(const size_t hash) noexcept {
	auto it = vao_map.find(hash);

	// Does not exist
	if(it == this->vao_map.end()) {
		return;
	}

	// Decrement reference count
	vao_map[hash].ref_count--;

	// If no more hash is using this entry
	if(vao_map[hash].ref_count != 0) {
		return;
	}

	const VAOEntry& data = it->second;

	// Delete all buffers at once
	glDeleteBuffers(1, &data.vbo_id);
	glDeleteBuffers(1, &data.ebo_id);
	glDeleteVertexArrays(1, &data.vao_id);

	vao_map.erase(it);

	#ifdef SCARAB_DEBUG_VAO_MANAGER
	LOG_DEBUG("Hash %zu deleted succesfully", hash);
	#endif
}

void VAOManager::cleanup() noexcept {
	for(auto& [hash, entry] : this->vao_map) {
		// Delete all buffers at once
		glDeleteBuffers(1, &entry.vbo_id);
		glDeleteBuffers(1, &entry.ebo_id);
		glDeleteVertexArrays(1, &entry.vao_id);
	}
	this->vao_map.clear();
}

