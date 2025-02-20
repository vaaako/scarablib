#include "scarablib/types/texture_array.hpp"
#include "scarablib/proper/error.hpp"
#include "scarablib/types/texture.hpp"

TextureArray::TextureArray(const std::vector<TextureArray::Config>& textures) {
	// Load first image to get dimensions
	Image* image = new Image(textures.front().path);
	if(image->data == nullptr) {
		throw ScarabError("Image (%s) was not found", image->path);
	}

	this->width  = static_cast<uint32>(image->width);
	this->height = static_cast<uint32>(image->height);

	// Generate and bind texture
	glGenTextures(1, &this->id);
	glBindTexture(GL_TEXTURE_2D_ARRAY, this->id);
	glTexStorage3D(
		GL_TEXTURE_2D_ARRAY, 1, Texture::extract_internal_format(*image),
		static_cast<GLint>(this->width), static_cast<GLint>(this->height),
		static_cast<GLint>(textures.size())
	);

	delete image; // Not used anymore

	// Load all frames
	for(size_t i = 0; i < textures.size(); i++) {
		const TextureArray::Config& conf = textures.at(i);
		Image* image = new Image(conf.path, conf.flip_vertically, conf.flip_horizontally);

		if(image->data == nullptr) {
			delete image;
			throw ScarabError("Image in texture array (%s) was not found", conf.path);
		}

		// Validate dimensions
		if(image->width != (int)this->width || image->height != (int)this->height) {
			throw ScarabError("Image (%s) has incorrect dimensions for texture array (%ix%i)", conf.path, this->width, this->height);

			// LOG_DEBUG("Resizing %i from (%ix%i) to (%ix%i)", i, image->width, image->height, this->width, this->height);
			// uint8* resized = new uint8[this->width * this->height * image->nr_channels];
			// stbir_resize_uint8_srgb(
			// 		image->data, image->width, image->height, image->width * image->nr_channels,
			// 		resized, this->width, this->height, this->width * image->nr_channels, STBIR_RGBA);
			// delete[] image->data;
			// image->data = resized;
		}

		// Upload to texture array layer i
		glTexSubImage3D(
			GL_TEXTURE_2D_ARRAY,
			0, // Mipmap level
			0, 0, static_cast<GLint>(i), // x, y, layer (z)
			static_cast<GLint>(this->width), static_cast<GLint>(this->height),
			1, // Depth
			Texture::extract_format(*image), GL_UNSIGNED_BYTE,
			image->data
		);

		delete image;
	}

	// Set texture parameters
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Unbind
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}

// TextureArray::TextureArray(const char* dir_path) {
// 	std::vector<std::string> files = FileHelper::list_files(dir_path, true);
// 	if(files.size() == 0) {
// 		throw ScarabError("Directory \"%s\" does not exist or is empty", dir_path);
// 	}
//
// 	// TODO: Make others
// }

TextureArray::~TextureArray() noexcept {
	glDeleteTextures(1, &this->id);
}

// void TextureArray::load_all_tex(const std::vector<const char*> paths) {
// 	// Load first image to get dimensions
// 	Image* image = new Image(paths.at(0));
//
// 	this->format = Texture::extract_data_format(*image);
// 	this->width  = static_cast<uint32>(image->width);
// 	this->height = static_cast<uint32>(image->height);
//
// 	// Generate and bind texture
// 	glGenTextures(1, &this->id);
// 	glBindTexture(this->tex_type, this->id);
// 	glTexStorage3D(
// 		GL_TEXTURE_2D_ARRAY, 1, this->format,
// 		static_cast<GLint>(this->width), static_cast<GLint>(this->height),
// 		static_cast<GLint>(paths.size())
// 	);
//
// 	delete image; // Not used anymore
//
// 	// Load all frames
// 	for(size_t i = 0; i < paths.size(); i++) {
// 		Image* image = new Image(paths.at(i));
//
// 		// Upload to texture array layer i
// 		glTexSubImage3D(
// 			GL_TEXTURE_2D_ARRAY,
// 			0, // Mipmap level
// 			0, 0, static_cast<GLint>(i), // x, y, layer (z)
// 			static_cast<GLint>(this->width), static_cast<GLint>(this->height),
// 			1, // Depth
// 			this->format, GL_UNSIGNED_BYTE,
// 			image->data
// 		);
//
// 		delete image;
// 	}
//
// 	// Set texture parameters
// 	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
// 	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
// 	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
// 	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//
// 	// Unbind
// 	glBindTexture(this->tex_type, 0);
// }
