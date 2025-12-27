#pragma once

#include "scarablib/typedef.hpp"
#include <algorithm>
#include <variant>
#include <vector>

namespace ScarabOpenGL {
	// Get a GLenum from a type
	template<typename T>
	static constexpr GLenum gl_type();

	// Converts a vector type to another
	template <typename T, typename U>
	std::vector<T> convert_to(const std::vector<U>& vec) {
		std::vector<T> output = std::vector<T>(vec.size());
		std::transform(vec.begin(), vec.end(), output.begin(), [](U v) {
			return static_cast<T>(v);
		});
		return output;
	}

	// Used on GL_CHECK macro
	void check_gl_error(const char* file, int line);
};

#define GL_CHECK() ScarabOpenGL::check_gl_error(__FILE__, __LINE__)


template<> constexpr GLenum ScarabOpenGL::gl_type<float>()  { return GL_FLOAT; }
template<> constexpr GLenum ScarabOpenGL::gl_type<double>() { return GL_DOUBLE; }
template<> constexpr GLenum ScarabOpenGL::gl_type<uint8>()  { return GL_UNSIGNED_BYTE; }
template<> constexpr GLenum ScarabOpenGL::gl_type<int8>()   { return GL_BYTE; }
template<> constexpr GLenum ScarabOpenGL::gl_type<uint16>() { return GL_UNSIGNED_SHORT; }
template<> constexpr GLenum ScarabOpenGL::gl_type<int16>()  { return GL_SHORT; }
template<> constexpr GLenum ScarabOpenGL::gl_type<uint32>() { return GL_UNSIGNED_INT; }
template<> constexpr GLenum ScarabOpenGL::gl_type<int32>()  { return GL_INT; }
