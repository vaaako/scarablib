#include "scarablib/components/boundingbox.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>

BoundingBox::BoundingBox(const std::vector<Vertex>& vertices) noexcept {
	this->calculate_local_bounds(vertices);
}

void BoundingBox::calculate_local_bounds(const std::vector<Vertex>& vertices) noexcept {
	// Init with largest and smallest values
	this->local_min = vec3<float>(FLT_MAX);
	this->local_max = vec3<float>(-FLT_MAX);

	for(const Vertex& vertex : vertices) {
		this->local_min = glm::min(this->local_min, vertex.position);
		this->local_max = glm::max(this->local_max, vertex.position);
	}

	this->max = this->local_max;
	this->min = this->local_min;
}

void BoundingBox::update_world_bounds(const glm::mat4& model_matrix) noexcept {
	// Compute new world-space bounding box
	this->min = vec3<float>(FLT_MAX);
	this->max = vec3<float>(-FLT_MAX);

	for(const vec3<float>& corner : this->get_local_corners()) {
		const vec3<float> world_pos = model_matrix * vec4<float>(corner, 1.0f);
		this->min = glm::min(this->min, world_pos);
		this->max = glm::max(this->max, world_pos);
	}
}

/*
void AABB::update_world_bounds_fast(const glm::mat4& M) noexcept {
	const glm::vec3 center = (local_max + local_min) * 0.5f;
	const glm::vec3 extents = (local_max - local_min) * 0.5f;

	glm::vec3 world_center = glm::vec3(M * glm::vec4(center, 1.0f));

	glm::mat3 absM = glm::mat3(
		glm::abs(M[0]), glm::abs(M[1]), glm::abs(M[2])
	);

	glm::vec3 world_extents = absM * extents;

	min = world_center - world_extents;
	max = world_center + world_extents;
}
*/


bool BoundingBox::collides_with_sphere(const vec3<float>& center, const float radius) const noexcept {
	// Find closest point on AABB to sphere center
	const vec3<float> closest_point = glm::clamp(center, this->min, this->max);
	const float distance_squared = glm::length2(center - closest_point);
	return distance_squared <= (radius * radius);
}

void BoundingBox::draw_local_bounds(const Camera& camera, const Color& color, const bool stripped) const noexcept {
	this->draw(false, camera, color, stripped);
}

void BoundingBox::draw_world_bounds(const Camera& camera, const Color& color, const bool stripped) const noexcept {
	this->draw(true, camera, color, stripped);
}

void BoundingBox::draw(const bool world, const Camera& camera, const Color& color, const bool stripped) const noexcept {
	// Unbind any shader or vao
	glBindVertexArray(0);
	glUseProgram(0);

	// Update camera for legacy opengl
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadMatrixf(glm::value_ptr(camera.get_proj_matrix()));

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadMatrixf(glm::value_ptr(camera.get_view_matrix()));
	// glLoadMatrixf(glm::value_ptr(camera.get_view_matrix() * this->model));

	if(stripped) {
		glEnable(GL_LINE_STIPPLE);
		glLineStipple(1, 0x00FF);
	}

	glColor4f(color.red / 255.0f, color.green / 255.0f, color.blue / 255.0f, color.alpha / 255.0f);
	glLineWidth(1.0f);

	vec3<float> max = this->local_max;
	vec3<float> min = this->local_min;
	if(world) {
		max = this->max;
		min = this->min;
	}

	glBegin(GL_LINES);
		// Bottom face
		glVertex3f(min.x, min.y, min.z); glVertex3f(max.x, min.y, min.z);
		glVertex3f(max.x, min.y, min.z); glVertex3f(max.x, min.y, max.z);
		glVertex3f(max.x, min.y, max.z); glVertex3f(min.x, min.y, max.z);
		glVertex3f(min.x, min.y, max.z); glVertex3f(min.x, min.y, min.z);
		
		// Top face
		glVertex3f(min.x, max.y, min.z); glVertex3f(max.x, max.y, min.z);
		glVertex3f(max.x, max.y, min.z); glVertex3f(max.x, max.y, max.z);
		glVertex3f(max.x, max.y, max.z); glVertex3f(min.x, max.y, max.z);
		glVertex3f(min.x, max.y, max.z); glVertex3f(min.x, max.y, min.z);
		
		// Vertical edges
		glVertex3f(min.x, min.y, min.z); glVertex3f(min.x, max.y, min.z);
		glVertex3f(max.x, min.y, min.z); glVertex3f(max.x, max.y, min.z);
		glVertex3f(max.x, min.y, max.z); glVertex3f(max.x, max.y, max.z);
		glVertex3f(min.x, min.y, max.z); glVertex3f(min.x, max.y, max.z);
	glEnd();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	if(stripped) {
		glDisable(GL_LINE_STIPPLE);
	}
}
