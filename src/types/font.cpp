#include "scarablib/types/font.hpp"
#include "scarablib/opengl/shader.hpp"
#include "scarablib/proper/error.hpp"
#include "scarablib/proper/log.hpp"
#include "scarablib/scenes/camera2d.hpp"
#include "scarablib/utils/file.hpp"

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

// TODO: Optimize. More characters

Font::Font(const char* path, const uint16 size) {
	std::vector<uint8> buffer = FileHelper::read_binary_file(path);

	if(buffer.empty()) {
		throw ScarabError("Font file (%s) is invalid", path);
	} else if(buffer.size() < 4) {
		throw ScarabError("Font file (%s) is too small", path);
	}

	// Most TTF files start with 0x00010000 or 0x74727565
	uint32_t version = *reinterpret_cast<uint32_t*>(buffer.data());
	if (version != 0x00010000 && version != 0x74727565) {
		LOG_WARNING("Unexpected font version: %08x", version);
	}

	// TEST
	constexpr int atlas_width  = 512;
	constexpr int atlas_height = 512;

	std::vector<uint8> temp_bitmap = std::vector<uint8>(static_cast<size_t>(atlas_width * atlas_height));

	int result = stbtt_BakeFontBitmap(
		buffer.data(), 0,
		size,
		temp_bitmap.data(),
		atlas_width, atlas_height,
		32, 96, // ASCII 32 to 127, 96 printable ASCII
		this->cdata
	);

	if (result <= 0) {
		// Maximum size is 88
		throw ScarabError("Failed to bake font (%s) bitmap. Maybe the size is too big or too small", path);
	}

	this->buffer_capacity = 20;
	this->buffer_data = new Glyph[this->buffer_capacity * 6];

	// Create Texture from bitmap
	// NOTE: could use texture class here

	glGenTextures(1, &this->texid);
	glBindTexture(GL_TEXTURE_2D, this->texid);
	glTexImage2D(
		GL_TEXTURE_2D, 0,
		GL_RED,
		atlas_width, atlas_height, 0,
		GL_RED,
		GL_UNSIGNED_BYTE,
		temp_bitmap.data()
	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Store char data in map for easy access
	// for(int i = 0; i < 96; i++) {
	// 	this->char_data[static_cast<char>(i + 32)] = char_data[i];
	// }

	// Create VAO and VBO
	glGenVertexArrays(1, &this->vao);
	glGenBuffers(1, &this->vbo);

	// Bind VAO and VBO
	glBindVertexArray(this->vao);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);

	// Allocate memory
	glBufferData(GL_ARRAY_BUFFER, sizeof(Glyph) * 6 * this->buffer_capacity, 0, GL_DYNAMIC_DRAW);

	// Set VAO and VBO attributes
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Glyph), (void*)0);
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

void Font::add_text(const Camera2D& camera, const std::string& text, const vec2<float>& pos, const float scale) noexcept {
	glBindVertexArray(this->vao);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);

	// Resize the vector to fit all characters
	if(this->buffer_capacity < text.length()) {
		this->buffer_capacity = text.length();
		glBufferData(GL_ARRAY_BUFFER, sizeof(Glyph) * 6 * this->buffer_capacity, 0, GL_DYNAMIC_DRAW);
		delete[] this->buffer_data;
		this->buffer_data = new Glyph[this->buffer_capacity * 6];
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->texid);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(pos.x, pos.y, 0.0f));
	model = glm::scale(model, glm::vec3(scale, scale, 1.0f));

	Shader& shader = this->get_shader();
	shader.use();
	shader.set_color("shapeColor", Colors::YELLOW);
	shader.set_matrix4f("mvp", (camera.get_proj_matrix() * camera.get_view_matrix()) * model);

	// Remove const
	float x = pos.x;
	float y = pos.y;

	uint32 num_vertices = 0;
	// size_t vertex_index = 0;

	Glyph* data = this->buffer_data;
	for(char c : text) {
		// In range
		if(c < 32 || c > 128) {
			continue;
		}

		stbtt_aligned_quad q;
		stbtt_GetBakedQuad(this->cdata, 512, 512, c - 32, &x, &y, &q, 1);

		// Apply scale if needed
		// float x0 = q.x0 * scale;
		// float y0 = q.y0 * scale;
		// float x1 = q.x1 * scale;
		// float y1 = q.y1 * scale;

		data[0].position = vec2<float>(q.x0, q.y0);
		data[1].position = vec2<float>(q.x1, q.y0);
		data[2].position = vec2<float>(q.x1, q.y1);
		data[3].position = vec2<float>(q.x0, q.y1);
		data[4].position = vec2<float>(q.x0, q.y0);
		data[5].position = vec2<float>(q.x1, q.y1);

		data[0].texuv = vec2<float>(q.s0, q.t0); 
		data[1].texuv = vec2<float>(q.s1, q.t0);
		data[2].texuv = vec2<float>(q.s1, q.t1);
		data[3].texuv = vec2<float>(q.s0, q.t1);
		data[4].texuv = vec2<float>(q.s0, q.t0);
		data[5].texuv = vec2<float>(q.s1, q.t1);

		data += 6;
		num_vertices += 6;
	}

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Glyph) * num_vertices, this->buffer_data);
	glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(num_vertices));
}

