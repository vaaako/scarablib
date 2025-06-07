#include "scarablib/legacyopengl/lshapes.hpp"
#include "scarablib/window/window.hpp"

void LShapes::draw_rectangle(const vec2<float>& position, const vec2<float>& size) noexcept {
	glBegin(GL_QUADS);
		glVertex2f(position.x, position.y);
		glVertex2f(position.x + size.x, position.y);
		glVertex2f(position.x + size.x, position.y + size.y);
		glVertex2f(position.x, position.y + size.y);
	glEnd();
}

void LShapes::draw_cube(const vec3<float>& position, const float size, const float angle) noexcept {
	const float hs = size / 2.0f;

	// Save current transformation state
	glPushMatrix();

	// Move and rotate
	glTranslatef(position.x, position.y, position.z);
	glRotatef(angle, 1.0f, 1.0f, 0.0f);

	glBegin(GL_QUADS);
		// NOTE: Define color here to apply all faces

		// Front face
		glColor3f(1, 0, 0); // Red
		glVertex3f(-hs, -hs,  hs);
		glVertex3f( hs, -hs,  hs);
		glVertex3f( hs,  hs,  hs);
		glVertex3f(-hs,  hs,  hs);

		// Back face
		glColor3f(0, 1, 0); // Green
		glVertex3f(-hs, -hs, -hs);
		glVertex3f(-hs,  hs, -hs);
		glVertex3f( hs,  hs, -hs);
		glVertex3f( hs, -hs, -hs);

		// Left face
		glColor3f(0, 0, 1); // Blue
		glVertex3f(-hs, -hs, -hs);
		glVertex3f(-hs, -hs,  hs);
		glVertex3f(-hs,  hs,  hs);
		glVertex3f(-hs,  hs, -hs);

		// Right face
		glColor3f(1, 1, 0); // Yellow
		glVertex3f(hs, -hs, -hs);
		glVertex3f(hs,  hs, -hs);
		glVertex3f(hs,  hs,  hs);
		glVertex3f(hs, -hs,  hs);

		// Top face
		glColor3f(0, 1, 1); // Cyan
		glVertex3f(-hs, hs, -hs);
		glVertex3f(-hs, hs,  hs);
		glVertex3f( hs, hs,  hs);
		glVertex3f( hs, hs, -hs);

		// Bottom face
		glColor3f(1, 0, 1); // Magenta
		glVertex3f(-hs, -hs, -hs);
		glVertex3f( hs, -hs, -hs);
		glVertex3f( hs, -hs,  hs);
		glVertex3f(-hs, -hs,  hs);

	glEnd();

	glPopMatrix();
}
