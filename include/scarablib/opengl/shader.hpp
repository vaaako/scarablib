#pragma once

#include "scarablib/typedef.hpp"

// Struct used for Shader object
struct Shader {
	// Supported shader versions
	enum class Type {
		// Just used to check when using a custom shader 
		None     = 0,
		// Vertex Shader
		Vertex   = GL_VERTEX_SHADER,
		// Fragment Shader
		Fragment = GL_FRAGMENT_SHADER
	};

	uint32 id   = 0;
	Shader::Type type = Shader::Type::None;

	Shader(const char* source, const Shader::Type type);
	~Shader() noexcept;
};
