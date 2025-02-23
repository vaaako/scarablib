#pragma once

#include "scarablib/opengl/shader.hpp"
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
		void add_text(const std::string& text, const vec2<float>& pos, const float scale) noexcept;
		// Draw font using a shader object
		void draw_all() noexcept;

	private:
		struct Glyph {
			vec2<float> position;
			vec2<float> texuv;
		};

		// Font
		const char* path;
		uint16 fontsize;
		int atlas_width = 512;  // Internally int
		int atlas_height = 512; // Internally int
		
		std::vector<Glyph> chars;
		std::unordered_map<char, stbtt_bakedchar> char_data;

		// OpenGL
		GLuint texid;
		GLuint vao;
		GLuint vbo;

		static inline Shader& get_shader() noexcept {
			static Shader shader = Shader(
				FileHelper::read_file(THIS_FILE_DIR + "/../opengl/shaders/2d/vertex.glsl").c_str(),
				FileHelper::read_file(THIS_FILE_DIR + "/../opengl/shaders/2d/fragment.glsl").c_str()
			);

			return shader;
		}
};


