#include "scarablib/gfx/3d/plane.hpp"
#include "scarablib/gfx/model.hpp"

Plane::Plane(const Model::Config& conf, const std::vector<Vertex>& vertices, const std::vector<uint32>& indices) noexcept
	: Model(conf, vertices, indices) {}

void Plane::face_position(const vec3<float>& focus_position, const vec3<float>& axis) noexcept {
	// Distance from the camera to the plane
	glm::vec3 direction = focus_position - this->get_position();
	direction = glm::normalize(direction); // Normalize the direction vector

	// Calculate the rotation angle to face the camera on the Y-axis (horizontal plane)
	float angle = glm::degrees(atan2(direction.x, direction.z)); // Ignore the Y-axis for rotation

	// Set the orientation of the plane to face the camera
	this->set_orientation(angle, axis); // axis is typically { 0.0f, 1.0f, 0.0f } for billboarding
}
