#pragma once

#include "scarablib/opengl/shader.hpp"
#include "scarablib/opengl/vao.hpp"
#include "scarablib/opengl/vbo.hpp"
#include "scarablib/typedef.hpp"
#include "scarablib/types/color.hpp"
#include "scarablib/utils/file.hpp"
#include <unordered_map>
#include <string>
#include <GL/glew.h>

// Font object used to draw text on the screen
class Font {
	// To update viewport
	friend class Scene2D;

	// WARNING: FONTS ARE NOT COMPLETED YET, I WILL UPDATE IT LATER

	public:
		// Build a font object passing a path to a .ttf file.
		// Also pass font's text, size and color
		Font(const char* path, const uint16 size = 24);
		~Font() noexcept;

		// Draw font using a shader object
		void draw_text(const std::string& text, const vec2<uint32>& pos, const Color& color = Colors::WHITE, float scale = 1.0f) noexcept;

	private:
		struct Glyph {
			GLuint texture_id; // Index of the glyph in the texture map
			vec2<uint32> size;
			vec2<uint32> bearing; // Offset from baseline to the left/top
			uint32 advance; // Horizontal offset to next glyph
		};

		// OpenGL
		GLuint texture_array;
		GLuint vao;
		GLuint vbo;

		// Font
		const char* path;
		std::vector<Glyph> chars;
		uint64 char_max;

		static inline Shader& get_shader() {
			static Shader shader_circle = Shader(
				FileHelper::read_file(THIS_FILE_DIR + "/../opengl/shaders/2d/font_vertex.glsl").c_str(),
				FileHelper::read_file(THIS_FILE_DIR + "/../opengl/shaders/2d/font_fragment.glsl").c_str()
			);

			return shader_circle;
		}
};


