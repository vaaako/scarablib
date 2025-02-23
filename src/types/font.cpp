#include "scarablib/types/font.hpp"
#include "scarablib/opengl/shader.hpp"
#include "scarablib/proper/error.hpp"
#include "scarablib/proper/log.hpp"
#include "scarablib/utils/file.hpp"

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

Font::Font(const char* path, const uint16 size) : fontsize(size) {
	std::vector<uint8> buffer = FileHelper::read_binary_file(path);

	if(!FileHelper::file_exists(path)) {
		throw ScarabError("Font file (%s) is invalid", path);
	} else if(buffer.empty()) {
		throw ScarabError("Font file (%s) is invalid", path);
	} else if(buffer.size() < 4) {
		throw ScarabError("Font file (%s) is too small", path);
	}

	// Most TTF files start with 0x00010000 or 0x74727565
	uint32_t version = *reinterpret_cast<uint32_t*>(buffer.data());
	if (version != 0x00010000 && version != 0x74727565) {
		LOG_WARNING("Unexpected font version: %08x", version);
	}

	std::vector<uint8> temp_bitmap = std::vector<uint8>(static_cast<size_t>(this->atlas_width * this->atlas_height));

	this->char_data.reserve(128); // ASCII Characters


	stbtt_bakedchar char_data[128];
	stbtt_BakeFontBitmap(
		buffer.data(), 0,
		this->fontsize,
		temp_bitmap.data(),
		this->atlas_width, this->atlas_height,
		32, 96, // ASCII 32 to 127
		char_data
	);

	// Store char data in map for easy access
	for(int i = 0; i < 96; i++) {
		this->char_data[static_cast<char>(i + 32)] = char_data[i];
	}

	// Create Texture from bitmap
	// NOTE: could use texture class here

	glGenTextures(1, &this->texid);
	glBindTexture(GL_TEXTURE_2D, this->texid);
	glTexImage2D(
		GL_TEXTURE_2D, 0,
		GL_R8,
		this->atlas_width, this->atlas_height, 0,
		GL_RED,
		GL_UNSIGNED_BYTE,
		temp_bitmap.data()
	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Create VAO and VBO
	glGenVertexArrays(1, &this->vao);
	glGenBuffers(1, &this->vbo);

	// Bind VAO and VBO
	glBindVertexArray(this->vao);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);

	// Set VAO and VBO attributes
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Glyph), (void*)offsetof(Glyph, position));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Glyph), (void*)offsetof(Glyph, texuv));
	glEnableVertexAttribArray(1);

	// Unbind VAO and VBO
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Font::~Font() noexcept {
	glDeleteVertexArrays(1, &this->vao);
	glDeleteBuffers(1, &this->vbo);
	glDeleteTextures(1, &this->texid);
};

void Font::add_text(const std::string& text, const vec2<float>& pos, const float scale) noexcept {
	float start_x = pos.x;

	for(char c : text) {
		if(this->char_data.find(c) == this->char_data.end()) {
			continue;
		}

		stbtt_bakedchar& char_info = this->char_data[c];

		float x0 = start_x + char_info.xoff * scale;
		float y0 = pos.y + char_info.yoff * scale;
		float x1 = x0 + char_info.x1 - char_info.x0 * scale;
		float y1 = y0 + char_info.y1 - char_info.y0 * scale;

		float s0 = char_info.x0 / (float)this->atlas_width;
		float t0 = char_info.y0 / (float)this->atlas_height;
		float s1 = char_info.x1 / (float)this->atlas_width;
		float t1 = char_info.y1 / (float)this->atlas_height;

		// Add quad to batch
		Glyph vertices[6] = {
			{ vec2<float>(x0, y0), vec2<float>(s0, t0) },
			{ vec2<float>(x1, y0), vec2<float>(s1, t0) },
			{ vec2<float>(x0, y1), vec2<float>(s0, t1) },
			{ vec2<float>(x0, y1), vec2<float>(s0, t1) },
			{ vec2<float>(x1, y0), vec2<float>(s1, t0) },
			{ vec2<float>(x1, y1), vec2<float>(s1, t1) }
		};

		this->chars.insert(this->chars.end(), std::begin(vertices), std::end(vertices));

		start_x += char_info.xadvance * scale;
	}
}


void Font::draw_all() noexcept {
	glDepthFunc(GL_ALWAYS);
	if(this->chars.empty()) {
		return;
	}

	glBindVertexArray(this->vao);

	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glBufferData(GL_ARRAY_BUFFER, this->chars.size() * sizeof(Glyph),
		this->chars.data(), GL_DYNAMIC_DRAW);


	Shader& shader = this->get_shader();
	shader.use();
	shader.set_color("shapeColor", Colors::WHITE);

	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)this->chars.size());

	this->chars.clear();
	glBindVertexArray(0);
}
