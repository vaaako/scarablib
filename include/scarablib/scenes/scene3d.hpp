#pragma once

#include "scarablib/scenes/camera.hpp"
#include "scarablib/scenes/Iscene.hpp"

// Scene object used for managing 3D objects
class Scene3D : public IScene<Model> {
	public:
		// Build 3D scene using the window object and a camera object
		Scene3D(Camera& camera) noexcept;

		// Disable copy and moving
		Scene3D(const Scene3D&) = delete;
		Scene3D& operator=(const Scene3D&) = delete;
		Scene3D(Scene3D&&) = delete;
		Scene3D& operator=(Scene3D&&) = delete;

		// Draw all models added to the scene
		void draw_all() const noexcept override;

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
};

