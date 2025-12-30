#include "scarablib/opengl/resourcesmanager.hpp"
#include "scarablib/opengl/shaders.hpp"
#include "scarablib/proper/log.hpp"
#include "scarablib/window/window.hpp" // SDL_GL_GetCurrentContext
// Please keep this so in the future if i want to change SDL version i will just need to rename in one file

std::shared_ptr<ShaderProgram> ResourcesManager::load_shader_program(const std::vector<ResourcesManager::ShaderInfo>& infos) {
	if(infos.empty()) {
		throw ScarabError("No shader info provided to create a program");
	}

	// Creates a temporary vector of ShaderInfo to pass to the constructor.
	// This vector is now holding the owner of all shader's IDs
	// but inside the ShaderProgram's constructor this ownership is moved (not explicitly)
	std::vector<std::shared_ptr<Shader>> shaders;

	// -- VALIDATE SHADERS
	for(const ResourcesManager::ShaderInfo& info : infos) {
		if(info.source == nullptr) {
			throw ScarabError("ShaderInfo \"source\" was not provided");
		}

		if(info.type == Shader::Type::None) {
			throw ScarabError("ShaderInfo \"type\" was not provided");
		}

		// TODO: Support Vertex shader too
		// Inject custom shader
		std::string source = info.source; // std::string to void memory erros inside if-check
		if(info.iscustom) {
			source = std::string(Shaders::DEFAULT_FRAGMENT);
			std::string placeholder = "// {{USER_CODE}}";
			source.replace(
				// Find substring
				source.find(placeholder),
				// Size
				placeholder.length(),
				// Replace with
				// "#define HAS_USER_SHADER\n#ifdef HAS_USER_SHADER\n" + std::string(info.source) + "\n#endif"
				"#define HAS_USER_SHADER" + std::string(info.source)
			);
		}
		shaders.emplace_back(this->get_or_compile_shader(source.c_str(), info.type));
	}

	// -- CHECK COMBINED HASHES
	// Check if the program is already cached
	size_t combined_hash = this->combine_shader_hashes(shaders);
	if(std::shared_ptr<ShaderProgram> cache = this->get_program(combined_hash)) {
	#if defined(SCARAB_DEBUG_SHADER_MANAGER)
		LOG_DEBUG("Found shader program hash: %zu", combined_hash);
	#endif
		return cache;
	}

#if defined(SCARAB_DEBUG_SHADER_MANAGER)
	LOG_DEBUG("Not Found/Expired shader program hash: %zu", combined_hash);
#endif

	// -- CREATE PROGRAM
	// Create the ShaderProgram and cache it
	std::shared_ptr<ShaderProgram> program = std::make_shared<ShaderProgram>(shaders);
	program->hash = combined_hash;
	this->program_cache[combined_hash] = program;
	return program;
}


std::shared_ptr<VertexArray> ResourcesManager::acquire_vertexarray(const void* data, const size_t capacity, const size_t vertex_size, const size_t hash, const bool dynamic_vertex) noexcept {
	// -- CHECK IF CACHED

	std::shared_ptr<VertexArray> vertexarray = this->get_vertexarray(hash);
	if(vertexarray != nullptr) {
	#if defined(SCARAB_DEBUG_VAO_MANAGER)
		LOG_DEBUG("Hash %zu found! Reusing VAO.", hash);
	#endif
		return vertexarray; // Return the existing entry
	}

#if defined(SCARAB_DEBUG_VAO_MANAGER)
	LOG_DEBUG("Hash %zu not found. Creating new VAO.", hash);
#endif

	vertexarray = std::make_shared<VertexArray>(data, capacity, vertex_size, dynamic_vertex);

#if defined(SCARAB_DEBUG_VAO_MANAGER)
	LOG_DEBUG("VAO ID made: %zu", vertexarray->get_vaoid());
	GL_CHECK();
#endif

	vertexarray->hash = hash;
	this->vertexarray_cache[hash] = vertexarray;
	return vertexarray;
}


size_t ResourcesManager::combine_shader_hashes(const std::vector<std::shared_ptr<Shader>>& shaders) const noexcept {
	size_t combined_hash = 0;
	for(const auto& shader : shaders) {
		ScarabHash::hash_combine(combined_hash, shader->hash);
	}
	return combined_hash;
}

std::shared_ptr<Shader> ResourcesManager::get_or_compile_shader(const char* source, Shader::Type type) {
	size_t hash = ScarabHash::hash_make(std::string(source));

	// Chek if the shader is already compiled and cached
	if(source != nullptr && this->shader_cache.count(hash)) {
	#if defined(SCARAB_DEBUG_SHADER_MANAGER)
		LOG_DEBUG("Found %s shader hash: %zu", ((int)type == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT", hash);
	#endif
		if(std::shared_ptr<Shader> cache = this->get_shader(hash)) {
			return cache;
		}
	}

#if defined(SCARAB_DEBUG_SHADER_MANAGER)
	LOG_DEBUG("NOT found/expired %s hash (%zu) not found, compiling new", ((int)type == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT", hash);
#endif

	std::shared_ptr<Shader> shader = std::make_shared<Shader>(source, type);
	this->shader_cache[shader->hash] = shader;
	return shader;
}

std::shared_ptr<VertexArray> ResourcesManager::get_vertexarray(const size_t hash) noexcept {
	auto it = this->vertexarray_cache.find(hash);
	if(it != this->vertexarray_cache.end()) {
		if(std::shared_ptr<VertexArray> ptr = it->second.lock()) {
			return ptr;
		}
		this->vertexarray_cache.erase(it); // expired, remove entry
	}
	return nullptr; // not found or expired
}


std::shared_ptr<Shader> ResourcesManager::get_shader(const size_t hash) noexcept {
	auto it = this->shader_cache.find(hash);
	if(it != this->shader_cache.end()) {
		if(std::shared_ptr<Shader> ptr = it->second.lock()) {
			return ptr;
		}
		this->shader_cache.erase(it); // expired, remove entry
	}
	return nullptr; // not found or expired
}

std::shared_ptr<ShaderProgram> ResourcesManager::get_program(const size_t hash) noexcept {
	auto it = this->program_cache.find(hash);
	if(it != this->program_cache.end()) {
		if(std::shared_ptr<ShaderProgram> ptr = it->second.lock()) {
			return ptr;
		}
		this->program_cache.erase(it); // expired, remove entry
	}
	return nullptr; // not found or expired
}


void ResourcesManager::cleanup() noexcept {
	// Unable to relase VAOs correctly
	if(SDL_GL_GetCurrentContext() == NULL) {
		LOG_WARNING_FN("Called without a valid OpenGL context. Leaking GPU resources");
	}
	// Has no effect since its called implicitly, but i like to have them here
	this->vertexarray_cache.clear();
	this->shader_cache.clear();
	this->program_cache.clear();
}

