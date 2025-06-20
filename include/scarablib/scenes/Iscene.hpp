#pragma once

#include "scarablib/gfx/3d/model.hpp"
#include "scarablib/opengl/shader_manager.hpp"
#include "scarablib/opengl/shaders.hpp"
#include "scarablib/proper/error.hpp"
#include "scarablib/window/window.hpp"
#include <algorithm>
#include <memory>
#include <string_view>

// Virtual class used to make Scene2D and Scene3D
template <typename T> // NOTE: Mesh always default?
class IScene {
	public:
		// Different modes of drawing shapes
		// - `OUTLINEMODE`: draws the outline of the shape
		// - `FILLMODE`: draws the shape as normal
		enum DrawMode : uint32 {
			OUTLINEMODE = GL_LINE,
			FILLMODE = GL_FILL
		};

		// Build scene object using the window object for viewport.
		IScene() noexcept;
		virtual ~IScene() noexcept;

		// Add a shape to the scene
		template <typename U, typename... Args>
		U* add(const std::string& key, Args&&... args);

		// Add a model to the scene
		// template <typename U>
		// U* add(const std::string_view& key, const char* path);

		// You are trying to pass a temporary object to the scene.
		// Use the `add_to_scene(const std::string_view& key, T* mesh)` function
		// void add(const std::string_view& key, T&& mesh);

		// Returns an object by key and dynamically convert it.
		template <typename U>
		std::shared_ptr<U> get_by_key(const std::string& key) noexcept;

		// Remove object by key
		void remove_by_key(const std::string& key);

		// Draw a single model.
		// If the model was added to the scene, it will be drawn twice
		virtual void draw(T& model) const noexcept = 0;

		// Draw all objects added to the scene
		virtual void draw_all() const noexcept = 0;

		// Update viewport using window object
		virtual inline void update_viewport(const Window& window) noexcept = 0;

		// Update viewport using custom width and height
		virtual void update_viewport(const uint32 width, const uint32 height) noexcept = 0;

		// Returns a reference to the scene's default shader.
		inline Shader& get_shader() {
			return *this->shader;
		}

		// Sets the polygon drawing mode
		inline void set_drawmode(const DrawMode drawmode) noexcept {
			glPolygonMode(GL_FRONT_AND_BACK, drawmode);
		}

		// Returns the number of objects in the scene
		inline uint64 length() noexcept {
			return this->scene.size();
		}

	protected:
		// Objects added to the scene, stored as shared pointers for automatic memory management.
		std::unordered_map<std::string, std::shared_ptr<T>> scene;
		// Use VAO as ID to track and batch draw meshes with the same VAO.
		std::unordered_map<uint32, std::vector<std::shared_ptr<T>>> vao_groups;

		Shader* shader = ShaderManager::get_instance().get_or_load_shader(
			"scene",                     // Shader name.
			Shaders::DEFAULT_VERTEX,     // Default vertex shader source.
			Shaders::DEFAULT_FRAGMENT    // Default fragment shader source.
		); // Pointer to the default shader used by the scene.
};


template <typename T>
IScene<T>::IScene() noexcept {}

template <typename T>
IScene<T>::~IScene() noexcept {
	this->scene.clear();
	this->vao_groups.clear();
}


template <typename T>
template <typename U>
std::shared_ptr<U> IScene<T>::get_by_key(const std::string& key) noexcept {
	auto it = this->scene.find(key);
	if(it == this->scene.end()) {
		LOG_ERROR("Object '%s' is not added to the scene", key.data());
		return nullptr;
	}

	// Try to cast the object to the requested type
	auto casted_ptr = std::dynamic_pointer_cast<U>(it->second);
	if (!casted_ptr) {
		throw ScarabError("Failed to cast object with key '%s' to type", key.data());
	}

	return casted_ptr;
}

template <typename T>
void IScene<T>::remove_by_key(const std::string& key) {
	auto it = this->scene.find(key);
	if(it == this->scene.end()) {
		throw ScarabError("Key '%s' was not found", key.data());
	}

	// Remove from the scene key map
	this->scene.erase(key);
	// Remove from the scene vao map
	this->vao_groups.erase(it->second->bundle.vao->get_id());
}



template <typename T>
template <typename U, typename... Args>
U* IScene<T>::add(const std::string& key, Args&&... args) {
	static_assert(std::is_base_of_v<Mesh, T>,
			"Scene can only be instantiated with Mesh types");

	if(this->scene.find(key) != this->scene.end()) {
		throw ScarabError("Key '%s' already exists", key.data());
	}

	std::shared_ptr<T> mesh_ptr = std::make_shared<U>(std::forward<Args>(args)...); // Create new mesh
	this->scene.emplace(key, mesh_ptr); // Used be get_by_key()

	auto& vao_groups = this->vao_groups[mesh_ptr->bundle.vao->get_id()];

	// Sort shaders to minimize shader changes
	// Find the correct position for insertion
	auto it = std::lower_bound(vao_groups.begin(), vao_groups.end(), mesh_ptr,
		[](const std::shared_ptr<T>& a, const std::shared_ptr<T>& b) {

		auto sa = a->get_shader();
		auto sb = b->get_shader();

		// If A has no shader (default) and B has a shader, put a before B
		if(!sa && sb) {
			return true;
		}

		// If A has a shader and B has no shader (default), put B before A
		if(sa && !sb) {
			return false;
		}

		// Fallback: Sort by shader ID
		if(sa && sb) {
			return sa->get_id() < sb->get_id();
		}

		// Fallback: If both have no shaders, maintain original order
		return false;
	});

	// Insert the model in the correct place
	vao_groups.insert(it, mesh_ptr);
	return static_cast<U*>(mesh_ptr.get());
}


// template <typename T>
// void IScene<T>::add_to_scene(const std::string_view& key, T&& mesh)  {
// 	mesh; // Prevent unused parameter warning
// 	throw ScarabError("You are trying to add a mesh to the scene, but %s is a temporary object, which is not allowed", key.data());
// }
