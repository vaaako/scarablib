#pragma once

#include "scarablib/window/window.hpp"
#include "scarablib/opengl/shader.hpp"

enum DrawMode : uint32 {
	OUTLINEMODE = GL_LINE,
	FILLMODE = GL_FILL
};

enum DrawType : uint32 {
	POINTS = GL_POINTS,
	LINES = GL_LINES,
	TRIANGLES = GL_TRIANGLES,
};

class Scene {
	public:
		Scene(const Window& window);
		~Scene() {
			// delete this->shader;
			// delete this->shader_texture;
		}

		// Get shader reference
		inline const Shader& get_shader() const {
			return *this->shader;
		}

		// Update viewport using window object
		virtual inline void update_viewport(const Window& window) = 0;

		// Update viewport using width and height values
		virtual inline void update_viewport(const uint32 width, const uint32 height) = 0;

	protected:
		// Pointer to avoid forced init on this constructor
		const Shader* shader = nullptr;

		uint32 width;
		uint32 height;
};

