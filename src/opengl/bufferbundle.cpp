#include "scarablib/opengl/bufferbundle.hpp"

BufferBundle::~BufferBundle() noexcept {
	// If was made using the manager
	if(this->vao_hash != 0) {
		VAOManager::get_instance().release_vao(vao_hash); // Early release of the manager
	// If not, but still check for the ID
	} else if(this->vao_id != 0) {
		glDeleteVertexArrays(1, &this->vao_id);
	}

	if(this->vbo != nullptr) {
		delete vbo;
	}

	if(this->ebo != nullptr) {
		delete ebo;
	}
}

void BufferBundle::make_vao(const std::vector<Vertex>& vertices, std::vector<uint32>& indices) noexcept {
	bool has_indices = !indices.empty();

	glGenVertexArrays(1, &this->vao_id);
	glBindVertexArray(this->vao_id);

	if(has_indices) {
		this->ebo = new EBO(indices);
	}

	// Make VBO
	this->vbo =  new VBO();
	this->vbo->bind();
	this->vbo->alloc_data(vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	// Link Position and texuv
	this->vbo->link_attrib(0, 3, sizeof(Vertex), 0);
	this->vbo->link_attrib(1, 2, sizeof(Vertex), offsetof(Vertex, texuv));

	// Unbind all
	glBindVertexArray(0);
	this->vbo->unbind();

	if(has_indices) {
		this->ebo->unbind();
	}
}
