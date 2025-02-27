#pragma once

#include "scarablib/gfx/3d/model.hpp"
#include "scarablib/proper/error.hpp"
#include "scarablib/window/window.hpp"
#include <memory>
#include <unordered_map>

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
	// Only Shape2D and Shape3D are accepted
	static_assert(std::is_same<T, Sprite>::value || std::is_same<T, Model>::value,
			"Scene can only be instantiated with Sprite or Mesh");

	public:
		// Build scene object using the window object for viewport
		IScene() noexcept;
		virtual ~IScene() = default;

		// Add a shape object to the scene.
		virtual void add_to_scene(const std::string& key, T* mesh) = 0;

		// Get a object by key and dynamically convert it
		template <typename U>
		std::shared_ptr<U> get_by_key(const std::string& key) noexcept {
			auto it = this->scene.find(key);
			if(it == this->scene.end()) {
				LOG_ERROR("Object '%s' is not added to the scene", key.c_str());
				return nullptr;
			}

			auto casted_ptr = std::dynamic_pointer_cast<U>(it->second);
			if(!casted_ptr) {
				throw ScarabError("Failed to cast object with key '%s' to type", key.c_str());
			}

			return casted_ptr;
		}

		// Remove object by key
		void remove_by_key(const std::string& key) noexcept;

		// Draw all objects added to the scene
		virtual void draw_all() const noexcept = 0;

		// Update the scene viewport using the window object
		virtual inline void update_viewport(const Window& window) noexcept = 0;

		// Update the scene viewport using a custom width and height values
		virtual void update_viewport(const uint32 width, const uint32 height) noexcept = 0;

		// Set drawmode to following shapes
		inline void set_drawmode(const DrawMode drawmode) noexcept {
			glPolygonMode(GL_FRONT_AND_BACK, drawmode);
		}

		// Get the number of objects in scene
		inline uint64 length() noexcept {
			return this->scene.size();
		}

	protected:
		// Objects added to the scene
		std::unordered_map<std::string, std::shared_ptr<T>> scene;
		// Use VAO as ID to track and batch draw meshes with the same VAO
		std::unordered_map<GLuint, std::vector<std::shared_ptr<T>>> vao_groups;
};

template <typename T> // Return the shared pointer
IScene<T>::IScene() noexcept {}

template <typename T>
void IScene<T>::remove_by_key(const std::string& key) noexcept {
	auto it = this->scene.find(key);
	if(it == this->scene.end()) {
		throw ScarabError("Key '%s' was not found", key.c_str());
	}
	this->scene.erase(it);
}

