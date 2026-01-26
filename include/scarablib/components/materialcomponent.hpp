#pragma once

#include "scarablib/gfx/color.hpp"
#include "scarablib/gfx/texture.hpp"
#include "scarablib/gfx/texture_array.hpp"
#include "scarablib/opengl/assets.hpp"
#include "scarablib/opengl/resourcesmanager.hpp"
#include "scarablib/opengl/shader_program.hpp"
#include <memory>


struct Material {
	// Material's texture
	std::shared_ptr<Texture> texture = Assets::default_texture(); // nullptr: Default texture
	// Material's texture array
	TextureArray* texture_array = nullptr;
	// Material's default shader
	std::shared_ptr<ShaderProgram> shader = ResourcesManager::default_shader();

	// Material's color
	Color color = Colors::WHITE;
	// How much of the texture to mix with texture array.
	// Where 0 is only texture and 1 is only texture array.
	// Does not take any effect if texture is nullptr
	float mix_amount = 0.0f;

	// 1. First Mesh: A Mesh is created and its MaterialComponent asks the ResourcesManager for the default shader
	// 2. ResourcesManager (Cache miss): The manager compiles the first shader and allocated memory for one ShaderProgram object and compiles the code.
	//    It creates a shared_ptr to manage this new object and stores it in its map. It returns a copy of this `shared_ptr`
	// 3. ResourcesManager (Cache hit): The manager looks up the name "default" in its map and finds the existing shared_ptr. It returns a copy of the shared_ptr
	//
	// Each shared_ptr consumes around 16 bytes (8 bytes raw pointer)

	~Material() noexcept;
};
