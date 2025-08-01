#pragma once

#include "scarablib/opengl/shader_manager.hpp"
#include "scarablib/opengl/shaders.hpp"
#include "scarablib/opengl/vertexbuffercomponent.hpp"
#include "scarablib/camera/camera.hpp"
#include "scarablib/typedef.hpp"

struct Skybox {
	// Uses a vector of 6 image paths as faces.
	// The order must be the following: Right, Left, Top, Bottom, Back and Front
	Skybox(const Camera& camera, const std::array<const char*, 6>& faces);
	~Skybox() noexcept = default;

	// Draw the skybox
	void draw() noexcept;

	private:
		const Camera& camera;

		VertexBufferComponent bundle;
		uint32 texid;

		Shader* shader = ShaderManager::get_instance().get_or_load_shader(
			"skybox",
			Shaders::SKYBOX_VERTEX,
			Shaders::SKYBOX_FRAGMENT
		);
};
