#pragma once

#include "scarablib/gfx/2d/sprite.hpp"
#include "scarablib/scenes/Iscene.hpp"
#include "scarablib/scenes/camera2d.hpp"
#include "scarablib/window/window.hpp"

/**
 * I could optimize more the draw method by using "begin_draw" and "end_draw" method like raylib
 * Using these methods, I would use the same VAO bind for all shapes inside of it, which is more optimized
 * I could also make something similar to love2d, where the color is changed using `love.graphics.color`
 * and all shapes drawed after this are colorized using the color defined
 *
 * Following the same idea, i could make the same for textures, this 3 changes would be much more optimized
 * but i prefer the way that is currently, maybe in the future i change it
 * */

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

		// Draw all models added to the scene
		void draw_all() const noexcept override;

		// Update the scene viewport using the window object
		inline void update_viewport(const Window& window) noexcept override {
			this->camera.update_viewport(window.get_width(), window.get_height());
		}

		// Update the scene viewport using a custom width and height values
		inline void update_viewport(const uint32 width, const uint32 height) noexcept override {
			this->camera.update_viewport(width, height);
		}

	private:
		Camera2D& camera;

		// Viewport
		uint32 width;
		uint32 height;
		glm::mat4 proj;
};

