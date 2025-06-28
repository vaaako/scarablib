#pragma once

#include "scarablib/opengl/shader.hpp"
#include "scarablib/opengl/shader_manager.hpp"
#include "scarablib/opengl/shaders.hpp"
#include "scarablib/camera/camera2d.hpp"
#include "scarablib/typedef.hpp"
#include <string>
#include <GL/glew.h>

// Font object used to draw text on the screen
// WARNING: NOT FINISHED YET. STILL DONT WORKING PROPERLY
class Font {
	public:
		// Build a font object passing a path to a .ttf file.
		// Also pass font's text, size and color
		Font(const Camera2D& camera, const char* path, const uint16 size = 24);
		~Font() noexcept;

		// Add text to batch rendering
		void draw_text(const std::string& text, const vec2<float>& pos, const float scale = 1.0f, const Color& color = Colors::WHITE) noexcept;

		inline Shader& get_shader() const noexcept {
			return *this->shader;
		}

	private:
		const Camera2D& camera;
		static constexpr int atlas_height = 512;
		static constexpr int atlas_width  = 512;

		struct Glyph {
			vec2<float> position;
			vec2<float> texuv;
		};

		// Font
		// If i declare cdata as stbtt_bakedchar in header file
		// i will have to include stb_truetype.h and when sharing the library i would have to also share stb_truetype.h
		// which i dont want
		void* cdata; // stbtt_bakedchar
		Glyph* buffer_data;
		size_t buffer_capacity;

		// OpenGL
		GLuint texid;
		GLuint vao;
		GLuint vbo;

		Shader* shader = ShaderManager::get_instance().get_or_load_shader(
			"font",
			Shaders::DEFAULT_VERTEX,
			Shaders::FONT_FRAGMENT
		);
};


