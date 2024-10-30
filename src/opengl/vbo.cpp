#include "scarablib/opengl/vbo.hpp"

VBO::VBO() {
	glGenBuffers(1, &this->id);
}

VBO::~VBO() {
	glDeleteBuffers(1, &this->id);
}

void VBO::alloc_data(const uint64 size, const void* data, const GLenum drawtype) {
	this->bind();
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(size), data, drawtype);
	this->unbind();
}

void VBO::link_attrib(const uint32 index, const uint32 dimension, const int total_byte_size, const uint32 offset) {
	this->bind();
	glVertexAttribPointer(index, static_cast<GLint>(dimension), GL_FLOAT, GL_FALSE, total_byte_size, (void*)(offset * sizeof(float)));
	glEnableVertexAttribArray(index);
	this->unbind();
}



void VBO::alloc_data(const std::vector<float>& data, const GLenum drawtype) {
	this->bind();
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(data.size() * sizeof(float)), data.data(), drawtype);
	this->unbind();
}

void VBO::link_attrib(const uint32 index, const uint32 dimension) {
	this->bind();
	glVertexAttribPointer(index, static_cast<GLint>(dimension), GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(index);
	this->unbind();
}



void VBO::make_from_vertex(const std::vector<Vertex>& data, const uint32 position_dimension) {
	constexpr GLsizei size = sizeof(Vertex);

	this->bind();

	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(data.size() * sizeof(Vertex)), data.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, static_cast<GLint>(position_dimension), GL_FLOAT, GL_FALSE, size, (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, size, (void*)offsetof(Vertex, texuv));
	glEnableVertexAttribArray(1);

	this->unbind();
}

//
// void VBO::make_from_vertex2d(const std::vector<Vertex2D>& data) {
// 	constexpr GLsizei size = sizeof(Vertex2D);
//
// 	this->bind();
//
// 	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(data.size() * sizeof(Vertex2D)), data.data(), GL_STATIC_DRAW);
//
// 	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, size, (void*)offsetof(Vertex2D, position));
// 	glEnableVertexAttribArray(0);
//
// 	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, size, (void*)offsetof(Vertex2D, texuv));
// 	glEnableVertexAttribArray(1);
//
// 	this->unbind();
// }

