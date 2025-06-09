#include "scarablib/gfx/3d/model.hpp"

Model::Model(const char* path) noexcept : Mesh(path) {}

void Model::set_rotation(const float angle, const vec3<bool>& axis) noexcept {
	// At least one axis need to be true to work
	if(axis == vec3<bool>(false)) {
		return; // Dont update
	}

	this->angle = angle;
	this->axis = axis;
	this->isdirty = true;
}

void Model::set_orientation(const float angle, const vec3<bool>& axis) noexcept {
	// At least one axis need to be true to work
	if(axis == vec3<bool>(false)) {
		return;
	}

	this->orient_angle = angle;
	this->orient_axis = axis;
	this->isdirty = true;
}


void Model::update_model_matrix() noexcept {
	if(!this->isdirty) {
		return;
	}

	this->model = glm::mat4(1.0f);

	// Calculate rotation matrices
	const glm::mat4 rotation    = glm::rotate(this->model, glm::radians(this->angle), this->axis);
	const glm::mat4 orientation = glm::rotate(this->model, glm::radians(this->orient_angle), this->orient_axis);

	// Modify in place
	this->model = glm::translate(this->model, this->position);
	this->model = glm::scale(this->model, this->scale);
	this->model *= orientation;
	this->model *= rotation;

	this->isdirty = false;

	// Update the bounding box in world space
	if(this->dynamic_bounding) {
		this->bbox.update_world_bounds(this->model);
	}
}

// I just need to provide the mvp just if any of the matrix changes, because the value is stored
// but i dont know how to do it currently (and i am lazy)
void Model::draw(const Camera& camera, const Shader& shader) noexcept {
	// Shader is binded outside for batch rendering

	this->update_model_matrix();

	// NOTE: is_dirty for color wouldn't work because would set this color to the next meshes
	shader.set_color("shapeColor", this->color);
	shader.set_matrix4f("mvp", (camera.get_proj_matrix() * camera.get_view_matrix()) * this->model);

	this->texture->bind();
	glDrawElements(GL_TRIANGLES, this->indices_length, this->indices_type, (void*)0);
	this->texture->unbind();
}


// TODO: Not working correctly and some changes needed
void Model::draw_collider(const Camera& camera, const Color& color, const bool stripped) noexcept {
	// Switch to fixed-function pipeline
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

	vec3<float>& max = this->bbox.max;
	vec3<float>& min = this->bbox.min;
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
