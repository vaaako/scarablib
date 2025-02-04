#pragma once

#include "scarablib/opengl/shader.hpp"
#include "scarablib/opengl/vbo.hpp"
#include "scarablib/scenes/camera.hpp"
#include "scarablib/typedef.hpp"
#include "scarablib/utils/file.hpp"
#include "scarablib/opengl/vao.hpp"
#include <vector>

struct Skybox {
	// Uses a vector of 6 image paths as faces.
	// The order must be the following: Right, Left, Top, Bottom, Back and Front
	Skybox(const Camera& camera, const std::vector<const char*>& faces);
	~Skybox() noexcept;

	void draw() noexcept;

	private:
		uint32 texid;
		const Camera& camera;

		VAO* vao = new VAO();
		VBO* vbo = new VBO();

		Shader* shader = new Shader(
			FileHelper::read_file(SOURCE_DIR + "/../opengl/shaders/3d/skybox_vertex.glsl").c_str(),
			FileHelper::read_file(SOURCE_DIR + "/../opengl/shaders/3d/skybox_fragment.glsl").c_str()
		);
};
