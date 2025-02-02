#include "scarablib/gfx/3d/plane.hpp"
#include "scarablib/gfx/model.hpp"

Plane::Plane(const Model::Config& conf, const std::vector<Vertex>& vertices, const std::vector<uint32>& indices) noexcept
	: Model(conf, vertices, indices) {}

void Plane::face_position(const vec3<float>& position, const vec3<float> axis) noexcept {
	// Distance from camera
	glm::vec3 direction = glm::normalize(position - this->get_position());

	// Set angle and ignore Y
	float angle = glm::degrees(atan2(direction.x, direction.z));

	// plane.set_rotation(angle, { 0.0f, 1.0f, 0.0f });
	this->set_orientation(angle, axis);
}
