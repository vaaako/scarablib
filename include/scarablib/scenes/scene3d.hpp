#pragma once

#include "scarablib/opengl/shader.hpp"
#include "scarablib/scenes/camera.hpp"
#include "scarablib/scenes/scene.hpp"
#include "scarablib/shapes/shape3d.hpp"
#include "scarablib/utils/file.hpp"

// Scene object used for managing 3D objects
class Scene3D : public Scene {
	public:
		// Build 3D scene using the window object and a camera object
		Scene3D(const Window& window, Camera& camera);
		~Scene3D();

		// Disable copy and moving
		Scene3D(const Scene3D&) = delete;
		Scene3D& operator=(const Scene3D&) = delete;
		Scene3D(Scene3D&&) = delete;
		Scene3D& operator=(Scene3D&&) = delete;

		// Draw a 3D mesh using a reference of it.
		void draw_mesh(Shape3D& shape);

		inline void update_viewport(const Window& window) override {
			this->camera.update_viewport(window);
		}

		inline void update_viewport(const uint32 width, const uint32 height) override {
			this->camera.update_viewport(width, height);
		}

	private:
		Camera& camera;

		Shader* shader = new Shader(
			FileHelper::read_file(SOURCE_DIR + "/../opengl/shaders/3d/vertex.glsl").c_str(),
			FileHelper::read_file(SOURCE_DIR + "/../opengl/shaders/3d/fragment.glsl").c_str()
		);
};
