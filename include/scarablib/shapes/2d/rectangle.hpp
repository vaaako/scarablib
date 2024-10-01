#pragma once

#include "scarablib/opengl/ebo.hpp"
#include "scarablib/opengl/shader.hpp"
#include "scarablib/opengl/vao.hpp"
#include "scarablib/opengl/vbo.hpp"
#include "scarablib/proper/vector/vec2.hpp"
#include <vector>

struct Rectangle {
	const uint8 indices_count = 6;

	Rectangle();

	// Same VAO for all rectangles

	static bool buffers_initialized; // Flag to check if buffers are initialized
	static std::vector<glm::mat4> instance_matrices;


	void draw(const Shader& shader, const vec2<float>& position) const;

	// Delay initialization of OpenGL objects to when a OpenGL context is created
	// Objects will initialize when this method is called for the first time

	static inline VAO& get_vao() {
		static VAO vao; // Initialized once
		return vao;
	}

	static inline VBO& get_vbo() {
		static VBO vbo;
		return vbo;
	}

	static inline EBO& get_ebo() {
		static EBO ebo;
		return ebo;
	}


	// Build buffers
	static void initialize_buffers();

};


