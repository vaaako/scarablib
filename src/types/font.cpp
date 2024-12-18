#include "scarablib/types/font.hpp"
#include "scarablib/proper/log.hpp"
#include "scarablib/utils/sdl.hpp"
#include "scarablib/utils/string.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_surface.h>
#include <SDL_ttf.h>

// Shape2D init doesn't matter
Font::Font(const char* path, const uint16 size, const TextureFilter filter)
	: Shape2D({ vec2<float>(0.0f), vec2<float>(size) }), path(path), filter((GLenum)filter), size(size) {

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

	// Init font
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
	this->font_texture = new Texture(surface->pixels, static_cast<uint32>(surface->w), static_cast<uint32>(surface->h), surface->format->BytesPerPixel, GL_BGRA);
	this->set_texture(this->font_texture);

	// Free surfaces
	SDL_FreeSurface(ttf_surface);
	SDL_FreeSurface(surface);
}


void Font::draw(const Shader& shader) {
	// Apply transformations if needed
	this->update_model();

	shader.set_matrix4f("model", this->model);
	shader.set_color("shapeColor", this->color);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	this->texture->unbind();
}

Font& Font::set_text(const std::string& text) {
	this->text = text;
	this->update_texture();
	return *this;
}

Font& Font::set_color(const Color color) {
	this->color = color;
	this->update_texture();
	return *this;
}

Font& Font::set_font_size(const uint16 size) {
	this->size = size;
	this->update_texture();
	return *this;
}

