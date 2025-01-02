#include "scarablib/types/font.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "scarablib/opengl/vbo.hpp"
#include "scarablib/proper/error.hpp"
#include "scarablib/proper/log.hpp"
#include "scarablib/utils/string.hpp"

#include <freetype/ft2build.h>
#include FT_FREETYPE_H

// NOTE: I wouldn't get to this point without the help of https://www.youtube.com/@WhateversRightStudios

Font::Font(const char* path, const uint16 size) {
	if(StringHelper::file_extension(path) != "ttf") {
		throw ScarabError("Font [%s] is not supported. Only .ttf format is supported supported", path);
	}

	FT_Library ft;
	if(FT_Init_FreeType(&ft)) {
		throw ScarabError("Could not initialize FreeType for some reason");
	}

	FT_Face face;
	if(FT_New_Face(ft, path, 0, &face)) {
		throw ScarabError("Font [%s] was not found!", path);
	}


	FT_Set_Pixel_Sizes(face, 0, size);
	FT_Select_Charmap(face, FT_ENCODING_UNICODE);

	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// NOTE: 128 is the default characters range
	// - 256: Latin-1 Supplement range
	for(wchar_t c = 0; c < face->num_glyphs; c++){
		// wchar_t wchar = *c;
		if(FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			LOG_ERROR("Failed to load glyph '%s'", c);
			continue;
		}
#ifdef SCARAB_DEBUG_GLYPHS_LOADED
		LOG_INFO("Loaded glyph '%c' with size (%d, %d)", c, face->glyph->bitmap.width, face->glyph->bitmap.rows);
#endif

		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
			face->glyph->bitmap.width, face->glyph->bitmap.rows,
			0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		this->chars.insert(std::pair<char, Glyph>(c, Glyph {
			texture,
			vec2<uint32>(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			vec2<uint32>(face->glyph->bitmap_left, face->glyph->bitmap_top),
			static_cast<GLuint>(face->glyph->advance.x)
		}));
	}

	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	float vertex_data[] = {
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 0.0f
	};

	// Configure VAO and VBO
	this->vbo->bind();
	this->vao->bind();

	this->vbo->alloc_data(sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
	this->vbo->link_attrib(0, 2, 0, 0);

	this->vbo->unbind();
	this->vao->unbind();
}

Font::~Font() {
	for (auto& pair : this->chars) {
		glDeleteTextures(1, &pair.second.texture_id);
	}
	delete this->vbo;
	delete this->vao;
};

// TODO: One drawcall
void Font::draw_text(const std::string& text, const vec2<uint32>& pos, const Color& color, const float scale) {
	// glDepthFunc(GL_LEQUAL);

	Shader& shader = this->get_shader();
	shader.use();
	shader.set_color("shapeColor", color);
	this->vao->bind();

	float cur_x  = static_cast<float>(pos.x);
	float cur_y  = static_cast<float>(pos.y);
	float init_x = static_cast<float>(pos.x);

	for(const char c : text) {
		Glyph& ch = this->chars.at(c);

		// Down one line
		if(c == '\n') {
			cur_x = init_x; // Reset x
			// Move down (spacing: 1.3)
			cur_y -= static_cast<float>((ch.size.y) * 1.3) * scale;
			continue;

		// Ignore space
		} else if(c == ' ') {
			cur_x += static_cast<float>(ch.advance >> 6) * scale;
			continue;
		}

		const float xpos = cur_x + static_cast<float>(ch.bearing.x) * scale;
		const float ypos = cur_y - static_cast<float>((ch.size.y - ch.bearing.y)) * scale;

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(xpos, ypos, 0))
			* glm::scale(model, glm::vec3(static_cast<float>(ch.size.x) * scale, static_cast<float>(ch.size.y) * scale, 0));
		shader.set_matrix4f("model", model);

		glBindTexture(GL_TEXTURE_2D, ch.texture_id);
		this->vbo->bind();
		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, 1); // 1 instance of 4 vertices

		cur_x += static_cast<float>(ch.advance >> 6) * scale; // The advance is in 1/64th of a pixel
	}

	this->vbo->unbind();
}

