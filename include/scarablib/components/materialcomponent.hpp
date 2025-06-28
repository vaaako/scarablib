#pragma once

#include "scarablib/gfx/color.hpp"
#include "scarablib/gfx/texture.hpp"
#include <cstddef>
#include <memory>

struct MaterialComponent {
	struct TextureHandle {
		private:
			std::shared_ptr<Texture> ptr; // If it was a reference it wouldnt rebind

		public:
			// Default texture
			TextureHandle() noexcept
				: ptr(Texture::default_texture()) {}
			// From rvalue texture
			explicit TextureHandle(Texture* texture) noexcept
				: ptr(texture ? std::shared_ptr<Texture>(texture) : Texture::default_texture()) {}
			// From lvalue texture
			explicit TextureHandle(const Texture& texture) noexcept
				: ptr(std::make_shared<Texture>(texture)) {}
			// From nullptr
			TextureHandle(std::nullptr_t) noexcept
				: ptr(Texture::default_texture()) {}

			// Copy constructor
			TextureHandle(const TextureHandle&) noexcept = default;
			// Move constructor
			TextureHandle(TextureHandle&&) noexcept = default;


			// Pointer-like access
			[[nodiscard]] const Texture* operator->() const noexcept {
				return this->ptr.get();
			}

			[[nodiscard]] constexpr Texture* operator->() noexcept {
				return this->ptr.get();
			}


			// Reference-like access
			constexpr operator const Texture&() const noexcept {
				return *this->ptr;
			}

			constexpr operator Texture&() noexcept {
				return *this->ptr;
			}

			// Assign nullptr reset to default texture
			TextureHandle& operator=(std::nullptr_t) noexcept {
				this->ptr = Texture::default_texture();
				return *this;
			}

			// Assign shared_ptr
			TextureHandle& operator=(const std::shared_ptr<Texture>& other) noexcept {
				this->ptr = other;
				return *this;
			}

			// Assign from lvalue texture
			TextureHandle& operator=(const Texture& other) noexcept {
				this->ptr = std::make_shared<Texture>(other);
				return *this;
			}

			// Delete rvalue texture
			TextureHandle& operator=(const Texture&& other) noexcept = delete;

			TextureHandle& operator=(Texture* texture) noexcept {
				this->ptr = texture ? std::shared_ptr<Texture>(texture) : Texture::default_texture();
				return *this;
			}

			// Assignment from another handle
			TextureHandle& operator=(const TextureHandle& other) noexcept = default;
			TextureHandle& operator=(TextureHandle&& other) noexcept = default;
	};

	// Current color
	Color color = Colors::WHITE;
	// Current texture being used.
	TextureHandle texture = nullptr; // Default texture
};
