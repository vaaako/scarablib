#pragma once

#include "scarablib/camera/camera2d.hpp"
#include "scarablib/components/materialcomponent.hpp"
#include "scarablib/typedef.hpp"
#include <string>

// Forward declaration to avoid exporting stb_truetype header with the library
struct STBTruetype;


// Font object used to draw text on the screen
// WARNING: NOT FINISHED YET. STILL DONT WORKING PROPERLY
class Font {
	public:
		// Build a font object passing a path to a .ttf file.
		// Also pass font's text, size and color
		Font(const Camera2D& camera, const char* path, const uint16 size = 24);
		~Font() noexcept;

		// Disable copying to prevent double-free pointer bugs
		Font(const Font&) = delete;
		Font& operator=(const Font&) = delete;

		// Add text to batch rendering
		void draw_text(const std::string& text, const vec2<float>& pos, const float scale = 1.0f, const Color& color = Colors::WHITE) noexcept;

	private:
		static constexpr int ATLAS_WIDTH  = 512;
		static constexpr int ATLAS_HEIGHT = 512;

		const Camera2D& camera;

		// Font
		STBTruetype* data; // Hidden STB data
		Vertex2D* buffer_data = nullptr;
		size_t buffer_capacity = 0;

		std::shared_ptr<MaterialComponent> material = std::make_shared<MaterialComponent>();
		std::shared_ptr<VertexArray> vertexarray;
};


