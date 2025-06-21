#pragma once

#include "scarablib/scenes/Iscene.hpp"
#include "scarablib/scenes/camera2d.hpp"

// Scene object used for managing 2D objects
class Scene2D : public IScene {
	public:
		// Build the Scene using the window object
		Scene2D(Camera2D& camera) noexcept;

		// Disable copy and moving
		Scene2D(const Scene2D&) = delete;
		Scene2D& operator=(const Scene2D&) = delete;
		Scene2D(Scene2D&&) = delete;
		Scene2D& operator=(Scene2D&&) = delete;
};

