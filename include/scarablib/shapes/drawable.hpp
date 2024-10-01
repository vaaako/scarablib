#pragma once

#include "scarablib/opengl/shader.hpp"
#include "scarablib/opengl/vao.hpp"
#include "scarablib/opengl/vbo.hpp"
#include "scarablib/proper/vector/vec2.hpp"
#include "scarablib/types/color.hpp"
#include "scarablib/types/vertex.hpp"
#include "scarablib/utils/file.hpp"

struct Drawable {
	Drawable();

	vec2<float> position = { 400.0f, 300.0f };
	vec2<float> size = { 50.0f, 50.0f };
	Color color = Colors::WHITE;

	void draw_rectangle();
	void draw_triangle();
	void draw_circle(const vec2<float>& pos, const float blur = 0.01f);

	private:
		VAO vao = VAO();

		Shader shader = Shader(
			FileHelper::read_file(SOURCE_DIR + "/../opengl/shaders/2d/vertex.glsl").c_str(),
			FileHelper::read_file(SOURCE_DIR + "/../opengl/shaders/2d/fragment.glsl").c_str()
		);

		Shader shader_circle = Shader(
			FileHelper::read_file(SOURCE_DIR + "/../opengl/shaders/2d/vertex.glsl").c_str(),
			FileHelper::read_file(SOURCE_DIR + "/../opengl/shaders/2d/circle_fragment.glsl").c_str()
		);
};
