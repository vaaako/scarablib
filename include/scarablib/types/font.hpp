#pragma once

#include "scarablib/opengl/shader.hpp"
#include "scarablib/scenes/camera2d.hpp"
#include "scarablib/typedef.hpp"
#include "scarablib/utils/file.hpp"
#include <string>
#include <GL/glew.h>

#include <stb_truetype.h>

// Font object used to draw text on the screen
class Font {
	// To update viewport
	friend class Scene2D;

	// WARNING: NOT FINISHED YET. STILL DONT WORKING PROPERLY

	public:
		// Build a font object passing a path to a .ttf file.
		// Also pass font's text, size and color
		Font(const Camera2D& camera, const char* path, const uint16 size = 24);
		~Font() noexcept;

		// Add text to batch rendering
		void draw_text(const std::string& text, const vec2<float>& pos, const float scale = 1.0f, const Color& color = Colors::WHITE) noexcept;

	private:
		const Camera2D& camera;
		static constexpr int atlas_height = 512;
		static constexpr int atlas_width  = 512;

		struct Glyph {
			vec2<float> position;
			vec2<float> texuv;
		};

		// Font
		stbtt_bakedchar cdata[96];
		Glyph* buffer_data;
		size_t buffer_capacity;

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


