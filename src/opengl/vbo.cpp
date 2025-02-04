#include "scarablib/opengl/vbo.hpp"

VBO::VBO() noexcept {
	glGenBuffers(1, &this->id);
}

VBO::~VBO() noexcept {
	glDeleteBuffers(1, &this->id);
}

void VBO::alloc_data(const uint64 size, const void* data, const GLenum usage) const noexcept {
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(size), data, usage);
}

void VBO::link_attrib(const uint32 index, const uint32 size, const uint32 total_byte_size, const uint32 offset) const noexcept {
	glVertexAttribPointer(index, static_cast<GLint>(size), GL_FLOAT, GL_FALSE, static_cast<GLsizei>(total_byte_size), (void*)(offset * sizeof(float)));
	glEnableVertexAttribArray(index);
}


void VBO::alloc_data(const std::vector<float>& data, const GLenum usage) const noexcept {
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(data.size() * sizeof(float)), data.data(), usage);
}


void VBO::update_data(const uint64 size, const void* data, GLenum usage) const noexcept {
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(size), data, usage);
}

void VBO::make_from_vertex(const std::vector<Vertex>& data, const uint32 size, const GLenum usage) const noexcept {
	constexpr GLsizei vertexsize = sizeof(Vertex);

	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(data.size() * sizeof(Vertex)), data.data(), usage);

	glVertexAttribPointer(0, static_cast<GLint>(size), GL_FLOAT, GL_FALSE, vertexsize, (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, vertexsize, (void*)offsetof(Vertex, texuv));
	glEnableVertexAttribArray(1);
}


void VBO::make_from_vertices(const std::vector<float>& data, const uint32 size, const GLenum usage) const noexcept {
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(data.size() * sizeof(float)), data.data(), usage);

	glVertexAttribPointer(0, static_cast<GLint>(size), GL_FLOAT, GL_FALSE, static_cast<GLsizei>(size * sizeof(float)), (void*)0);
	glEnableVertexAttribArray(0);
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

