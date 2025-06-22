#pragma once

#include "scarablib/types/color.hpp"
#include "scarablib/types/texture.hpp"
#include <cstddef>

struct MaterialComponent {
	struct TextureHandle {
		private:
			Texture* ref; // If it was a reference it wouldnt rebind

		public:
			inline TextureHandle() noexcept : ref(Texture::default_texture().get()) {}
			inline constexpr TextureHandle(Texture& texture) noexcept : ref(&texture) {}
			inline TextureHandle(std::nullptr_t) noexcept : ref(Texture::default_texture().get()) {}

			// Use as Texture reference
			constexpr operator const Texture&() const noexcept {
				return *this->ref;
			}

			constexpr operator Texture&() noexcept {
				return *this->ref;
			}


			// Pointer-like access
			constexpr const Texture* operator->() const noexcept {
				return this->ref;
			}

			constexpr Texture* operator->() noexcept {
				return this->ref;
			}

			// If nullptr is passed, set to default texture
			TextureHandle& operator=(std::nullptr_t) noexcept {
				this->ref = Texture::default_texture().get();
				return *this;
			}

			TextureHandle& operator=(Texture& texture) noexcept {
				this->ref = &texture;
				return *this;
			}

			// Assignment from another handle
			TextureHandle& operator=(const TextureHandle& other) noexcept {
				this->ref = other.ref;
				return *this;
			}
	};

	// Current color
	Color color = Colors::WHITE;
	// Current texture being used.
	TextureHandle texture = nullptr; // Default texture
};
