#include "scarablib/gfx/font.hpp"
#include "scarablib/gfx/texture.hpp"
#include "scarablib/opengl/assets.hpp"
#include "scarablib/opengl/shader_program.hpp"
#include "scarablib/proper/error.hpp"
#include "scarablib/camera/camera2d.hpp"
#include "scarablib/utils/file.hpp"
#include "scarablib/utils/hash.hpp"

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb/stb_truetype.h>

struct STBTruetype {
	std::vector<stbtt_bakedchar> cdata;
	STBTruetype() noexcept : cdata(96) {} // ASCII 32-127
};

Font::Font(const Camera2D& camera, const char* path, const uint16 size)
	: camera(camera), data(new STBTruetype()) {
	const char* source = R"glsl(
		void mainImage(out vec4 fragcolor, in vec2 texuv) {
			// Sample red channel (glyph alpha)
			vec4 tex = vec4(texture(texSampler, texuv).r);
			fragcolor = vec4(tex * shapeColor);
		}
	)glsl";

	this->material->shader = ResourcesManager::get_instance().load_shader_program({
		ResourcesManager::ShaderInfo {
			.source = Shaders::DEFAULT_VERTEX,
			.type   = Shader::Type::Vertex
		},
		ResourcesManager::ShaderInfo {
			.source   = source,
			.type     = Shader::Type::Fragment,
			.iscustom = true
		},
	});

	// Load font file
	std::vector<uint8> buffer = ScarabFile::read_binary_file(path);
	if(buffer.empty()) {
		throw ScarabError("Font file (%s) is invalid", path);
	}

	// Bake bitmap
	std::vector<uint8> temp_bitmap =
		std::vector<uint8>(this->ATLAS_WIDTH * this->ATLAS_HEIGHT);

	int result = stbtt_BakeFontBitmap(
		buffer.data(), 0, (float)size,
		temp_bitmap.data(), this->ATLAS_WIDTH, this->ATLAS_HEIGHT,
		32, 96, // ASCII 32 to 127, 96 printable ASCII
		this->data->cdata.data()
	);

	if(result <= 0) {
		throw ScarabError("Failed to bake font (%s). Atlas too small?", path);
	}

	// Texture from bitmap
	// 1-byte alignment for grayscale
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	Image tmpimg = Image(temp_bitmap.data(), this->ATLAS_WIDTH, this->ATLAS_HEIGHT, 1);
	tmpimg.owns_data = false; // Disables free
	// This avoids double free (inside Image and end of this method)
	this->material->texture = Assets::load(tmpimg);

	// Buffer setup
	this->buffer_capacity = 128; // Start with a decent size
	this->buffer_data = new Vertex2D[this->buffer_capacity * 6];

	this->vertexarray = ResourcesManager::get_instance()
		.acquire_vertexarray(nullptr, 6 * this->buffer_capacity,
			sizeof(Vertex2D), ScarabHash::hash_make(std::string(path)), true);

	// Set position and texuv attributes
	this->vertexarray->add_attribute<float>(2, false); // Position
	this->vertexarray->add_attribute<float>(2, false); // UV
}


Font::~Font() noexcept {
	delete this->data;
	delete[] this->buffer_data;
}

void Font::draw_text(const std::string& text, const vec2<float>& pos, const float scale, const Color& color) noexcept {
	if(text.empty()) {
		return;
	}

	std::shared_ptr<VertexArray>& vertexarray = this->vertexarray; // Cache

	// Grow buffer if needed
	if(text.length() > this->buffer_capacity) {
		this->buffer_capacity = text.length() + 64;
		delete[] this->buffer_data;
		this->buffer_data = new Vertex2D[this->buffer_capacity * 6];

		// Orphan the old GPU buffer
		vertexarray->alloc_data(nullptr, 6 * this->buffer_capacity, true);
	}

	// Generate quads
	float curx = 0.0f; // Start at local 0 to let Model Matrix handle pos
	float cury = 0.0f;
	uint32 num_vertices = 0;

	Vertex2D* data = this->buffer_data; // Cache
	for(char c : text) {
		// In range
		if(c < 32 || c > 127) {
			continue;
		}

		stbtt_aligned_quad q;
		stbtt_GetBakedQuad(this->data->cdata.data(), this->ATLAS_WIDTH, this->ATLAS_HEIGHT,
				c - 32, &curx, &cury, &q, 1);

		// Apply scale if needed
		// float x0 = q.x0 * scale;
		// float y0 = q.y0 * scale;
		// float x1 = q.x1 * scale;
		// float y1 = q.y1 * scale;

		// Standard 6-vertex quad (Triangles: 0,1,2 and 0,2,3)
		// Position data
		data[0].position = vec2<float>(q.x0, q.y0);
		data[1].position = vec2<float>(q.x1, q.y0);
		data[2].position = vec2<float>(q.x1, q.y1);
		data[3].position = vec2<float>(q.x0, q.y1);
		data[4].position = vec2<float>(q.x0, q.y0);
		data[5].position = vec2<float>(q.x1, q.y1);

		// UV data
		data[0].texuv = vec2<float>(q.s0, q.t0);
		data[1].texuv = vec2<float>(q.s1, q.t0);
		data[2].texuv = vec2<float>(q.s1, q.t1);
		data[3].texuv = vec2<float>(q.s0, q.t1);
		data[4].texuv = vec2<float>(q.s0, q.t0);
		data[5].texuv = vec2<float>(q.s1, q.t1);

		data += 6;
		num_vertices += 6;
	}

	// Render
	this->material->texture->bind();

	std::shared_ptr<ShaderProgram> shader = this->material->shader;
	shader->use();


	// Make model
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, pos.y, 0.0f));
	model = glm::scale(model, glm::vec3(scale, scale, 1.0f));

	// Bind Shader
	shader->set_color("shapeColor", color);
	shader->set_matrix4f("mvp", (this->camera.get_proj_matrix() * this->camera.get_view_matrix()) * model);

	vertexarray->bind_vao();
	vertexarray->update_vertices(this->buffer_data, num_vertices * sizeof(Vertex2D));
	glDrawArrays(GL_TRIANGLES, 0, num_vertices);
}
