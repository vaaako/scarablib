#pragma once

#include "scarablib/scenes/Iscene.hpp"
#include "scarablib/scenes/camera.hpp"

// Scene object used for managing 3D objects
class Scene3D : public IScene {
	public:
		// Build 3D scene using the window object and a camera object
		Scene3D(Camera& camera) noexcept;

		// Disable copy and moving
		Scene3D(const Scene3D&) = delete;
		Scene3D& operator=(const Scene3D&) = delete;
		Scene3D(Scene3D&&) = delete;
		Scene3D& operator=(Scene3D&&) = delete;
};

