#include "scarablib/shapes/3d/mesh.hpp"
#include "scarablib/opengl/ebo.hpp"
#include "scarablib/opengl/vbo.hpp"

Mesh::Mesh() {}
Mesh::~Mesh() {}

void Mesh::init_buffers(const std::vector<float>& vertices, const std::vector<uint32>& indices) {
	this->get_vao().bind();

	// Gen VBO
	VBO vbo = VBO();
	vbo.alloc_data(vertices);
	vbo.link_attrib(0, 3);

	// Gen EBO
	EBO ebo = EBO(indices);

	// Unbind vao
	this->get_vao().unbind();
	vbo.unbind();
	ebo.unbind();

	this->indices_length = static_cast<uint32>(indices.size());
}

void Mesh::init_buffers(const std::vector<float>& vertices, const std::vector<float>& tex_coords, const std::vector<uint32>& indices) {
	this->get_vao().bind();

	// Gen VBO
	VBO vbo = VBO();
	vbo.alloc_data(vertices);
	vbo.link_attrib(0, 3);

	vbo.alloc_data(tex_coords);
	vbo.link_attrib(1, 2);

	// Gen EBO
	EBO ebo = EBO(indices);

	// Unbind vao
	this->get_vao().unbind();
	vbo.unbind();
	ebo.unbind();

	this->indices_length = static_cast<uint32>(indices.size());
}


void Mesh::init_buffers(const std::vector<Vertex>& data, const std::vector<uint32>& indices) {
	this->get_vao().bind();

	// Gen VBO
	VBO vbo = VBO();
	// Gen EBO
	EBO ebo = EBO(indices); // segfault here idk

	vbo.make_from_vertex(data, 3);

	// Unbind vao
	this->get_vao().unbind();
	vbo.unbind();
	ebo.unbind();

	this->indices_length = static_cast<uint32>(indices.size());
}

void Mesh::draw(const Camera& camera, const Shader& shader) {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Init matrices
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 proj = glm::mat4(1.0f);

	// Model matrix //
	model = glm::translate(model, glm::vec3(this->position.x, this->position.y, this->position.z));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(this->size.x, this->size.y, this->size.z)); // Resize

	// View matrix //
	view = glm::lookAt(camera.position, (camera.position + camera.orientation), camera.up);

	// Add perspective
	proj = glm::perspective(glm::radians(camera.fov),
			(static_cast<float>(camera.get_width()) / static_cast<float>(camera.get_height())), camera.near_plane, camera.far_plane);

	shader.use();
	// Apply changes
	shader.set_matrix4f("mvp", (proj * view) * model);
	shader.set_vector4f("shapeColor", { 1.0f, 1.0f, 1.0f, 1.0f });

	this->get_vao().bind();
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(this->indices_length), GL_UNSIGNED_INT, (void*)0);
	this->get_vao().unbind();
}
