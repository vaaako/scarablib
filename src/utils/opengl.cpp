#include "scarablib/utils/opengl.hpp"
#include <iostream>

void ScarabOpenGL::check_gl_error(const char* file, const int line) {
	GLenum error;
	while((error = glGetError()) != GL_NO_ERROR) {
		std::cerr << "OpenGL Error " << error << " at " << file << ":" << line << std::endl;
	}
}
