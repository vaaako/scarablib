#pragma once

#include "scarablib/opengl/resourcesmanager.hpp"
#include "scarablib/opengl/shaders.hpp"
#include "scarablib/opengl/vertexarray.hpp"
#include "scarablib/camera/camera.hpp"
#include "scarablib/typedef.hpp"
#include <memory>

struct Skybox {
	// Uses a vector of 6 image paths as faces.
	// The order must be the following: Right, Left, Top, Bottom, Back and Front
	Skybox(const Camera& camera, const std::array<const char*, 6>& faces);
	~Skybox() noexcept = default;

	// Draw the skybox
	void draw() noexcept;

	private:
		const Camera& camera;
		std::shared_ptr<VertexArray> vertexarray;
		uint32 texid;

	std::shared_ptr<ShaderProgram> shader = ResourcesManager::get_instance().load_shader_program({
		// Default vertex and fragment shader source
		{ .source = Shaders::SKYBOX_VERTEX,   .type = Shader::Type::Vertex },
		{ .source = Shaders::SKYBOX_FRAGMENT, .type = Shader::Type::Fragment },
	});
};
