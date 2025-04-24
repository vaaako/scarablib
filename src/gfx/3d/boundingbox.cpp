#include "scarablib/gfx/3d/boundingbox.hpp"

BoundingBox::BoundingBox(const std::vector<Vertex>& vertices) noexcept {
	this->calculate_local_bounds(vertices);
}

void BoundingBox::calculate_local_bounds(const std::vector<Vertex>& vertices) noexcept {
	// Init with largest and smallest values
	this->min = vec3<float>(FLT_MAX);
	this->max = vec3<float>(-FLT_MAX);

	for(const Vertex& vertex : vertices) {
		this->min = glm::min(this->min, vertex.position);
		this->max = glm::max(this->max, vertex.position);
	}

	this->size = this->max - this->min;
}

void BoundingBox::update_world_bounds(const glm::mat4& model_matrix) noexcept {
	// Transform all 8 corners of the local bounding box
	std::vector<vec3<float>> corners = this->get_bounding_corners();

	// Compute new world-space bounding box
	this->min = vec3<float>(FLT_MAX);
	this->max = vec3<float>(-FLT_MAX);

	for(const vec3<float>& corner : corners) {
		vec3<float> world_pos = model_matrix * vec4<float>(corner, 1.0f);
		this->min = glm::min(this->min, world_pos);
		this->max = glm::max(this->max, world_pos);
	}
}

