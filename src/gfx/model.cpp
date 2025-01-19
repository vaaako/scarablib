#include "scarablib/gfx/model.hpp"

Model::Model(const ModelConf& conf, const GLuint& vao_id, const std::vector<Vertex>& vertices, const std::vector<uint32>& indices)
	: Mesh(vao_id, vertices, indices), conf(conf) {
	this->conf.size = this->calc_size(this->vertices);
	this->update_min_and_max();
}

Model::Model(const char* path) : Mesh(path) {
	this->conf.size = this->calc_size(this->vertices);
	this->update_min_and_max();
}

Model& Model::set_rotation(const float angle, const vec3<bool> axis) {
	this->conf.angle = angle;
	// At least one axis need to be true to work
	if(axis == vec3<bool>(false)) {
		this->conf.axis = (vec3<float>)axis;
		return *this;
	}
	this->conf.axis = (vec3<float>)axis;
	this->isdirty = true;
	return *this;
}

Model& Model::set_orientation(const float angle, const vec3<bool> axis) {
	this->conf.orient_angle = angle;
	this->conf.orient_axis = (vec3<float>)axis;

	// At least one axis need to be true to work
	if(axis == vec3<bool>(false)) {
		return *this;
	}

	this->isdirty = true;
	return *this;
}

void Model::draw(Camera& camera, const Shader& shader) {
	if(this->isdirty) {
		this->model = glm::mat4(1.0f);

		// Model matrix
		this->model = glm::translate(this->model, this->conf.position)
					* glm::rotate(this->model, glm::radians(this->conf.orient_angle), this->conf.orient_axis)
					* glm::rotate(this->model, glm::radians(this->conf.angle), this->conf.axis)
					* glm::scale(this->model, this->conf.scale);

		this->isdirty = false;
	}
	
	// View matrix
	glm::mat4 view = camera.get_view_matrix();

	// Add perspective
	glm::mat4 proj = camera.get_proj_matrix();

	// NOTE: "is dirty" for color wouldn't work because would set the last color updated for all Modeles (using this later maybe)
	shader.set_color("shapeColor", this->conf.color);
	shader.set_matrix4f("mvp", (proj * view) * this->model);

	this->texture->bind();
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(this->indices_length), GL_UNSIGNED_INT, (void*)0);
	this->texture->unbind();
}


vec3<float> Model::calc_size(const std::vector<Vertex>& vertices) {
	// Init with largest and smallest values
	vec3<float> min = vec3<float>(FLT_MAX);
	vec3<float> max = vec3<float>(-FLT_MAX);

	for(const Vertex& vertex : vertices) {
		min = glm::min(min, vertex.position);
		max = glm::max(max, vertex.position);
	}

	return max - min;
}

