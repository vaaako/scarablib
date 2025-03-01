#pragma once

#include "scarablib/opengl/shader.hpp"
#include "scarablib/scenes/camera.hpp"
#include "scarablib/scenes/Iscene.hpp"
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
class Scene3D : public IScene<Model> {
	public:
		// Build 3D scene using the window object and a camera object
		Scene3D(Camera& camera) noexcept;
		~Scene3D() noexcept;

		// Disable copy and moving
		Scene3D(const Scene3D&) = delete;
		Scene3D& operator=(const Scene3D&) = delete;
		Scene3D(Scene3D&&) = delete;
		Scene3D& operator=(Scene3D&&) = delete;

		// Add a model to the scene.
		// Do not pass a pointer of a non allocated model
		void add_to_scene(const std::string& key, Model* model) override;

		// Draw all models added to the scene
		void draw_all() const noexcept override;

		// Returns the default shader used by the scene
		inline Shader& get_shader() {
			return *this->shader;
		}

		// If enabled it will draw the collision of every model.
		// You can also enable for invidual models
		inline void draw_all_collider(const bool draw) noexcept {
			this->draw_every_box = draw;
		}

		// Update the scene viewport using the window object
		inline void update_viewport(const Window& window) noexcept override {
			this->camera.update_viewport(window);
		}

		// Update the scene viewport using a custom width and height values
		inline void update_viewport(const uint32 width, const uint32 height) noexcept override {
			this->camera.update_viewport(width, height);
		}


	private:
		// Check to draw all colliders
		bool draw_every_box = false;

		// Reference to camera, to change viewport
		Camera& camera;

		// Default shaders
		Shader* shader = new Shader(
			FileHelper::read_file("resources/shaders/vertex.glsl").c_str(),
			FileHelper::read_file("resources/shaders/fragment.glsl").c_str()
		);
};

