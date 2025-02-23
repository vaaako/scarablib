#pragma once

#include "scarablib/opengl/shader.hpp"
#include "scarablib/scenes/camera.hpp"
#include "scarablib/types/vertex.hpp"

class Model;

// This class is for the bounding box of a 3D model.
// It can be used for hitbox and collision detection
class BoundingBox {
	// Acess min, max and value
	friend Model;

	public:
		// Computes the bounding box size using the model's model matrix.
		// This initializes the bounding box in local space.
		// NOTE: Boxsize will have values changed in update_world_bounding_box.
		// NOTE: Model matrix will is used just for debug (draw method)
		BoundingBox(BoundingBox::Size& boxsize, const glm::mat4& model_matrix) noexcept;
		~BoundingBox() noexcept;

		// Computes the bounding box in local space using the model's vertices.
		// This is called once during initialization
		void compute_bounding_box(const std::vector<Vertex>& vertices) noexcept;

		// Returns the 8 corners of the bounding box in world space.
		// These corners are used for rendering, collision detection, and transformations
		inline std::vector<vec3<float>> get_bounding_corners() const noexcept {
			return {
				vec3<float>(this->min.x, this->min.y, this->min.z),
				vec3<float>(this->min.x, this->min.y, this->max.z),
				vec3<float>(this->min.x, this->max.y, this->min.z),
				vec3<float>(this->min.x, this->max.y, this->max.z),
				vec3<float>(this->max.x, this->min.y, this->min.z),
				vec3<float>(this->max.x, this->min.y, this->max.z),
				vec3<float>(this->max.x, this->max.y, this->min.z),
				vec3<float>(this->max.x, this->max.y, this->max.z)
			};
		}

		// Updates the bounding box in world space using the model's transformation matrix.
		// This ensures the bounding box aligns with the model's position, scale, and rotation
		void update_world_bounding_box(const glm::mat4& model_matrix) noexcept;

		// Draws the bounding box as a wireframe for debugging purposes
		void draw(const Camera& camera, const Shader& shader) const noexcept;

	private:
		// Minimum and maximum corners of the bounding box (AABB).
		// These define the extents of the bounding box in local or world space
		BoundingBox::Size& boxsize; // Reference so it doesn't need to be copied

		// Updates on init and compute_bounding_box
		glm::mat4 model;

		// OpenGL
		size_t vao_hash; // Keep track of the hash being used
		uint32 vao_id;   // For drawing
		const GLsizei indices_size = 24;
};

