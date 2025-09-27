#include "scarablib/opengl/shader_manager.hpp"

#include "scarablib/proper/error.hpp"
#include "scarablib/proper/log.hpp"
#include "scarablib/utils/hash.hpp"
#include "scarablib/window/window.hpp" // SDL_GL_GetCurrentContext
// Please keep this so in the future if i want to change SDL version i will just need to rename in one file

#include <memory>

#define SCARAB_DEBUG_SHADER_MANAGER

std::shared_ptr<ShaderProgram> ShaderManager::load_shader_program(const std::vector<ShaderProgram::ShaderInfo>& infos) {
	if(infos.empty()) {
		throw ScarabError("No shader info provided to create a program");
	}

	// Hash all shaders to check if any program is cached
	size_t combined_hash = 0;
	std::vector<size_t> individual_hashes;
	for(const ShaderProgram::ShaderInfo& info : infos) {
		size_t hash = ScarabHash::hash_make(std::string(info.source));
		individual_hashes.push_back(hash);
		ScarabHash::hash_combine(combined_hash, hash);
	}

	// -- CHECK COMBINED HASHES
	
	// Check if the program is already cached
	if(this->program_cache.count(combined_hash)) {
		#ifdef SCARAB_DEBUG_SHADER_MANAGER
		LOG_DEBUG("Found shader program hash: %zu", combined_hash);
		#endif
		if(std::shared_ptr<ShaderProgram> cache = this->get_program(combined_hash)) {
			return cache;
		}
	}

	#ifdef SCARAB_DEBUG_SHADER_MANAGER
	LOG_DEBUG("Not Found shader program hash: %zu", combined_hash);
	#endif


	// -- GET OR COMPILE SHADERS
	
	// Creates a temporary vector of ShaderInfo to pass to the constructor.
	// This vector is now holding the owner of all shader's IDs
	// but inside the ShaderProgram's constructor this ownership is moved (not explicitly)
	std::vector<ShaderProgram::ShaderInfo> shaders_build_info;
	for(const auto& info : infos) {
		shaders_build_info.emplace_back(ShaderProgram::ShaderInfo {
			.source      = nullptr,
			.type        = info.type,
			.existing_id = this->get_or_compile_shader(info.source, info.type)
		});
	}

	// -- CREATE PROGRAM

	// Create the ShaderProgram and cache it
	std::shared_ptr<ShaderProgram> program = std::make_shared<ShaderProgram>(shaders_build_info);
	program->hash = combined_hash;
	this->program_cache[combined_hash] = program;
	return program;
}

std::shared_ptr<uint32> ShaderManager::get_or_compile_shader(const char* source, ShaderProgram::Type type) {
	size_t hash = ScarabHash::hash_make(std::string(source));

	// Chek if the shader is already compiled and cached
	if(this->shader_cache.count(hash)) {
		LOG_DEBUG("Found %s shader hash: %zu", ((int)type == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT", hash);
		if(std::shared_ptr<uint32> cache = this->get_shader(hash)) {
			return cache;
		}
	}

	#ifdef SCARAB_DEBUG_SHADER_MANAGER
	LOG_DEBUG("NOT found/expired %s hash (%zu) not found, compiling new", ((int)type == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT", hash);
	#endif

	std::shared_ptr<uint32> id = ShaderProgram::compile_shader(source, type);
	this->shader_cache[hash] = id;
	return id;
}

std::shared_ptr<uint32> ShaderManager::get_shader(const size_t hash) noexcept {
	auto it = this->shader_cache.find(hash);
	if(it != this->shader_cache.end()) {
		if(std::shared_ptr<uint32> ptr = it->second.lock()) {
			return ptr;
		}
		this->shader_cache.erase(it); // expired, remove entry
	}
	return nullptr; // not found or expired
}

std::shared_ptr<ShaderProgram> ShaderManager::get_program(const size_t hash) noexcept {
	auto it = this->program_cache.find(hash);
	if(it != this->program_cache.end()) {
		if(std::shared_ptr<ShaderProgram> ptr = it->second.lock()) {
			return ptr;
		}
		this->program_cache.erase(it); // expired, remove entry
	}
	return nullptr; // not found or expired
}


void ShaderManager::cleanup() noexcept {
	// Unable to relase VAOs correctly
	if(SDL_GL_GetCurrentContext() == NULL) {
		LOG_WARNING_FN("Called without a valid OpenGL context. Leaking GPU resources");
	}

	// Has no effect since its called when window gets destroyed, but i like to have them here
	this->shader_cache.clear();
	this->program_cache.clear();
}

