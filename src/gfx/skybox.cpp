#include "scarablib/gfx/skybox.hpp"
#include "scarablib/opengl/vbo.hpp"
#include "scarablib/proper/error.hpp"
#include "scarablib/types/image.hpp"

Skybox::Skybox(const Camera& camera, const std::vector<const char*>& faces) : camera(camera) {
	if(faces.size() != 6) {
		throw ScarabError("Skybox needs 6 faces in the following order: right, left, top, bottom, front and back");
	}

	// All cube faces
	// NOTE: Dont make static since a skybox will created only one or two times, so its a waste of memory
	std::vector<float> vertices = {
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	this->vao->bind();

	this->vbo->bind();
	this->vbo->make_from_vertices(vertices, 3, GL_STATIC_DRAW);

	this->vao->unbind();
	this->vbo->unbind();

	// Gen texture cube map
	glGenTextures(1, &this->texid);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->texid);

	for(uint16 i = 0; i < 6; i++) {
		Image* image = new Image(faces.at(i), true); // why when i set this to false the skybox bugs? (true actually makes the textures NOT flip on load, see image.cpp)
		if(image->data == nullptr) {
			throw ScarabError("Image (%s) was not found", image->path);
		}

		// Order: Right > Left > Top > Bottom > Back > Front
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->data);

		delete image;
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); // Z

	// Bind to set uniform
	this->shader->use();
	this->shader->set_int("samplerSkybox", 0);
	this->shader->unbind();
}

Skybox::~Skybox() noexcept {
	delete this->vao;
	delete this->vbo;
	delete this->shader;
}

void Skybox::draw() noexcept {
	// glDepthFunc(GL_LEQUAL);

	this->shader->use();
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->texid);
	this->shader->set_matrix4f("view", glm::mat3(camera.get_view_matrix())); // Conversion: Remove translation
	this->shader->set_matrix4f("proj", camera.get_proj_matrix());

	this->vao->bind();
	glDrawArrays(GL_TRIANGLES, 0, 36);
	this->vao->unbind();

	this->shader->unbind();
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	// Back to default
	// glDepthFunc(GL_LESS);
}
