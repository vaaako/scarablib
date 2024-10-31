#pragma once

#include "scarablib/shapes/mesh.hpp"
#include "scarablib/window/window.hpp"

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
class Scene {
	// Only Shape2D and Shape3D are accepted
	static_assert(std::is_same<T, Shape2D>::value || std::is_same<T, Mesh>::value,
			"Scene can only be instantiated with Shape2D or Mesh");

	public:
		// Build scene object using the window object for viewport
		Scene(const Window& window);
		virtual ~Scene() = default;

		// Add a shape object to the scene.
		// WARNING: Shapes added to the scene are not deleted automatically, is recommended to make the shape object and then add to the scene as a pointer
		void add_to_scene(T* shape);

		// Add a vector of shape object to the scene.
		// WARNING: Shapes added to the scene are not deleted automatically, is recommended to make the shape object and then add to the scene as a pointer
		void add_to_scene(const std::vector<T*>& shapes);

		// Remove object from scene using it's index
		void remove_index(const uint32 index);

		inline void set_drawmode(const DrawMode drawmode) {
			glPolygonMode(GL_FRONT_AND_BACK, drawmode);
		}

		// Draw all objects in scene
		virtual void draw_all() = 0;

		// Update viewport using window object
		virtual inline void update_viewport(const Window& window) = 0;

		// Update viewport using width and height values
		virtual void update_viewport(const uint32 width, const uint32 height) = 0;

	protected:
		// Viewport
		uint32 width;
		uint32 height;

		std::vector<T*> scene;
};

template <typename T>
Scene<T>::Scene(const Window& window)
	: width(window.get_width()), height(window.get_height()) {}


template <typename T>
void Scene<T>::add_to_scene(T* shape) {
	this->scene.emplace_back(shape);
}

template <typename T>
void Scene<T>::add_to_scene(const std::vector<T*>& shapes) {
	for(Shape2D* shape : shapes) {
		this->scene.emplace_back(shape);
	}
}

template <typename T>
void Scene<T>::remove_index(const uint32 index) {
	const uint64 last_index = this->scene.size() - 1;

	if(index > last_index) {
		LOG_ERROR("The index you are trying to remove is higher than the size of the objects in scene (%d)", this->scene.size());
		return;
	}

	// Move to last place
	if(index < last_index) {
		std::swap(this->scene[index], this->scene[last_index]);
	}

	// Remove last element
	this->scene.pop_back();
}

