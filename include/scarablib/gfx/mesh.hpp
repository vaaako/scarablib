#pragma once

#include <GL/glew.h>
#include "scarablib/opengl/shader.hpp"
#include "scarablib/opengl/vao.hpp"
#include "scarablib/opengl/vbo.hpp"
#include "scarablib/scenes/camera.hpp"
#include "scarablib/typedef.hpp"
#include "scarablib/types/texture.hpp"
#include "scarablib/types/vertex.hpp"


// Struct used to initialize a Mesh
struct MeshConf {
	vec3<float> position;
	vec3<float> size = vec3<float>(1.0f); // This will change
	vec3<float> scale = vec3<float>(1.0f);
	Color color = Colors::WHITE;

	// min and max corner of the bounding box (AABB)
	// This will be calculated on constructor
	vec3<float> min = vec3<float>(0.0f);
	vec3<float> max = vec3<float>(0.0f);

	// Orientation (default angle of the Mesh)
	
	// Orientation of the Mesh
	float orient_angle = 0.0f;
	// Axis where the orientation angle will be applied
	vec3<float> orient_axis = { 1.0f, 0.0f, 0.0f };

	// Rotation based on orientation
	float angle = 0.0f;
	// Axis where the rotation will be applied
	vec3<float> axis = { 1.0f, 0.0f, 0.0f };
};

// An object used for as a base for 3D Shapes
class Mesh {
	public:
		// Make a mesh from existing VAO and initial config
		Mesh(const MeshConf& conf, const VAO* vao, const uint32 indices_length = 0);
		Mesh(const char* path);

		// Draw current mesh.
		// It needs a camera object and a shader
		virtual void draw(Camera& camera, const Shader& shader);

		// Get current texture
		inline Texture get_texture() const {
			return *this->texture;
		}

		// Get current X position
		inline float get_x() const {
			return this->conf.position.x;
		}

		// Get current Y position
		inline float get_y() const {
			return this->conf.position.y;
		}

		// Get current Z position
		inline float get_z() const {
			return this->conf.position.z;
		}

		// Get current position
		inline vec3<float> get_position() const {
			return this->conf.position;
		}

		// Get current size of each axis
		inline vec3<float> get_size() const {
			return this->conf.size;
		}

		// Get current scale of each axis
		inline vec3<float> get_scale() const {
			return this->conf.scale;
		}

		// Get current color
		inline Color get_color() const {
			return this->conf.color;
		}

		// Get current angle
		inline float get_angle() const {
			return this->conf.angle;
		}


		// Get minimum corner of the bounding box (AABB)
		inline vec3<float> get_min() const {
			return this->conf.min;
		}

		// Get maximum corner of the bounding box (AABB)
		inline vec3<float> get_max() const {
			return this->conf.max;
		}



		// SETTERS //

		// Set a texture to be used
		Mesh& set_texture(Texture* texture);

		// Removes the shape's texture
		inline Mesh& remove_texture() {
			this->texture = &this->get_deftex();
			return *this;
		}

		// Set a new position using a vector
		inline Mesh& set_position(const vec3<float>& position) {
			this->conf.position = position;
			this->update_min_and_max();
			this->isdirty = true;
			return *this;
		}

		// Set a new X position
		inline Mesh& set_x(const float newx) {
			this->conf.position.x = newx;
			this->isdirty = true;
			return *this;
		}

		// Set a new Y position
		inline Mesh& set_y(const float newy) {
			this->conf.position.y = newy;
			this->isdirty = true;
			return *this;
		}

		// Set a new Z position
		inline Mesh& set_z(const float newz) {
			this->conf.position.z = newz;
			this->isdirty = true;
			return *this;
		}

		// Set a new size using a vector (X and Y)
		inline Mesh& set_size(const vec3<float>& size) {
			this->conf.size = size;
			this->update_min_and_max();
			this->isdirty = true;
			return *this;
		}

		// Set a new size using a vector (X and Y)
		inline Mesh& set_size(const float size) {
			this->conf.size = { size, size, size };
			this->update_min_and_max();
			this->isdirty = true;
			return *this;
		}

		// Scale the shape using a different value for each axis.
		// e.g., `size.x = size.x * scale.x`, `size.y = size.y * scale.y` and `size.z = size.z * scale.z`
		inline Mesh& set_scale(const vec3<float>& scale) {
			this->conf.scale = scale;
			this->isdirty = true;
			return *this;
		}

		// Scale the shape using a different value for each axis.
		// e.g., `size.x = size.x * scale.x`, `size.y = size.y * scale.y` and `size.z = size.z * scale.z`
		inline Mesh& set_scale(const float scale) {
			this->conf.scale = { scale, scale, scale };
			this->isdirty = true;
			return *this;
		}

		// Set a new color
		// If using a texture and a color at the    std::cout << "Cube Size: " << size.x << " x " << size.y << " x " << size.z << std::endl; same time, the texture will be colorized using the color defined
		inline Mesh& set_color(const Color& color) {
			this->conf.color = color;
			return *this;
		}

		// Set a new rotation angle in degrees.
		// Axis is wich axis the angle will be applied (XYZ)
		Mesh& set_rotation(const float angle, const vec3<bool> axis);

		// Set a new orientation to the Mesh in degrees.
		// Orientation is the "default" angle of the Mesh.
		// The rotation will be applied using the orientation as base
		Mesh& set_orientation(const float angle, const vec3<bool> axis);

		inline Mesh make_instance() const {
			return Mesh(*this);
		}

		// Get current mesh VAO.
		// If you are using this in a struct of a object, make it return a static VAO (like in cube.hpp).
		// If not, it will return by default the VAO setted inside the Mesh constructor (used when loading external obj)
		virtual inline const VAO& get_vao() const {
			return *this->vao;
		}

	protected:
		const VAO* vao; // Reference to some VAO
		// const VBO* vbo;
		uint32 indices_length; // Not const because when loading external obj, is not possible to explicitly set

		glm::mat4 model = glm::mat4(1.0f);
		bool isdirty; // Calculate matrix if anything changed (initializing on constructor so inheritance works)

		// Storing config
		MeshConf conf;

		// Texture will always be a "reference" to another existing texture
		Texture* texture = &this->get_deftex(); // Current texture being used
		GLuint last_tex_id = this->texture->get_id();

		// Inline //

		// Default texture (solid white)
		inline Texture& get_deftex() const {
			// I don't like data being statically allocated but whatever
			static Texture def_tex = Texture(); // Make solid white texture
			return def_tex;
		}

		inline void update_min_and_max() {
			// Scale or Size?
			this->conf.min = vec3<float>(
				this->conf.position.x - this->conf.scale.x / 2,
				this->conf.position.y - this->conf.scale.y / 2,
				this->conf.position.z - this->conf.scale.z / 2
			);

			this->conf.max = vec3<float>(
				this->conf.position.x + this->conf.scale.x / 2,
				this->conf.position.y + this->conf.scale.y / 2,
				this->conf.position.z + this->conf.scale.z / 2
			);
		}
};


// Not happy with this here, but whatever
// TODO: Put this in some class or namespace

// Load an external object and return it's vertices.
// Takes a path and a vector of indices to be populate
std::vector<Vertex> load_obj(const std::string& path, std::vector<uint32>& out_indices, vec3<float>& size);
// Calculates the size of a shape using it's vertices
vec3<float> calc_size(const std::vector<Vertex>& vertices);
