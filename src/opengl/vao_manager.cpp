#include "scarablib/opengl/vao_manager.hpp"
#include "scarablib/proper/log.hpp"

// std::vector<Vertex>
GLuint VAOManager::make_vao(const std::vector<Vertex>& vertices, const std::vector<uint32>& indices) noexcept {
	size_t hash = this->compute_hash(vertices, indices);
	// If not found, make a new VAO and new entry
	if(this->vao_map.find(hash) == this->vao_map.end()) {
		this->vao_map[hash] = { .data = this->create_vao(vertices, indices), .ref_count = 1 };
	// If found and not already in use, increment ref count
	} else {
		vao_map.at(hash).ref_count++; // More models using this VAO
	}
	return this->vao_map.at(hash).data.vao_id;
}

// std::vector<vec3<float>>
GLuint VAOManager::make_vao(const std::vector<vec3<float>>& vertices, const std::vector<uint32>& indices) noexcept {
	size_t hash = this->compute_hash(vertices, indices);
	// If not found, make a new VAO and new entry
	if(this->vao_map.find(hash) == this->vao_map.end()) {
		this->vao_map[hash] = { .data = this->create_vao(vertices, indices), .ref_count = 1 };
	// If found and not already in use, increment ref count
	} else {
		vao_map.at(hash).ref_count++; // More models using this VAO
	}
	return this->vao_map.at(hash).data.vao_id;
}

GLuint VAOManager::get_vao(const size_t hash) const noexcept {
	if(this->vao_map.find(hash) == this->vao_map.end()) {
		return 0;
	}
	return this->vao_map.at(hash).data.vao_id;
}

void VAOManager::release_vao(const size_t hash) noexcept {
	if(this->vao_map.find(hash) != this->vao_map.end()) {
		vao_map[hash].ref_count--; // Decrement reference count
		if(vao_map[hash].ref_count == 0) {
			// Delete VAO, VBO and EBO if no longer in use
			glDeleteVertexArrays(1, &vao_map[hash].data.vao_id);
			glDeleteBuffers(1, &vao_map[hash].data.vbo_id);
			glDeleteBuffers(1, &vao_map[hash].data.ebo_id);
			vao_map.erase(hash);
		}

		#ifdef SCARAB_DEBUG_RELEASE_VAO
		LOG_DEBUG_FN("Hash %d deleted succesfully", hash);
		#endif
	}
}

void VAOManager::cleanup() noexcept {
	for(auto& [hash, entry] : vao_map) {
		glDeleteVertexArrays(1, &entry.data.vao_id);
		glDeleteBuffers(1, &entry.data.vbo_id);
		glDeleteBuffers(1, &entry.data.ebo_id);
	}
	vao_map.clear();
}

// std::vector<Vertex>
size_t VAOManager::compute_hash(const std::vector<Vertex>& vertices, const std::vector<uint32>& indices) const noexcept {
	size_t seed = vertices.size() + indices.size();
	for(const auto& v : vertices) {
		seed ^= std::hash<float>{}(v.position.x) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		seed ^= std::hash<float>{}(v.position.y) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		seed ^= std::hash<float>{}(v.position.z) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}
	for(const auto& i : indices) {
		seed ^= std::hash<uint32>{}(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}
	return seed;
}

// std::vector<vec3<float>>
size_t VAOManager::compute_hash(const std::vector<vec3<float>>& vertices, const std::vector<uint32>& indices) const noexcept {
	size_t seed = vertices.size() + indices.size();
	for(const auto& v : vertices) {
		seed ^= std::hash<float>{}(v.x) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		seed ^= std::hash<float>{}(v.y) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		seed ^= std::hash<float>{}(v.z) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}
	for(const auto& i : indices) {
		seed ^= std::hash<uint32>{}(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}
	return seed;
}

// TODO: How to unload VBO and EBO dynamically?
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
