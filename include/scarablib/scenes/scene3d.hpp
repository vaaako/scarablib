#pragma once

#include "scarablib/opengl/shader.hpp"
#include "scarablib/scenes/camera.hpp"
#include "scarablib/scenes/Iscene.hpp"
#include "scarablib/gfx/mesh.hpp"
#include "scarablib/utils/file.hpp"

#ifdef SCARAB_DEBUG_DRAWCALL
#include "scarablib/types/font.hpp"
#include "scarablib/scenes/scene2d.hpp"
#endif

/**
 * A more optimized draw method would be "draw_start" and "draw_end" where in
 * draw_start is enabled everything in advance (like depth test)
 *
 * */

// Scene object used for managing 3D objects
class Scene3D : public IScene<Mesh> {
	public:
		// Build 3D scene using the window object and a camera object
		Scene3D(const Window& window, Camera& camera);
		~Scene3D();

		// Disable copy and moving
		Scene3D(const Scene3D&) = delete;
		Scene3D& operator=(const Scene3D&) = delete;
		Scene3D(Scene3D&&) = delete;
		Scene3D& operator=(Scene3D&&) = delete;

		// Add a mesh to the scene
		void add_to_scene(const std::string& key, Mesh* mesh) override;

		// Draw all meshes added to the scene
		void draw_all() const override;

		// Update the scene viewport using the window object
		inline void update_viewport(const Window& window) override {
			this->camera.update_viewport(window);
		}

		// Update the scene viewport using a custom width and height values
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

