#include "scarablib/opengl/assets.hpp"
#include "scarablib/proper/log.hpp"
#include "scarablib/utils/hash.hpp"
#include "scarablib/window/window.hpp" // SDL_GL_GetCurrentContext

#define SCARAB_DEBUG_ASSETS_MANAGER

Assets::Instance Assets::instance;

std::shared_ptr<Texture> Assets::load(const char* path, const bool flip_vertically, const bool flip_horizontally) noexcept {
	if(path == nullptr) {
		return Assets::default_texture();
	}

	// Theoretically raw-data textures and file textures can collide
	// This "header" is added to prevent this collision
	size_t hash = ScarabHash::hash_make(std::string_view("FILE_TEXTURE"));
	ScarabHash::hash_combine(hash, std::string_view(path));
	ScarabHash::hash_combine(hash, flip_vertically);
	ScarabHash::hash_combine(hash, flip_horizontally);

	// Chek if the texture is already compiled and cached
	std::shared_ptr<Texture> texture = Assets::get_tex(hash);
	if(texture != nullptr) {
		return texture;
	}

#if defined(SCARAB_DEBUG_ASSETS_MANAGER)
	LOG_DEBUG("NOT found/expired texture hash (%zu), compiling new", hash);
#endif

	Image image = Image(path, flip_vertically, flip_horizontally);
	texture = std::make_shared<Texture>(image);
	Assets::instance.tex_cache[hash] = texture;
	return texture;
}

std::shared_ptr<Texture> Assets::load(uint8* data, const uint32 width, const uint32 height, const uint8 channels) noexcept {
	if(data == nullptr) {
		return Assets::default_texture();
	}

	const size_t byte_count =
		size_t(width) * size_t(height) * size_t(channels);

	// Theoretically raw-data textures and file textures can collide
	// This "header" is added to prevent this collision
	size_t hash = ScarabHash::hash_make(std::string_view("RAW_TEXTURE"));
	ScarabHash::hash_combine(hash, ScarabHash::hash_bytes_fnv1a(data, byte_count));
	ScarabHash::hash_combine(hash, width);
	ScarabHash::hash_combine(hash, height);
	ScarabHash::hash_combine(hash, channels);

	// Chek if the texture is already compiled and cached
	std::shared_ptr<Texture> texture = Assets::get_tex(hash);
	if(texture != nullptr) {
		return texture;
	}

#if defined(SCARAB_DEBUG_ASSETS_MANAGER)
	LOG_DEBUG("NOT found/expired texture hash (%zu), compiling new", hash);
#endif

	Image image = Image(data, width, height, channels);
	texture = std::make_shared<Texture>(image);
	instance.tex_cache[hash] = texture;
	return texture;
}

std::shared_ptr<Texture> Assets::load(const Image& image) noexcept {
	if(image.data == nullptr) {
		return Assets::default_texture();
	}

	const size_t byte_count =
		size_t(image.width) * size_t(image.height) * size_t(image.nr_channels);

	// Theoretically raw-data textures and file textures can collide
	// This "header" is added to prevent this collision
	size_t hash = ScarabHash::hash_make(std::string_view("IMAGE_TEXTURE"));
	if(image.path) {
		ScarabHash::hash_combine(hash, std::string_view(image.path));
	} else {
		ScarabHash::hash_combine(hash, ScarabHash::hash_bytes_fnv1a(image.data, byte_count));
	}
	ScarabHash::hash_combine(hash, image.width);
	ScarabHash::hash_combine(hash, image.height);
	ScarabHash::hash_combine(hash, image.nr_channels);

	// Chek if the texture is already compiled and cached
	std::shared_ptr<Texture> texture = Assets::get_tex(hash);
	if(texture != nullptr) {
		return texture;
	}

#if defined(SCARAB_DEBUG_ASSETS_MANAGER)
	LOG_DEBUG("NOT found/expired texture hash (%zu), compiling new", hash);
#endif

	texture = std::make_shared<Texture>(image);
	Assets::instance.tex_cache[hash] = texture;
	return texture;
}

std::shared_ptr<Texture> Assets::get_tex(const size_t hash) {
	// Check if Texture was cached already
	auto it = Assets::instance.tex_cache.find(hash);
	if(it == Assets::instance.tex_cache.end()) {
		return nullptr;
	}

	if(std::shared_ptr<Texture> tex = it->second.lock()) {
	#if defined(SCARAB_DEBUG_ASSETS_MANAGER)
		LOG_DEBUG("Found texture with hash: %zu", hash);
	#endif
		return tex; // Get Cache
	}

	// weak_ptr expired, remove entry
	Assets::instance.tex_cache.erase(it);
	return nullptr;
}

void Assets::cleanup() noexcept {
	// Unable to relase VAOs correctly
	if(SDL_GL_GetCurrentContext() == NULL) {
		LOG_WARNING_FN("Called without a valid OpenGL context. Leaking GPU resources");
	}
	// Has no effect since its called implicitly, but i like to have them here
	Assets::instance.tex_cache.clear();
	Assets::instance.texarr_cache.clear();
	// Assets::instance.def_tex.reset();
}
