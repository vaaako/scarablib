#pragma once

#include "scarablib/opengl/shader.hpp"
#include "scarablib/opengl/vao.hpp"
#include "scarablib/opengl/vbo.hpp"
#include "scarablib/typedef.hpp"
#include "scarablib/types/color.hpp"
#include "scarablib/utils/file.hpp"
#include <unordered_map>
#include <string>
#include <vector>
#include <GL/glew.h>

struct Glyph {
	GLuint texture_id;
	vec2<uint32> size;
	vec2<uint32> bearing; // Offset from baseline to the left/top
	GLuint advance; // Horizontal offset to next glyph
	vec4<float> tex_coords;
};

// Font object used to draw text on the screen
class Font {
	// To update viewport
	friend class Scene2D;

	public:
		// Build a font object passing a path to a .ttf file.
		// Also pass font's text, size and color
		Font(const char* path, const uint16 size = 24);
		~Font();

		// Draw font using a shader object
		void draw_text(const std::string& text, const vec2<uint32> pos, const Color& color = Colors::WHITE, const float scale = 1.0f);

	private:
		// OpenGL
		GLuint texture_id;
		VAO* vao = new VAO();
		VBO* vbo = new VBO();

		// Font
		const char* path;
		std::unordered_map<char, Glyph> chars;

		// Update text
		std::vector<float> generate_text(const std::string& text, const float x, const float y, const float scale);

		static inline Shader& get_shader() {
			static Shader shader_circle = Shader(
				FileHelper::read_file(SOURCE_DIR + "/../opengl/shaders/2d/font_vertex.glsl").c_str(),
				FileHelper::read_file(SOURCE_DIR + "/../opengl/shaders/2d/font_fragment.glsl").c_str()
			);

			return shader_circle;
		}
};

