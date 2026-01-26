#include "scarablib/geometry/sprite.hpp"
#include "scarablib/geometry/mesh.hpp"

Sprite::Sprite(const std::vector<Vertex2D>& vertices) noexcept
	: Mesh(vertices) {
	// Position and TexUV
	this->vertexarray->add_attribute<float>(2, false);
	this->vertexarray->add_attribute<float>(2, false);

	// Set 2D Shader
	std::shared_ptr<ShaderProgram> shader = ResourcesManager::get_instance().load_shader_program({
		// Default vertex and fragment shader source
		{ .source = Shaders::DEFAULT_VERTEX2D,   .type = Shader::Type::Vertex },
		{ .source = Shaders::DEFAULT_FRAGMENT, .type = Shader::Type::Fragment },
	});
}

void Sprite::update_model_matrix() noexcept {
	// Just update if is dirty
	if(!this->isdirty) {
		return;
	}

	this->model = glm::mat4(1.0f);

	// Modify in place
	this->model = glm::translate(this->model, glm::vec3(this->position->x, this->position->y, 0.0f));
	// Origin from top-lef to center, to apply rotation
	this->model = glm::translate(this->model, glm::vec3(0.5f * this->size->x, 0.5f * this->size->y, 0.0f));
	// Rotate
	this->model = glm::rotate(this->model, glm::radians(static_cast<float>(this->angle)), glm::vec3(0.0f, 0.0f, 1.0f));
	// Origin back to top-left
	this->model = glm::translate(this->model, glm::vec3(-0.5f * this->size->x, -0.5f * this->size->y, 0.0f));
	this->model = glm::scale(this->model, glm::vec3(this->size->x, this->size->y, 0.0f));

	this->isdirty = false;

	// TODO: Collision calculation will be here
}

// I could just provide mvp if any of the matrix changes, because the value is stored in memory.
// But i just know when model changes, and camera matrix is likely to change everytime
void Sprite::draw_logic() noexcept {
	this->update_model_matrix();

	// this->material->shader->set_matrix4f("mvp", (camera.get_proj_matrix() * camera.get_view_matrix()) * this->model);

	// Not indices btw
	glDrawArrays(GL_TRIANGLE_FAN, 0, this->vertexarray->get_length());
}

