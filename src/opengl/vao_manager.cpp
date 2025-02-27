#include "scarablib/opengl/vao_manager.hpp"

GLuint VAOManager::get_vao(const size_t hash) const noexcept {
	auto it = vao_map.find(hash);
	if(it != this->vao_map.end()) {
		return it->second.data.vao_id;
	}
	return 0;
}

void VAOManager::release_vao(const size_t hash) noexcept {
	auto it = vao_map.find(hash);
	if(it != this->vao_map.end()) {
		// Decrement reference count
		vao_map[hash].ref_count--;
		// If no more hash is using this VAO
		if(vao_map[hash].ref_count == 0) {
			const VAOData& data = it->second.data;

			// Delete all buffers at once
			GLuint buffers[] = { data.vbo_id, data.ebo_id };
			glDeleteBuffers(2, buffers);
			glDeleteVertexArrays(1, &data.vao_id);

			vao_map.erase(it);

			#ifdef SCARAB_DEBUG_VAO_MANAGER
			LOG_DEBUG("Hash %zu deleted succesfully", hash);
			#endif
		}
	}
}

void VAOManager::cleanup() noexcept {
	for(auto& [hash, entry] : vao_map) {
		const VAOData& data = entry.data;
		GLuint buffers[] = { data.vbo_id, data.ebo_id };
		glDeleteBuffers(2, buffers);
		glDeleteVertexArrays(1, &data.vao_id);
	}
	vao_map.clear();
}

