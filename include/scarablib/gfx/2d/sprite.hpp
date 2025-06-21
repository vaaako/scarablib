#pragma once

#include "scarablib/gfx/mesh.hpp"
#include "scarablib/opengl/shader.hpp"
#include "scarablib/proper/dirtyproxy.hpp"
#include "scarablib/types/vertex.hpp"

// Renderable 2D object with transform/state
class Sprite : public Mesh {
	public:
		// Sprite's position
		DirtyProxy<vec2<float>> position = DirtyProxy(vec2<float>(0.0f), this->isdirty);
		// Sprite's size in pixels
		DirtyProxy<vec2<float>> size = DirtyProxy(vec2<float>(1.0f), this->isdirty);
		// Rotation angle
		DirtyProxy<float> angle = DirtyProxy(0.0f, this->isdirty);

		Sprite(const std::vector<Vertex>& vertices) noexcept;

		// This method does not draw the model to the screen, as it does not bind the VAO and Shader (batch rendering)
		virtual void draw_logic(const Camera& camera, const Shader& shader) noexcept override;

	protected:
		void update_model_matrix() noexcept override;
};

