#include "scarablib/types/texture_array.hpp"
#include "scarablib/proper/error.hpp"
#include "scarablib/types/texture.hpp"
#include "scarablib/utils/file.hpp"

TextureArray::TextureArray(const std::vector<const char*> paths) {
	// Load first image to get dimensions
	Image* image = new Image(paths.at(0));

	this->format = Texture::extract_data_format(*image);
	this->width  = static_cast<uint32>(image->width);
	this->height = static_cast<uint32>(image->height);

	// Generate and bind texture
	glGenTextures(1, &this->id);
	glBindTexture(this->tex_type, this->id);
	glTexStorage3D(
		GL_TEXTURE_2D_ARRAY, 1, this->format,
		static_cast<GLint>(this->width), static_cast<GLint>(this->height),
		static_cast<GLint>(paths.size())
	);

	delete image; // Not used anymore

	// Load all frames
	for(size_t i = 0; i < paths.size(); i++) {
		Image* image = new Image(paths.at(i));

		// Upload to texture array layer i
		glTexSubImage3D(
			GL_TEXTURE_2D_ARRAY,
			0, // Mipmap level
			0, 0, static_cast<GLint>(i), // x, y, layer (z)
			static_cast<GLint>(this->width), static_cast<GLint>(this->height),
			1, // Depth
			this->format, GL_UNSIGNED_BYTE,
			image->data
		);

		delete image;
	}

	// Set texture parameters
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Unbind
	glBindTexture(this->tex_type, 0);
}

TextureArray::TextureArray(const char* dir_path) {
	std::vector<std::string> files = FileHelper::list_files(dir_path, true);
	if(files.size() == 0) {
		throw ScarabError("Directory \"%s\" does not exist or is empty", dir_path);
	}
}

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
