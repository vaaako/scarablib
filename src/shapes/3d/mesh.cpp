#include "scarablib/shapes/3d/mesh.hpp"
#include "scarablib/opengl/vao.hpp"

Mesh::Mesh(const VAO* vao, const uint32 indices_length)
	: vao(vao), indices_length(indices_length) {}

// Mesh::Mesh(const std::vector<Vertex>& data, const std::vector<uint32>& indices)
// 	: indices_length(static_cast<uint32>(indices.size())) {
//
// 	VAO vao = VAO();
// 	vao.bind();
//
// 	// Gen VBO
// 	VBO vbo = VBO();
// 	// Gen EBO
// 	EBO ebo = EBO(indices); // segfault here idk
//
// 	vbo.make_from_vertex(data, 3);
//
// 	// Unbind vao
// 	vao.unbind();
// 	vbo.unbind();
// 	ebo.unbind();
// }

