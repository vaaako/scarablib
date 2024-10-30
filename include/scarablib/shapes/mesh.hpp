#pragma once

#include <GL/glew.h>
#include "scarablib/opengl/shader.hpp"
#include "scarablib/opengl/vao.hpp"
#include "scarablib/scenes/camera.hpp"
#include "scarablib/typedef.hpp"
#include "scarablib/types/texture.hpp"
#include "scarablib/types/vertex.hpp"


// Struct used to initialize a Mesh
struct MeshConf {
	const vec3<float> position;
	const vec3<float> size = 1.0f;
	Color color = Colors::WHITE;
	float angle = 0.0f;
};

// An object used for as a base for 3D Shapes
class Mesh {
	public:
		// Make a mesh from existing VAO and initial config
		Mesh(const MeshConf& conf, const VAO* vao, const uint32 indices_length = 0);
		Mesh(const char* path);

		// Mesh(const VAO* vao, const uint32 indices_length = 0);
		//
		// Init mesh using a vector of Vertex and Indices
		// Mesh(const std::vector<Vertex>& data, const std::vector<uint32>& indices);
		// Init mesh using a vector of Vertices, Texture Coordinates (optional) and Indices
		// Mesh(const std::vector<float>& vertices, const std::vector<float>& tex_coords, const std::vector<uint32>& indices);

		// Draw current mesh.
		// It needs a camera object and a shader
		virtual void draw(const Camera& camera, const Shader& shader);

		// Get current texture
		inline Texture get_texture() const {
			return *this->texture;
		}

		// Get current position
		inline vec3<float> get_position() const {
			return this->position;
		}

		// Get current size of each axis
		inline vec3<float> get_size() const {
			return this->size;
		}

		// Get current color
		inline Color get_color() const {
			return this->color;
		}

		// Get current angle
		inline float get_angle() const {
			return this->angle;
		}

		// SETTERS //

		// Set a texture to be used
		inline Mesh& set_texture(Texture* texture) {
			if(texture == nullptr){
				this->texture = &this->get_deftex();
				return *this;
			}

			this->texture = texture;
			return *this;
		}

		// Removes the shape's texture
		inline Mesh& remove_texture() {
			this->texture = &this->get_deftex();
			return *this;
		}

		// Set a new position using a vector
		inline Mesh& set_position(const vec3<float>& position) {
			this->position = position;
			this->isdirty = true;
			return *this;
		}

		// Set a new size using a vector (X and Y)
		inline Mesh& set_size(const vec3<float>& size) {
			this->size = size;
			this->isdirty = true;
			return *this;
		}

		// Scale the shape using a single value for all axis.
		// e.g., `size = size.xyz * scale`
		inline void set_scale(const float& scale) {
			this->size * scale;
			this->isdirty = true;
		}

		// Scale the shape using a different value for each axis.
		// e.g., `size.x = size.x * scale.x`, `size.y = size.y * scale.y` and `size.z = size.z * scale.z`
		inline void set_scale(const vec3<float>& scale) {
			this->size * scale;
			this->isdirty = true;
		}

		// Set a new color
		// If using a texture and a color at the same time, the texture will be colorized using the color defined
		inline void set_color(const Color& color) {
			this->color = color;
		}

		// Set a new rotation angle
		inline void set_angle(const float angle) {
			this->angle = angle;
			this->isdirty = true;
		}


		// Get current mesh VAO.
		// If you are using this in a struct of a object, make it return a static VAO (like in cube.hpp).
		// If not, it will return by default the VAO setted inside the Mesh constructor (used when loading external obj)
		virtual inline const VAO& get_vao() {
			return *this->vao;
		}

	protected:
		void update_model();

		// Reference to some VAO
		const VAO* vao;
		// Not const because when loading external obj, is not possible to explicitly set
		uint32 indices_length;


		// Shape members //

		vec3<float> position = 0.0f;
		vec3<float> size = 1.0f;
		Color color = Colors::WHITE;
		float angle = 0.0f;

		// Texture will always be a "reference" to another existing texture
		Texture* texture = &this->get_deftex(); // Current texture being used

		// This need to be intialized on constructor, so the inheritance goes well
		glm::mat4 model;
		bool isdirty; // Change model if changed

		// Inline //

		// Default texture (solid white)
		inline Texture& get_deftex() const {
			// I don't like data being statically allocated but whatever
			static Texture def_tex = Texture(); // Make solid white texture
			return def_tex;
		}
};


// Not happy with this here, but whatever

// Load an external object and return it's vertices.
// Takes a path and a vector of indices to be populate
std::vector<Vertex> load_obj(const std::string& path, std::vector<uint32>& out_indices);
