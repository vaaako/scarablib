#pragma once

#include "scarablib/opengl/vao.hpp"
#include "scarablib/scenes/scene.hpp"

class Scene3D : public Scene {
	public:
		Scene3D(const Window& window);
		~Scene3D();

		// Disable copy and moving
		Scene3D(const Scene3D&) = delete;
		Scene3D& operator=(const Scene3D&) = delete;
		Scene3D(Scene3D&&) = delete;
		Scene3D& operator=(Scene3D&&) = delete;
	private:
		VAO* vao = new VAO();

		Shader& shader = new Shader(
			FileHelper::read_file(SOURCE_DIR + "/../opengl/shaders/2d/vertex.glsl").c_str(),
			FileHelper::read_file(SOURCE_DIR + "/../opengl/shaders/2d/fragment.glsl").c_str()
		);
};
