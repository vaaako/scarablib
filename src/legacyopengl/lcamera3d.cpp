#include "scarablib/legacyopengl/lcamera3d.hpp"
#include "scarablib/window/window.hpp"
#include <GL/glu.h>

LCamera3D::LCamera3D(const Window& window, const float fov, const float sensitivity) noexcept
	: fov(fov), sensitivity(sensitivity), window(window) {};


void LCamera3D::begin() const noexcept {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(this->fov, (float)this->window.get_width() / (float)this->window.get_height(), this->znear, this->zfar);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Apply rotation
	glRotatef(this->pitch, 1.0f, 0.0f, 0.0f);
	glRotatef(this->yaw, 0.0f, 1.0f, 0.0f);
	// Move
	glTranslatef(this->position.x, this->position.y, this->position.z);
}
