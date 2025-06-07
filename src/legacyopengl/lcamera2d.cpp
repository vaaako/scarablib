#include "scarablib/legacyopengl/lcamera2d.hpp"
#include "scarablib/window/window.hpp"
#include <GL/glu.h>

LCamera2D::LCamera2D(const Window& window, const float x, const float y, const float zoom) noexcept
	: x(x), y(y), zoom(zoom), window(window) {}

void LCamera2D::begin() const noexcept {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, this->window.get_width(), this->window.get_height(), 0.0f, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
