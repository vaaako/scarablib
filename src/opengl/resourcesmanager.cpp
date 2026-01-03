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
	shaders.reserve(infos.size());

	size_t combined_hash = 0; // To check if program exist
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
		ScarabHash::hash_combine(combined_hash, ScarabHash::hash_make(std::string_view(source)));
	}

	// -- CHECK COMBINED HASHES
	// Check if the program is already cached
	std::shared_ptr<ShaderProgram> program = this->get_program(combined_hash);
	if(program != nullptr) {
		return program;
	}

#if defined(SCARAB_DEBUG_SHADER_MANAGER)
	LOG_DEBUG(
		"Not Found/Expired shader program hash: %zu \nVertex Shader: \n%s \nFragment Shader: \n%s",
		combined_hash,
		infos[0].source,
		infos[1].source
	);
#endif

	// -- CREATE PROGRAM
	// Create the ShaderProgram and cache it
	program = std::make_shared<ShaderProgram>(shaders);
	program->hash = combined_hash;
	this->program_cache[combined_hash] = program;
	return program;
}


std::shared_ptr<VertexArray> ResourcesManager::acquire_vertexarray(const void* data, const size_t capacity, const size_t vertex_size, const size_t hash, const bool dynamic_vertex) noexcept {
	// -- CHECK IF CACHED

	std::shared_ptr<VertexArray> vertexarray = this->get_vertexarray(hash);
	if(vertexarray != nullptr) {
	#if defined(SCARAB_DEBUG_VERTEXARRAY_MANAGER)
		LOG_DEBUG("Hash %zu found! Reusing VAO.", hash);
	#endif
		return vertexarray; // Return the existing entry
	}

#if defined(SCARAB_DEBUG_VERTEXARRAY_MANAGER)
	LOG_DEBUG("Hash %zu not found. Creating new VAO.", hash);
#endif

	vertexarray = std::make_shared<VertexArray>(data, capacity, vertex_size, dynamic_vertex);

#if defined(SCARAB_DEBUG_VERTEXARRAY_MANAGER)
	LOG_DEBUG("VAO ID made: %zu", vertexarray->get_vaoid());
	GL_CHECK();
#endif

	vertexarray->hash = hash;
	this->vertexarray_cache[hash] = vertexarray;
	return vertexarray;
}


// size_t ResourcesManager::combine_shader_hashes(const std::vector<std::shared_ptr<Shader>>& shaders) const noexcept {
// 	size_t combined_hash = 0;
// 	for(const auto& shader : shaders) {
// 		ScarabHash::hash_combine(combined_hash, shader->hash);
// 	}
// 	return combined_hash;
// }

std::shared_ptr<Shader> ResourcesManager::get_or_compile_shader(const char* source, Shader::Type type) {
	size_t hash = ScarabHash::hash_make(std::string_view(source));

	std::shared_ptr shader = this->get_shader(hash);
	if(shader != nullptr) {
		return shader;
	}

#if defined(SCARAB_DEBUG_SHADER_MANAGER)
	LOG_DEBUG("NOT found/expired %s hash (%zu) not found, compiling new", ((int)type == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT", hash);
#endif

	shader = std::make_shared<Shader>(source, type);
	this->shader_cache[hash] = shader;
	return shader;
}

std::shared_ptr<VertexArray> ResourcesManager::get_vertexarray(const size_t hash) noexcept {
	// Chek if the Shader is already compiled and cached
	auto it = this->vertexarray_cache.find(hash);
	if(it == this->vertexarray_cache.end()) {
		return nullptr;
	}

	if(std::shared_ptr<VertexArray> cache = it->second.lock()) {
	#if defined(SCARAB_DEBUG_SHADER_MANAGER)
		LOG_DEBUG("Found Vertex Array with hash: %zu", hash);
	#endif
		return cache; // Get Cache
	}

	// weak_ptr expired, remove entry
	this->vertexarray_cache.erase(it);
	return nullptr;
}


std::shared_ptr<Shader> ResourcesManager::get_shader(const size_t hash) noexcept {
	// Chek if the Shader is already compiled and cached
	auto it = this->shader_cache.find(hash);
	if(it == this->shader_cache.end()) {
		return nullptr;
	}

	if(std::shared_ptr<Shader> cache = it->second.lock()) {
	#if defined(SCARAB_DEBUG_SHADER_MANAGER)
		LOG_DEBUG("Found Shader with hash: %zu", hash);
	#endif
		return cache; // Get Cache
	}

	// weak_ptr expired, remove entry
	this->shader_cache.erase(it);
	return nullptr;
}

std::shared_ptr<ShaderProgram> ResourcesManager::get_program(const size_t hash) noexcept {
	// Chek if the Shader Program is already compiled and cached
	auto it = this->program_cache.find(hash);
	if(it == this->program_cache.end()) {
		return nullptr;
	}

	if(std::shared_ptr<ShaderProgram> cache = it->second.lock()) {
	#if defined(SCARAB_DEBUG_SHADER_MANAGER)
		LOG_DEBUG("Found Shader Program with hash: %zu", hash);
	#endif
		return cache; // Get Cache
	}

	// weak_ptr expired, remove entry
	this->program_cache.erase(it);
	return nullptr;
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

