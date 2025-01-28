#include "scarablib/gfx/3d/plane.hpp"
#include "scarablib/gfx/model.hpp"

Plane::Plane(const ModelConf& conf) noexcept : Model(conf, this->get_vao().get_id(), {
		// Front face
		Vertex { .position = glm::vec3(-0.5f, -0.5f, 0.0f), .texuv = glm::vec2(0.0f, 0.0f) }, // Bottom left vertex
		Vertex { .position = glm::vec3( 0.5f, -0.5f, 0.0f), .texuv = glm::vec2(1.0f, 0.0f) }, // Bottom right vertex
		Vertex { .position = glm::vec3( 0.5f,  0.5f, 0.0f), .texuv = glm::vec2(1.0f, 1.0f) }, // Top right vertex
		Vertex { .position = glm::vec3(-0.5f,  0.5f, 0.0f), .texuv = glm::vec2(0.0f, 1.0f) }, // Top left vertex

		// Back face
		// Vertex { .position = glm::vec3(-0.5f, -0.5f, 0.0f), .texuv = glm::vec2(1.0f, 0.0f) }, // 4
		// Vertex { .position = glm::vec3(-0.5f,  0.5f, 0.0f), .texuv = glm::vec2(1.0f, 1.0f) }, // 5
		// Vertex { .position = glm::vec3( 0.5f,  0.5f, 0.0f), .texuv = glm::vec2(0.0f, 1.0f) }, // 6
		// Vertex { .position = glm::vec3( 0.5f, -0.5f, 0.0f), .texuv = glm::vec2(0.0f, 0.0f) }, // 7
	},
	{
		// Front face
		0, 1, 2,
		0, 2, 3,

		// Back face
		// 4, 5, 6,
		// 4, 6, 7,
	}) {}


void Plane::face_position(const vec3<float>& position, const vec3<float> axis) noexcept {
	// Distance from camera
	glm::vec3 direction = position - this->get_position();
	direction = glm::normalize(direction);

	// Set angle and ignore Y
	float angle = glm::degrees(atan2(direction.x, direction.z));

	// plane.set_rotation(angle, { 0.0f, 1.0f, 0.0f });
	this->set_orientation(angle, axis);
}
