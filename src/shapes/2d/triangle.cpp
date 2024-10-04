#include "scarablib/shapes/2d/triangle.hpp"

Triangle::Triangle(const vec2<float>& position, const vec2<float>& size, const Color& color, const float angle)
	: Shape2D(position, size, color, angle) {
}

void Triangle::draw(const Shader& shader) const {
	// Apply transformations
	if(true) {
		this->model = glm::mat4(1.0f);
		this->model = glm::translate(this->model, glm::vec3(this->position.x, this->position.y, 0.0f))
					* glm::scale(this->model, glm::vec3(this->size.x, this->size.y, 1.0f));
		this->isdirty = false;
	}

	shader.set_matrix4f("model", this->model);
	shader.set_vector4f("shapeColor", this->color.to_vec4<float>());
	glDrawArrays(GL_TRIANGLES, 4, 3);
}
