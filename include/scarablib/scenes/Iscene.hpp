#pragma once

#include "scarablib/gfx/model.hpp"
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
class Shape2D;
class Shape3D;

// Virtual class used to make Scene2D and Scene3D
template <typename T>
class IScene {
	// Only Shape2D and Shape3D are accepted
	static_assert(std::is_same<T, Shape2D>::value || std::is_same<T, Model>::value,
			"Scene can only be instantiated with Shape2D or Mesh");

	public:
		// Build scene object using the window object for viewport
		IScene(const Window& window);
		virtual ~IScene() = default;

		// Add a shape object to the scene.
		// WARNING: Shapes added to the scene are not deleted automatically, is recommended to make the shape object and then add to the scene as a pointer
		virtual void add_to_scene(const std::string& key, T* mesh) = 0;

		// Get a object by key and dynamically convert it
		template <typename U>
		std::shared_ptr<U> get_by_key(const std::string& key) {
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
		void remove_by_key(const std::string& key);

		// Draw all objects added to the scene
		virtual void draw_all() const = 0;

		// Update the scene viewport using the window object
		virtual inline void update_viewport(const Window& window) = 0;

		// Update the scene viewport using a custom width and height values
		virtual void update_viewport(const uint32 width, const uint32 height) = 0;

		// Set drawmode to following shapes
		inline void set_drawmode(const DrawMode drawmode) {
			glPolygonMode(GL_FRONT_AND_BACK, drawmode);
		}

		// Get the number of objects in scene
		inline uint64 length() {
			return this->scene.size();
		}

	protected:
		// TODO: This will be useful later
		const Window& window;

		// TODO: Remove this when implementing window resize
		// Viewport
		uint32 width;
		uint32 height;

		std::unordered_map<std::string, std::shared_ptr<T>> scene;
		// Use VAO as ID to query and batch draw meshes with the same VAO
		std::unordered_map<GLuint, std::vector<std::shared_ptr<T>>> vao_groups;
};

template <typename T> // Return the shared pointer
IScene<T>::IScene(const Window& window)
	: window(window), width(window.get_width()), height(window.get_height()) {}


template <typename T>
void IScene<T>::remove_by_key(const std::string& key) {
	auto it = this->scene.find(key);
	if(it == this->scene.end()) {
		throw ScarabError("Key '%s' not found", key.c_str());
	}
	this->scene.erase(it);
}

// template <typename T>
// void Scene<T>::remove_index(const uint32 index) {
// 	const uint64 last_index = this->scene.size() - 1;
//
// 	if(index > last_index) {
// 		LOG_ERROR("The index you are trying to remove is higher than the size of the objects in scene (%d)", this->scene.size());
// 		return;
// 	}
//
// 	// Move to last place
// 	if(index < last_index) {
// 		std::swap(this->scene[index], this->scene[last_index]);
// 	}
//
// 	// Remove last element
// 	this->scene.pop_back();
// }

