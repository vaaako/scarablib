#pragma once

#include "scarablib/opengl/shader.hpp"
#include "scarablib/opengl/vao.hpp"
#include "scarablib/proper/vector/vec2.hpp"
#include "scarablib/typedef.hpp"
#include "scarablib/types/color.hpp"
#include "scarablib/types/texture.hpp"
#include "scarablib/types/vertex.hpp"
#include "scarablib/scenes/scene.hpp"

// NOTE -- Technically I could use the same VAO for Triangle and Rectangle

class Shape2D {
	public:
		// Construct using a vector of vertices and a vector of indices values
		Shape2D(const std::vector<Vertex>& vertices, const std::vector<uint32>& indices);
		// No need to delete texture, since is actually a "reference" to another texture
		~Shape2D();

		void draw(const Shader& shader, const DrawMode drawmode = DrawMode::FILLMODE, const DrawType drawtype = DrawType::TRIANGLES) const;

		// Get current texture
		inline Texture* get_texture() const {
			return this->texture;
		}

		// Set rotation angle in radians
		inline Shape2D& set_angle(const float angle) {
			this->angle = angle;
			return *this;
		}

		// Set the shape's color
		//
		// If a shape is using a texture and a color at the same time, the texture will colorize using the color defined
		inline Shape2D& set_color(const Color& color) {
			this->color = color;
			return *this;
		}


		inline Shape2D& set_position(const vec2<float>& position) {
			this->position = position;
			return *this;
		}

		// Set size using a different value for each axis (X and Y)
		inline Shape2D& set_size(const vec2<float>& size) {
			this->size = size;
			return *this;
		}

		// Set size using a single value for all axis
		inline Shape2D& set_size(const float size) {
			this->size = size;
			return *this;
		}

		// Scale the shape using a single value for all axis
		//
		// Ex.: `new_scale = current_scale * new_scale`
		inline Shape2D& set_scale(const float scale) {
			this->size = this->size * scale;
			return *this;
		}

		// Sete a scale using different values for each axis (X and Y)
		inline Shape2D& set_scale(const vec2<float>& scale) {
			this->size = this->size * scale;
			return *this;
		}

		inline Shape2D& set_texture(Texture* texture) {
			// TODO -- for 3d objects
			// if(!has_texuv) {
			// 	LOG_INFO("This mesh does not support texture");
			// 	return *this;
			// }

			this->texture = texture;
			return *this;
		}


	private:
		Texture* texture = nullptr; // Pointer because might be nullptr
		Color color = { 255, 255, 255, 255 };

		vec2<float> position = { 100.0f, 100.0f };
		vec2<float> size = { 50.0f, 50.0f };
		// vec2<float> size = { 1.0f, 1.0f };

		float angle = 0.0f;
		// bool has_texuv = true;

		VAO* vao = new VAO(); // Pointer because size will increase
		GLsizei indices_size; // Pre calculated indices size
};
