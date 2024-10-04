#include "scarablib/shapes/2d/circle.hpp"

Circle::Circle(const vec2<float>& position, const vec2<float>& size, const Color& color, const float angle)
	: Shape2D(position, size, color, angle) {}

void Circle::draw(const Shader& shader) const {
	// Apply transformations
	if(this->isdirty) {
		this->model = glm::mat4(1.0f);
		this->model = glm::translate(this->model, glm::vec3(this->position.x, this->position.y, 0.0f))
					* glm::scale(this->model, glm::vec3(this->size.x, this->size.y, 1.0f));
		this->isdirty = false;
	}

	shader.set_matrix4f("model", this->model);
	shader.set_vector4f("shapeColor", this->color.to_vec4<float>());
	shader.set_float("blur", this->blur);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}
