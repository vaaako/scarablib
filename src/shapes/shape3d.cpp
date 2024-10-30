#include "scarablib/shapes/shape3d.hpp"

Shape3D::Shape3D(const Shape3DConf& conf, const VAO* vao, const uint32 indices_length)
	: Mesh(vao, indices_length),
	position(conf.position), size(conf.size), color(conf.color), angle(conf.angle) {}


void Shape3D::draw(const Camera& camera, const Shader& shader) {
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
	shader.set_vector4f("shapeColor", this->color.to_vec4<float>());

	this->texture->bind();
	this->vao->bind();

	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(this->indices_length), GL_UNSIGNED_INT, (void*)0);

	this->vao->unbind();
	this->texture->unbind();
}
