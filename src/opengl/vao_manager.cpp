#include "scarablib/opengl/vao_manager.hpp"
#include "scarablib/proper/log.hpp"

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
	for(auto& [hash, entry] : this->vao_map) {
		const VAOData& data = entry.data;
		GLuint buffers[] = { data.vbo_id, data.ebo_id }; // Delete both at the same time
		glDeleteBuffers(2, buffers);
		glDeleteVertexArrays(1, &data.vao_id);
	}
	this->vao_map.clear();
}


/*
GLuint VAOManager::get_or_make_vao(const std::vector<std::vector<vec3<float>>>& data, const std::vector<uint32>& indices) {
	if(data.empty() || data.at(0).empty()) {
		throw ScarabError("Data vector is empty");
	}

	// Pass vertices
	const size_t hash = this->compute_hash(data.at(0), indices);

	// If not found, make a new VAO and entry
	auto it = this->vao_map.find(hash);
	if (it == this->vao_map.end()) {
		// Make VAO using all the data
		VAOEntry entry = { .data = this->create_vao(data, indices), .ref_count = 1 };
		this->vao_map[hash] = entry;

		#ifdef SCARAB_DEBUG_VAO_MANAGER
		LOG_DEBUG("Hash %zu created succesfully", hash);
		#endif

		return entry.data.vao_id;
	}

	#ifdef SCARAB_DEBUG_VAO_MANAGER
	LOG_DEBUG("Hash %zu found!", hash);
	#endif

	// Keep track of ref_count (trying to make another VAO of the same data)
	it->second.ref_count++;
	return it->second.data.vao_id;
}
*/


/*
VAOManager::VAOData VAOManager::create_vao(const std::vector<std::vector<vec3<float>>>& vertices, const std::vector<uint32>& indices) const {
	VAOData data;

	// Gen VAO
	glGenVertexArrays(1, &data.vao_id);
	glBindVertexArray(data.vao_id);

	// Gen and bind EBO
	if(!indices.empty()) {
		glGenBuffers(1, &data.ebo_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.ebo_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizei>(indices.size() * sizeof(uint32)), indices.data(), GL_STATIC_DRAW);
	}

	// Combine all vertices (positions + texCoords)
	std::vector<vec3<float>> combined_vertices;
	for(GLuint i = 0; i < vertices.size(); i++) {
		for (const auto& vertex : vertices.at(i)) {
			combined_vertices.push_back(vertex);
		}
	}

	// Gen and bind VBO
	glGenBuffers(1, &data.vbo_id);
	glBindBuffer(GL_ARRAY_BUFFER, data.vbo_id);
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(combined_vertices.size() * sizeof(vec3<float>)), combined_vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3<float>), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vec2<float>), (void*)sizeof(vec3<float>));
	glEnableVertexAttribArray(1);

	// Set up attributes
	// TODO: Take a uint8 parameter that tells the dimension of each data (32 -> 3 dimensions first data, 2	dimensions second data)

	// uint32 offset = 0;
	// for(GLuint i = 0; i < vertices.size(); i++) {
	// 	glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, sizeof(vec3<float>), (void*)offset);
	// 	glEnableVertexAttribArray(i);
	//
	// 	offset += sizeof(vec3<float>);
	// }

	// Unbind all
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return data;
}
*/

