#include "scarablib/shapes/2d/triangle.hpp"

Triangle::Triangle(const Shape2DConf& conf)
	: Shape2D(conf) {
}

void Triangle::draw(const Shader& shader) {
	// Apply transformations if needed
	this->update_model();

	shader.set_matrix4f("model", this->model);
	shader.set_vector4f("shapeColor", this->color.to_vec4<float>());
	glDrawArrays(GL_TRIANGLES, 4, 3);

	this->texture->unbind();
}