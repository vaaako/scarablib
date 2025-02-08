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
		vao_map[hash].ref_count--; // Decrement reference count
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


VAOManager::VAOData VAOManager::create_vao(const std::vector<Vertex>& vertices, const std::vector<uint32>& indices) const noexcept {
	VAOData data;

	// Gen VAO
	glGenVertexArrays(1, &data.vao_id);
	glBindVertexArray(data.vao_id);

	// Gen and bind EBO
	glGenBuffers(1, &data.ebo_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.ebo_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizei>(indices.size() * sizeof(uint32)), indices.data(), GL_STATIC_DRAW);

	// Gen and bind VBO
	glGenBuffers(1, &data.vbo_id);
	glBindBuffer(GL_ARRAY_BUFFER, data.vbo_id);
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(Vertex)), vertices.data(), GL_STATIC_DRAW);
	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);
	// Normal
	// glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	// glEnableVertexAttribArray(1);
	// TexUV
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texuv));
	glEnableVertexAttribArray(1);

	// Unbind all
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return data;
}

VAOManager::VAOData VAOManager::create_vao(const std::vector<vec3<float>>& vertices, const std::vector<uint32>& indices) const noexcept {
	VAOData data;

	// Gen VAO
	glGenVertexArrays(1, &data.vao_id);
	glBindVertexArray(data.vao_id);

	// Gen and bind EBO
	glGenBuffers(1, &data.ebo_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.ebo_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizei>(indices.size() * sizeof(uint32)), indices.data(), GL_STATIC_DRAW);

	// Gen and bind VBO
	glGenBuffers(1, &data.vbo_id);
	glBindBuffer(GL_ARRAY_BUFFER, data.vbo_id);
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(vec3<float>)), vertices.data(), GL_STATIC_DRAW);
	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3<float>), (void*)0);
	glEnableVertexAttribArray(0);

	// Unbind all
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return data;
}
