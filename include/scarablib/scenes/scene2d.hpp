#pragma once

#include "scarablib/gfx/2d/sprite.hpp"
#include "scarablib/scenes/Iscene.hpp"
#include "scarablib/scenes/camera2d.hpp"

// Scene object used for managing 2D objects
class Scene2D : public IScene<Sprite> {
	public:
		// Build the Scene using the window object
		Scene2D(Camera2D& camera) noexcept;

		// Disable copy and moving
		Scene2D(const Scene2D&) = delete;
		Scene2D& operator=(const Scene2D&) = delete;
		Scene2D(Scene2D&&) = delete;
		Scene2D& operator=(Scene2D&&) = delete;

		// Draw a single model.
		// If the model was added to the scene, it will be drawn twice
		void draw(Sprite& shape) const noexcept override;

		// Draw all objects added to the scene
		void draw_all() const noexcept override;

		// Update viewport using window object
		inline void update_viewport(const Window& window) noexcept override {
			this->camera.update_viewport(window.get_width(), window.get_height());
		}

		// Update viewport using custom width and height
		inline void update_viewport(const uint32 width, const uint32 height) noexcept override {
			this->camera.update_viewport(width, height);
		}

	private:
		Camera2D& camera;
};

