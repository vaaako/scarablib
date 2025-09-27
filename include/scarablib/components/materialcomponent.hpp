#pragma once

#include "scarablib/gfx/color.hpp"
#include "scarablib/gfx/texture.hpp"
#include "scarablib/gfx/texture_array.hpp"
#include "scarablib/opengl/shader_manager.hpp"
#include "scarablib/opengl/shaders.hpp"
#include <cstddef>
#include <memory>


struct MaterialComponent {
	// Handles when user tries to set material.texture to nullptr.
	// Setting it to the default solid white texture instead.
	// Since 99% of the time the shader will use a texture anyway.
	// so having this system and not a "uniform check" (check for u_hasTexture) makes sense
	struct TextureHandle {
		std::shared_ptr<Texture> ptr; // In this case is better to use a shared_ptr

		public:
			// Default texture
			TextureHandle() noexcept;

			explicit TextureHandle(Texture* texture) noexcept;
			// From lvalue texture
			explicit TextureHandle(const Texture& texture) noexcept;
			// From nullptr
			TextureHandle(std::nullptr_t) noexcept;

			// Copy constructor
			TextureHandle(const TextureHandle&) noexcept = default;
			// Move constructor
			TextureHandle(TextureHandle&&) noexcept = default;
			// Assignment from another handle
			TextureHandle& operator=(const TextureHandle& other) noexcept = default;
			TextureHandle& operator=(TextureHandle&& other) noexcept = default;
			// Delete rvalue texture
			TextureHandle& operator=(const Texture&& other) noexcept = delete;

			// Pointer-like const access
			[[nodiscard]] inline const Texture* operator->() const noexcept {
				return this->ptr.get();
			}

			// Pointer-like non-const access
			[[nodiscard]] inline Texture* operator->() noexcept {
				return this->ptr.get();
			}

			// Reference-like const access
			inline operator const Texture&() const noexcept {
				return *this->ptr;
			}

			// Reference-like non-const access
			inline operator Texture&() noexcept {
				return *this->ptr;
			}

			// -- OPERATOR == //
			inline bool operator==(TextureHandle& other) noexcept {
				return this->ptr->get_id() == other->get_id();
			}

			inline bool operator!=(TextureHandle& other) noexcept {
				return this->ptr->get_id() == other->get_id();
			}

			inline bool operator==(std::nullptr_t) noexcept {
				return this->ptr->get_id() == Texture::default_texture()->get_id();
			}

			inline bool operator!=(std::nullptr_t) noexcept {
				return this->ptr->get_id() != Texture::default_texture()->get_id();
			}
			// OPERATOR == -- //

			// Assign nullptr reset to default texture
			inline TextureHandle& operator=(std::nullptr_t) noexcept {
				this->ptr = Texture::default_texture();
				return *this;
			}

			// Assign shared_ptr
			inline TextureHandle& operator=(const std::shared_ptr<Texture>& other) noexcept {
				this->ptr = other;
				return *this;
			}

			// Assign from rvalue Texture&
			inline TextureHandle& operator=(const Texture& other) noexcept {
				this->ptr = std::make_shared<Texture>(other);
				return *this;
			}

			// Assign from rvalue Texture*
			inline TextureHandle& operator=(Texture* texture) noexcept {
				this->ptr = texture ? std::shared_ptr<Texture>(texture) : Texture::default_texture();
				return *this;
			}
	};

	// How much of the texture to mix with texture array.
	// Where 0 is only texture and 1 is only texture array.
	// Does not take any effect if texture is nullptr
	float mix_amount = 0.0f;

	// Material's color
	Color color = Colors::WHITE;
	// Material's texture
	TextureHandle texture = nullptr; // nullptr: Default texture
	// Material's texture array
	TextureArray* texture_array = nullptr;

	// Material's default shader
	std::shared_ptr<ShaderProgram> shader = ShaderManager::get_instance().load_shader_program({
		// Default vertex and fragment shader source
		{ .source = Shaders::DEFAULT_VERTEX,   .type = ShaderProgram::Type::Vertex },
		{ .source = Shaders::DEFAULT_FRAGMENT, .type = ShaderProgram::Type::Fragment },
	});

	// 1. First Mesh: A Mesh is created and its MaterialComponent asks the ShaderManager for the default shader
	// 2. ShaderManager (Cache miss): The manager compiles the first shader and allocated memory for one ShaderProgram object and compiles the code. It creates a shared_ptr to manage this new object and stores it in its map. It returns a copy of this `shared_ptr`
	// 3. ShaderManager (Cache hit): The manager looks up the name "default" in its map and finds the existing shared_ptr. It returns a copy of the shared_ptr
	//
	// Each shared_ptr consumes around 16 bytes (8 bytes raw pointer)

	MaterialComponent() noexcept = default;
	~MaterialComponent() noexcept;
};
