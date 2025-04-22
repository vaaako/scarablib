#pragma once

#include "scarablib/gfx/3d/model.hpp"
#include "scarablib/opengl/shader_manager.hpp"
#include "scarablib/opengl/shaders.hpp"
#include "scarablib/proper/error.hpp"
#include "scarablib/utils/file.hpp"
#include "scarablib/window/window.hpp"
#include <algorithm>

// Different modes of drawing shapes
// `OUTLINEMODE` draws the outline of the shape
// `FILLMODE` draws the shape as normal
enum DrawMode : uint32 {
	OUTLINEMODE = GL_LINE,
	FILLMODE = GL_FILL
};

// Avoid foward declaration
class Sprite;

// Virtual class used to make Scene2D and Scene3D
template <typename T>
class IScene {
	// Only Sprite and Model are accepted
	static_assert(std::is_same<T, Sprite>::value || std::is_same<T, Model>::value,
			"Scene can only be instantiated with Sprite or Mesh");

	public:
		// Build scene object using the window object for viewport
		IScene() noexcept;
		virtual ~IScene() noexcept {
			this->scene.clear();
			this->vao_groups.clear();
		}

		// Add a shape to the scene.
		// Do not pass a pointer of a non allocated model or a temporary object.
		// The poiter passed is transformed into a shared pointer, so it will be automatically deleted.
		// Do not manually deleted or a double free will occur
		virtual void add_to_scene(const std::string_view& key, T* mesh);

		// You are trying to pass a temporary object to the scene.
		// Use the add_to_scene(const std::string_view& key, T* mesh) function
		void add_to_scene(const std::string_view& key, T&& mesh);

		// Get a object by key and dynamically convert it
		template <typename U>
		std::shared_ptr<U> get_by_key(const std::string_view& key) noexcept {
			auto it = this->scene.find(key);
			if(it == this->scene.end()) {
				LOG_ERROR("Object '%s' is not added to the scene", key.data());
				return nullptr;
			}

			auto casted_ptr = std::dynamic_pointer_cast<U>(it->second);
			if(!casted_ptr) {
				throw ScarabError("Failed to cast object with key '%s' to type", key.data());
			}

			return casted_ptr;
		}

		// Remove object by key
		void remove_by_key(const std::string_view& key);

		// Draw a single model.
		// If model was added to the scene, it will be drawn twice
		virtual void draw(T& model) const noexcept = 0;

		// Draw all objects added to the scene
		virtual void draw_all() const noexcept = 0;

		// Update the scene viewport using the window object
		virtual inline void update_viewport(const Window& window) noexcept = 0;

		// Update the scene viewport using a custom width and height values
		virtual void update_viewport(const uint32 width, const uint32 height) noexcept = 0;

		// Returns the default shader used by the scene
		inline Shader& get_shader() {
			return *this->shader;
		}

		// Set drawmode to the following shapes
		inline void set_drawmode(const DrawMode drawmode) noexcept {
			glPolygonMode(GL_FRONT_AND_BACK, drawmode);
		}

		// Get the number of objects in scene
		inline uint64 length() noexcept {
			return this->scene.size();
		}

	protected:
		// Objects added to the scene
		std::unordered_map<std::string_view, std::shared_ptr<T>> scene;
		// Use VAO as ID to track and batch draw meshes with the same VAO
		std::unordered_map<GLuint, std::vector<std::shared_ptr<T>>> vao_groups;

		Shader* shader = ShaderManager::get_instance().get_or_load_shader(
			"scene",
			Shaders::DEFAULT_VERTEX,
			Shaders::DEFAULT_FRAGMENT
		);
};


template <typename T>
IScene<T>::IScene() noexcept {}

template <typename T>
void IScene<T>::remove_by_key(const std::string_view& key) {
	auto it = this->scene.find(key);
	if(it == this->scene.end()) {
		throw ScarabError("Key '%s' was not found", key.data());
	}
	this->scene.erase(it);
}

template <typename T>
void IScene<T>::add_to_scene(const std::string_view& key, T* mesh) {
	static_assert(std::is_same<T, Sprite>::value || std::is_same<T, Model>::value,
			"Scene can only be instantiated with Sprite or Mesh");

	if(!mesh) {
		throw ScarabError("Attempted to add a null mesh to the scene with key '%s'", key.data());
	}

	std::shared_ptr<T> shared_mesh = std::shared_ptr<T>(mesh);
	this->scene.emplace(key, shared_mesh); // Used be get_by_key()

	auto& vao_groups = this->vao_groups[mesh->get_bundle().get_vao_id()];

	// Sort shaders to minimize shader changes
	// Find the correct position for insertion
	auto it = std::lower_bound(vao_groups.begin(), vao_groups.end(), shared_mesh,
		[](const std::shared_ptr<T>& a, const std::shared_ptr<T>& b) {

		// If a has no shader (default) and b has a shader, put a before b
		if(a->get_shader() == nullptr && b->get_shader() != nullptr) {
			return true;
		}

		// If a has a shader and b has no shader (default), put b before a
		if(a->get_shader() != nullptr && b->get_shader() == nullptr) {
			return false;
		}

		// Fallback: Sort by shader ID
		if(a->get_shader() != nullptr && b->get_shader() != nullptr) {
			return a->get_shader()->get_id() < b->get_shader()->get_id();
		}

		// Fallback: If both have no shaders, maintain original order
		return false;
	});

	// Insert the model in the correct place
	vao_groups.insert(it, shared_mesh);
}


template <typename T>
void IScene<T>::add_to_scene(const std::string_view& key, T&& mesh)  {
	mesh; // Prevent unused parameter warning
	throw ScarabError("You are trying to add a mesh to the scene, but %s is a temporary object, which is not allowed", key.data());
}
