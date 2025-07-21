#pragma once

#include "scarablib/gfx/color.hpp"
#include "scarablib/gfx/texture.hpp"
#include "scarablib/gfx/texture_array.hpp"
#include "scarablib/typedef.hpp"
#include <cstddef>
#include <memory>

struct MaterialComponent {
	struct TextureHandle {
		std::shared_ptr<Texture> ptr; // In this case is better to use a shared_ptr

		public:
			// Default texture
			TextureHandle() noexcept;
			// From rvalue texture
			// explicit TextureHandle(Texture* texture) noexcept
			// 	: ptr(texture ? std::shared_ptr<Texture>(texture) : Texture::default_texture()) {}
			// // From lvalue texture
			// explicit TextureHandle(const Texture& texture) noexcept
			// 	: ptr(std::make_shared<Texture>(texture)) {}

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
			[[nodiscard]] inline constexpr const Texture* operator->() const noexcept {
				return this->ptr.get();
			}

			// Pointer-like non-const access
			[[nodiscard]] inline constexpr Texture* operator->() noexcept {
				return this->ptr.get();
			}

			// Reference-like const access
			inline constexpr operator const Texture&() const noexcept {
				return *this->ptr;
			}

			// Reference-like non-const access
			inline constexpr operator Texture&() noexcept {
				return *this->ptr;
			}

			// Assign nullptr reset to default texture
			inline TextureHandle& operator=(std::nullptr_t) noexcept {
				this->ptr = Texture::default_texture();
				return *this;
			}

			// Assign shared_ptr
			inline constexpr TextureHandle& operator=(const std::shared_ptr<Texture>& other) noexcept {
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

	// Current texture index being used>
	// This only takes effect if have a texture array
	uint16 texture_index = 0;
	// This member is here and not in TextureArray because then i would have
	// to check if TextureArray is not null in Model, Billboard and Circle
	// and i dont want to do this

	// Current color
	Color color = Colors::WHITE;
	// Current texture being used.
	TextureHandle texture = nullptr; // Default texture
	TextureArray* texture_array = nullptr;

	~MaterialComponent() noexcept;
};
