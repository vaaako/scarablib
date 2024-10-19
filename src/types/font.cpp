#include "scarablib/types/font.hpp"
#include "SDL.h"
#include "SDL_surface.h"
#include "SDL_ttf.h"
#include "scarablib/proper/log.hpp"
#include "scarablib/utils/sdl.hpp"
#include "scarablib/utils/string.hpp"

// Rectangle init doesn't matter
Font::Font(const char* path, const std::string& text, const uint16 size, const Color color, const TextureFilter filter)
	: Rectangle({ 0, 0 }, { 1.0f, 1.0f }), path(path), filter((GLenum)filter), text(text), size(size), color(color) {

	if(StringHelper::file_extension(path) != "ttf") {
		TTF_Quit();
		SDL_Quit();
		LOG_ERROR("Font format is not supported (%s) \nOnly .ttf format is supported", path);
		return;
	}

	// Check if exist
	if(TTF_OpenFont(path, size) == NULL) {
		TTF_Quit();
		SDL_Quit();
		LOG_ERROR("Failed to load \"%s\"", path);
		return;
	}

	// Config
	this->set_size(size);
	this->set_color(color);
	this->set_text(text);
	this->update_texture();
}

void Font::update_texture() {
	// Delete old sdl font (if have)
	TTF_CloseFont(this->sdl_font);

	// Make surface
	this->sdl_font = TTF_OpenFont(this->path, this->size);
	SDL_Surface* ttf_surface = TTF_RenderText_Blended(this->sdl_font, this->text.c_str(), { this->color.red, this->color.green, this->color.blue, this->color.alpha });

	// Correct surface to RGB format
	SDL_Surface* surface = SDLHelper::base2_surface(ttf_surface);
	SDLHelper::flip_vertically(surface);
	SDLHelper::flip_horizontally(surface);

	// Set size
	this->set_size({ static_cast<float>(surface->w), static_cast<float>(surface->h) });

	// Remove previous font's texture and set a new one
	delete this->font_texture;
	this->font_texture = new Texture(reinterpret_cast<uint8*>(surface->pixels), static_cast<uint32>(surface->w), static_cast<uint32>(surface->h), GL_RGBA);
	this->set_texture(this->font_texture);

	// Free surfaces
	SDL_FreeSurface(ttf_surface);
	SDL_FreeSurface(surface);
}

void Font::set_text(const std::string& text) {
	this->text = text;
	this->update_texture();
}

void Font::set_color(const Color color) {
	this->color = color;
	this->update_texture();
}

void Font::set_font_size(const uint16 size) {
	this->size = size;
	this->update_texture();
}
