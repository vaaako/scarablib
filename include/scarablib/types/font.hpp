#pragma once

#include "scarablib/opengl/shader.hpp"
#include "scarablib/scenes/camera2d.hpp"
#include "scarablib/typedef.hpp"
#include "scarablib/utils/file.hpp"
#include <unordered_map>
#include <string>
#include <GL/glew.h>

#include <stb_truetype.h>

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

		// Add text to batch rendering
		void add_text(const Camera2D& camera, const std::string& text, const vec2<float>& pos, const float scale) noexcept;
		// Draw font using a shader object
		void draw_all() noexcept;

	private:
		struct Glyph {
			vec2<float> position;
			vec2<float> texuv;
		};

		// Font
		stbtt_bakedchar cdata[96];
		Glyph* buffer_data;
		uint32 buffer_capacity;

		// OpenGL
		GLuint texid;
		GLuint vao;
		GLuint vbo;

		static inline Shader& get_shader() noexcept {
			static Shader shader = Shader(
				FileHelper::read_file("resources/shaders/vertex.glsl").c_str(),
				FileHelper::read_file("resources/shaders/2d/font_fs.glsl").c_str()
			);

			return shader;
		}
};


