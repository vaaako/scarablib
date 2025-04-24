#pragma once

#include "scarablib/scenes/Iscene.hpp"
#include "scarablib/scenes/camera.hpp"

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

		// Draw a single model.
		// If the model was added to the scene, it will be drawn twice
		void draw(Model& model) const noexcept override;

		// Draw all objects added to the scene
		void draw_all() const noexcept override;

		// Update viewport using window object
		inline void update_viewport(const Window& window) noexcept override {
			this->camera.update_viewport(window);
		}

		// Update viewport using custom width and height
		inline void update_viewport(const uint32 width, const uint32 height) noexcept override {
			this->camera.update_viewport(width, height);
		}


	private:
		Camera& camera;
};

