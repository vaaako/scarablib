#pragma once

#include "scarablib/gfx/shape2d.hpp"
#include "scarablib/typedef.hpp"
#include "scarablib/types/color.hpp"
#include "scarablib/types/texture.hpp"
#include <string>
#include <SDL_ttf.h>

// Font object used to draw text on the screen
struct Font : public Shape2D {
	// Build a font object passing a path to a .ttf file.
	// Also pass font's text, size and color
	Font(const char* path, const uint16 size = 24, const TextureFilter filter = TextureFilter::LINEAR);
	~Font() {
		TTF_CloseFont(this->sdl_font);
	}

	// Draw font using a shader object
	void draw(const Shader& shader) override;

	// Change the font's text
	Font& set_text(const std::string& text);

	// Change font's color
	Font& set_color(const Color color);

	// Change font's size.
	// Don't use the method `set_size`, it will not work for fonts
	Font& set_font_size(const uint16 size);

	private:
		TTF_Font* sdl_font = TTF_OpenFont("", 0); // Init nothing so it can delete without check

		const char* path;
		GLenum filter;
		Texture* font_texture = new Texture(); // Track texture being used for font

		std::string text = "Sample Text";
		uint16 size;
		Color color = Colors::WHITE;

		// After an update, this need to run
		void update_texture();
};
