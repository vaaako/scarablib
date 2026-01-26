#pragma once

#include "scarablib/gfx/texture.hpp"
#include "scarablib/gfx/texture_array.hpp"
#include <memory>
#include <unordered_map>

// REMEMBER: I would make a system that when loading textures for a Texture Array
// The code would look up to see if each individual texture was already allocated
// and use it instead
// But this isnt possible, because Texture Array does not store individual textures


// Texture and Texture Array manager
class Assets {
	public:
		// Returns a default solid white texture
		static std::shared_ptr<Texture> default_texture() noexcept {
			// static std::shared_ptr<Texture> def_tex = std::make_shared<Texture>(white_pixel, 1, 1, 4);
			static std::shared_ptr<Texture> def_tex = std::make_shared<Texture>();
			return def_tex;
		}

		// static std::shared_ptr<Texture> load_texture(path, other stuff);
		// static std::shared_ptr<TextureArray> load_texturearray(textures);

		static std::shared_ptr<Texture> load(const char* path, const bool flip_v = false, const bool flip_h = false) noexcept;
		static std::shared_ptr<Texture> load(const Image& image) noexcept;

		// Cleans up all maps;
		// WARNING: This is called inside Window destructor, DO NOT call it manually
		static void cleanup() noexcept;

	private:
		struct Instance {
			std::unordered_map<size_t, std::weak_ptr<Texture>> tex_cache;
			std::unordered_map<size_t, std::weak_ptr<TextureArray>> texarr_cache;
			std::shared_ptr<Texture> def_tex;
		};
		static Instance instance;
		static std::shared_ptr<Texture> get_tex(const size_t hash);
};

	// TODO:
	// - Make Texture and Texture Array be created from Image class
	// - Two methods each (in Assets) one using Image struct and other passing
	//
	// - Loading Texture2D: Put in a Image struct and hash the struct
	// - Then check if the hash contains in texture2d_cache map
	//
	// - Loading Texture Array: Load all as image struct and hash all together
	// - Then check if the hash contains in texturearray_cache map
	//
	// - Caching Image is a bit overkill
	//
	// - Make hash from Image struct
	// - Put default texture here
	// - TextureHandle (from MaterialComponent) keeps the same, but only handling std::shared_ptr<Texture>
	// - Make TextureArrayHandle for MaterialComponent
	//
/*
Pseudo-code

// Texture2D
ImageData img = ImageCache::load(path);
if (!Texture2DCache.contains(img, params)) {
    upload Texture2D
}
return Texture2DCache.get(img, params);

// TextureArray
for path in paths:
    ImageData img = ImageCache::load(path)
validate layers
if (!TextureArrayCache.contains(layers, params)) {
    upload TextureArray
}
return TextureArrayCache.get(layers, params);
*/



	// std::shared_ptr<Texture> get_texture(const size_t hash) noexcept {
	// 	auto it = this->vertexarray_cache.find(hash);
	// 	if(it != this->vertexarray_cache.end()) {
	// 		if(std::shared_ptr<Texture> ptr = it->second.lock()) {
	// 			return ptr;
	// 		}
	// 		this->vertexarray_cache.erase(it); // expired, remove entry
	// 	}
	// 	return nullptr; // not found or expired
	// }
