#include "scarablib/opengl/assets.hpp"
#include "scarablib/proper/log.hpp"
#include "scarablib/window/window.hpp" // SDL_GL_GetCurrentContext

std::shared_ptr<Texture> Assets::load(const char* path, const bool flip_vertically, const bool flip_horizontally) {
	if(path == nullptr) {
		return Assets::default_texture();
	}

	size_t hash = 123;
	// Chek if the texture is already compiled and cached
	if(Assets::tex_cache.contains(hash)) {
	#if defined(SCARAB_DEBUG_ASSETS_MANAGER)
		LOG_DEBUG("Found texture: %zu", hash);
	#endif
		// Get Texture
		auto it = Assets::tex_cache.find(hash);
		if(it != Assets::tex_cache.end()) {
			if(std::shared_ptr<Texture> tex = it->second.lock()) {
				return tex; // Get Cache
			}
			Assets::tex_cache.erase(it); // Expired, remove entry
		}
	}

#if defined(SCARAB_DEBUG_ASSETS_MANAGER)
	LOG_DEBUG("NOT found/expired texture hash (%zu) not found, compiling new", hash);
#endif

	std::shared_ptr<Texture> shader = std::make_shared<Texture>(path, flip_vertically, flip_horizontally);
	Assets::tex_cache[hash] = shader;
	return shader;
}


void Assets::cleanup() noexcept {
	// Unable to relase VAOs correctly
	if(SDL_GL_GetCurrentContext() == NULL) {
		LOG_WARNING_FN("Called without a valid OpenGL context. Leaking GPU resources");
	}
	// Has no effect since its called implicitly, but i like to have them here
	this->tex_cache.clear();
	this->texarr_cache.clear();
}
