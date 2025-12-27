#pragma once

#include "scarablib/camera/camera2d.hpp"
#include "scarablib/components/materialcomponent.hpp"
#include "scarablib/typedef.hpp"
#include <string>

// To avoid exporting stb_truetype header with the library
// struct _stbtt_bakedchar;

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

	private:
		static constexpr int atlas_height = 512;
		static constexpr int atlas_width  = 512;

		const Camera2D& camera;

		struct Glyph {
			vec2<float> position;
			vec2<float> texuv;
		};

		// Font
		void* cdata; // stbtt_bakedchar
		Glyph* buffer_data;
		size_t buffer_capacity;

		// Material of this mesh
		std::shared_ptr<MaterialComponent> material = std::make_shared<MaterialComponent>();

		// Bundle for VAO, VBO and EBO
		std::shared_ptr<VertexArray> vertexarray;
};


