#include "scarablib/gfx/3d/skybox.hpp"
#include "scarablib/geometry/vertex.hpp"
#include "scarablib/gfx/texture.hpp"
#include "scarablib/opengl/vaomanager.hpp"
#include "scarablib/proper/error.hpp"
#include "scarablib/typedef.hpp"
#include "scarablib/gfx/image.hpp"

Skybox::Skybox(const Camera& camera, const std::array<const char*, 6>& faces)
	: camera(camera) {

	// TODO: Back to vec3<float> later
	static const std::vector<Vertex> vertices = {
		Vertex { glm::vec3(-1.0f,  1.0f, -1.0f) }, // 0 - Top-Left-Back
		Vertex { glm::vec3(1.0f,  1.0f, -1.0f)  }, // 1 - Top-Right-Back
		Vertex { glm::vec3(1.0f, -1.0f, -1.0f)  }, // 2 - Bottom-Right-Back
		Vertex { glm::vec3(-1.0f, -1.0f, -1.0f) }, // 3 - Bottom-Left-Back
		Vertex { glm::vec3(-1.0f,  1.0f,  1.0f) }, // 4 - Top-Left-Front
		Vertex { glm::vec3( 1.0f,  1.0f,  1.0f) }, // 5 - Top-Right-Front
		Vertex { glm::vec3( 1.0f, -1.0f,  1.0f) }, // 6 - Bottom-Right-Front
		Vertex { glm::vec3(-1.0f, -1.0f,  1.0f )}  // 7 - Bottom-Left-Front
	};

	static const std::vector<uint32> indices = {
		// Back face
		0, 3, 2,
		2, 1, 0,
		// Front face
		4, 5, 6,
		6, 7, 4,
		// Left face
		7, 3, 0,
		0, 4, 7,
		// Right face
		1, 2, 6,
		6, 5, 1,
		// Top face
		0, 1, 5,
		5, 4, 0,
		// Bottom face
		3, 7, 6,
		6, 2, 3
	};

	this->vertexarray = VAOManager::get_instance()
		.acquire_vertexarray(vertices, indices);

	// Gen texture cube map
	glGenTextures(1, &this->texid);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->texid);

	for(uint16 i = 0; i < 6; i++) {
		// OpenGL loads cubemap textures differently, so here makes sense enabling flip vertically (which actually disables)
		Image* image = new Image(faces.at(i), false, true);
		if(image->data == nullptr) {
			delete image;
			throw ScarabError("Image (%s) in skybox was not found", image->path);
		}

		// Order: Right > Left > Top > Bottom > Back > Front
		const GLenum format = Texture::extract_format((uint32)image->nr_channels, false);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, static_cast<GLint>(format), image->width, image->height, 0, format, GL_UNSIGNED_BYTE, image->data);

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

void Skybox::draw() noexcept {
	// glDepthFunc(GL_LEQUAL);
	std::shared_ptr<ShaderProgram> shader = this->shader; // cache
	std::shared_ptr<VertexArray> vertexarray = this->vertexarray;

	shader->use();
	shader->set_matrix4f("view", glm::mat3(camera.get_view_matrix())); // Conversion: Remove translation
	shader->set_matrix4f("proj", camera.get_proj_matrix());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->texid);

	vertexarray->bind_vao();
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	vertexarray->unbind_vao();

	shader->unbind();
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	// glDepthFunc(GL_LESS);
}
