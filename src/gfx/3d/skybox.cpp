#include "scarablib/gfx/3d/skybox.hpp"
#include "scarablib/geometry/vertex.hpp"
#include "scarablib/gfx/texture.hpp"
#include "scarablib/proper/error.hpp"
#include "scarablib/proper/log.hpp"
#include "scarablib/typedef.hpp"
#include "scarablib/gfx/image.hpp"

// I could make a different texture class for loading cubemap textures
// but i dont think is necessary
// so thats why i load Skybox's texture this way

Skybox::Skybox(const Camera& camera, const std::array<const char*, 6>& faces)
	: camera(camera) {

	// TODO: Back to vec3<float> later
	static const std::vector<Vertex> vertices = {
		Vertex { glm::vec3(-1.0f,  1.0f, -1.0f) }, // 0 - Top-Left-Back
		Vertex { glm::vec3( 1.0f,  1.0f, -1.0f) }, // 1 - Top-Right-Back
		Vertex { glm::vec3( 1.0f, -1.0f, -1.0f) }, // 2 - Bottom-Right-Back
		Vertex { glm::vec3(-1.0f, -1.0f, -1.0f) }, // 3 - Bottom-Left-Back
		Vertex { glm::vec3(-1.0f,  1.0f,  1.0f) }, // 4 - Top-Left-Front
		Vertex { glm::vec3( 1.0f,  1.0f,  1.0f) }, // 5 - Top-Right-Front
		Vertex { glm::vec3( 1.0f, -1.0f,  1.0f) }, // 6 - Bottom-Right-Front
		Vertex { glm::vec3(-1.0f, -1.0f,  1.0f )}  // 7 - Bottom-Left-Front
	};

	static const std::vector<uint8> indices = {
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

	this->vertexarray = ResourcesManager::get_instance()
		.acquire_vertexarray(vertices, indices);
	this->vertexarray->add_attribute<float>(3, false);


#if !defined(BUILD_OPGL30)
	glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &this->texid);

	// Load first image to get dimensions and format
	Image* firstimage = new Image(faces.at(0), false, true);
	if (firstimage->data == nullptr) {
		delete firstimage;
		throw ScarabError("Image (%s) in skybox was not found", faces.at(0));
	}
	glTextureStorage2D(this->texid,
		1,
		TextureBase::extract_format(firstimage->nr_channels, true),
		firstimage->width, firstimage->height
	);

	delete firstimage;
#else
	// Gen texture cube map
	glGenTextures(1, &this->texid);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->texid);
#endif

	// Order: Right > Left > Top > Bottom > Back > Front
	for(uint16 i = 0; i < 6; i++) {
		// OpenGL loads cubemap textures differently, so here makes sense enabling flip vertically (which actually disables)
		Image* image = new Image(faces.at(i), false, true);
		if(image->data == nullptr) {
			delete image;
			throw ScarabError("Image (%s) in skybox was not found", image->path);
		}

		const GLenum format = Texture::extract_format((uint32)image->nr_channels, false);
	#if !defined(BUILD_OPGL30)
		glTextureSubImage3D(this->texid,
			0, // mipmap level
			0, 0, i, // x, y, layer
			image->width, image->height, 1, // width, height, depth
			format,
			GL_UNSIGNED_BYTE,
			image->data
		);
	#else
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0,
			Texture::extract_format((uint32)image->nr_channels, true),
			image->width, image->height, 0,
			Texture::extract_format((uint32)image->nr_channels, false),
			GL_UNSIGNED_BYTE,
			image->data
		);
	#endif

		delete image;
	}

#if !defined(BUILD_OPGL30)
	glGenerateTextureMipmap(this->texid);

	glTextureParameteri(this->texid, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(this->texid, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTextureParameteri(this->texid, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(this->texid, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTextureParameteri(this->texid, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); // Z
#else
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); // Z
#endif

	// Bind to set uniform
	this->shader->use();
	this->shader->set_int("samplerSkybox", 0);
	this->shader->unbind();
}

void Skybox::draw() noexcept {
	glDepthFunc(GL_LEQUAL);

	std::shared_ptr<ShaderProgram>& shader = this->shader; // cache
	std::shared_ptr<VertexArray>& vertexarray = this->vertexarray;

	shader->use();
	// Remove translation from the view matrix to keep the skybox "centered" on the camera
	shader->set_matrix4f("view", glm::mat3(camera.get_view_matrix()));
	shader->set_matrix4f("proj", camera.get_proj_matrix());

	#if !defined(BUILD_OPGL30)
		glBindTextureUnit(0, this->texid);
	#else
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, this->texid);
	#endif

	vertexarray->bind_vao();
	// Indices are static, so it will be always GL_UNSIGNED_BYTE
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, 0);
	vertexarray->unbind_vao();

	shader->unbind();

#if defined(BUILD_OPGL30)
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
#endif

	glDepthFunc(GL_LESS);
}
