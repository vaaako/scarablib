#include "scarablib/gfx/texture_array.hpp"
#include "scarablib/proper/error.hpp"

TextureArray::TextureArray(const uint32 width, const uint32 height, const uint32 num_textures, const Texture::Filter filter)
	: width(static_cast<GLint>(width)), height(static_cast<GLint>(height)), max_textures(num_textures) {

	GLint max_layers;
	glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &max_layers);
	if(num_textures > static_cast<uint32>(max_layers)) {
		throw ScarabError("You passed a number of textures of %i, but the limit is %i", num_textures, max_layers);
	}

	// Generate and bind texture
	glGenTextures(1, &this->id);
	glBindTexture(GL_TEXTURE_2D_ARRAY, this->id);
	glTexStorage3D(
		GL_TEXTURE_2D_ARRAY,
		1, // Mipmap level
		GL_RGBA8,
		static_cast<GLint>(width), static_cast<GLint>(height),
		static_cast<GLint>(num_textures)
	);

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(filter));
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(filter));
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

TextureArray::TextureArray(const std::vector<TextureArray::Config>& textures, const Texture::Filter filter, const uint32 num_textures) {
	// Load first image to get dimensions
	Image* image = new Image(textures.front().path);
	if(image->data == nullptr) {
		throw ScarabError("Image (%s) was not found", image->path);
	}

	this->max_textures = (num_textures != 0)
		? num_textures
		: static_cast<uint32>(textures.size());

	if(max_textures > textures.size()) {
		throw ScarabError("Texture array limit (%d) is smaller than the number of textures (%d)", max_textures, textures.size());
	}

	GLint max_layers;
	glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &max_layers);
	if(max_textures > static_cast<uint32>(max_layers)) {
		throw ScarabError("The maximum number of textures in a texture array is %d, but the limit is %d", max_textures, max_layers);
	}

	this->width  = image->width;
	this->height = image->height;

	// Generate and bind texture
	glGenTextures(1, &this->id);
	glBindTexture(GL_TEXTURE_2D_ARRAY, this->id);
	glTexStorage3D(
		GL_TEXTURE_2D_ARRAY, 1,
		Texture::extract_format(*image, true),
		image->width, image->height,
		static_cast<GLint>(max_textures)
	);

	delete image; // Not used anymore

	// Load all frames
	for(size_t i = 0; i < textures.size(); i++) {
		const TextureArray::Config& conf = textures.at(i);
		Image* image = new Image(conf.path, conf.flip_vertically, conf.flip_horizontally);

		if(image->data == nullptr) {
			throw ScarabError("Image in texture array (%s) was not found", conf.path);
		}

		// Validate dimensions
		if(image->width != (int)this->width || image->height != (int)this->height) {
			delete image;
			throw ScarabError("Image (%s) has incorrect dimensions for texture array (%ix%i)", conf.path, this->width, this->height);
		}

		// Upload to texture array layer i
		glTexSubImage3D(
			GL_TEXTURE_2D_ARRAY,
			0, // Mipmap level
			0, 0, static_cast<GLint>(i), // x, y, layer (z)
			image->width, image->height,
			1, // Depth
			Texture::extract_format(*image, false),
			GL_UNSIGNED_BYTE,
			image->data
		);

		delete image;
	}

	// Current Z-index of the latest added texture
	this->cur_index = static_cast<GLint>(textures.size());

	// Set texture parameters
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(filter));
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(filter));
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);


	// Generate mipmaps if max textures is reached
	if(this->max_textures == textures.size()) {
		glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
	}

	// Unbind
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}

void TextureArray::add_texture(const TextureArray::Config& config) {
	// Check if limit has been reached
	if(this->cur_index >= static_cast<GLint>(this->max_textures)) {
		throw ScarabError("Texture array limit (%d) has been reached", this->max_textures);
	}

	// Load texture
	Image* image = new Image(config.path, config.flip_vertically, config.flip_horizontally);
	if(image->data == nullptr) {
		throw ScarabError("Image (%s) was not found", image->path);
	}

	// Validate dimensions
	if((image->width != (int)this->width) || (image->height != (int)this->height)) {
		delete image;
		throw ScarabError("Image (%s) has incorrect dimensions for texture array (%ix%i)", config.path, this->width, this->height);
	}

	// Bind
	glBindTexture(GL_TEXTURE_2D_ARRAY, this->id);

	// Upload to texture array
	glTexSubImage3D(
		GL_TEXTURE_2D_ARRAY,
		0, // Mipmap level
		0, 0, this->cur_index, // x, y, layer (z)
		static_cast<GLint>(this->width), static_cast<GLint>(this->height),
		1, // Depth
		// Texture::extract_format(*image),
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		image->data
	);

	// Increment index
	this->cur_index++;

	// Unbind
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

	// Cleanup
	delete image;
}

TextureArray::~TextureArray() noexcept {
	glDeleteTextures(1, &this->id);
}

