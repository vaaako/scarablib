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
		// Computes the bounding box size using the model's vertices.
		// This initializes the bounding box in local space
		BoundingBox(const std::vector<Vertex>& vertices);
		~BoundingBox() noexcept;

		// Returns the 8 corners of the bounding box in world space.
		// These corners are used for rendering, collision detection, and transformations
		inline std::vector<vec3<float>> get_bounding_box_corners() const noexcept {
			return {
				vec3<float>(box_min.x, box_min.y, box_min.z),
				vec3<float>(box_min.x, box_min.y, box_max.z),
				vec3<float>(box_min.x, box_max.y, box_min.z),
				vec3<float>(box_min.x, box_max.y, box_max.z),
				vec3<float>(box_max.x, box_min.y, box_min.z),
				vec3<float>(box_max.x, box_min.y, box_max.z),
				vec3<float>(box_max.x, box_max.y, box_min.z),
				vec3<float>(box_max.x, box_max.y, box_max.z)
			};
		}

		// Updates the bounding box in world space using the model's transformation matrix.
		// This ensures the bounding box aligns with the model's position, scale, and rotation
		void update_world_bounding_box(const glm::mat4& model_matrix) noexcept;

		// Draws the bounding box as a wireframe for debugging purposes
		void draw(const Camera& camera, const Shader& shader, const glm::mat4& model) const noexcept;

	private:
		// Minimum and maximum corners of the bounding box (AABB).
		// These define the extents of the bounding box in local or world space
		vec3<float> box_min  = vec3<float>(1.0f); // Minimum corner of the bounding box
		vec3<float> box_max  = vec3<float>(1.0f); // Maximum corner of the bounding box
		vec3<float> box_size = vec3<float>(1.0f); // Size of the bounding box (max - min)

		// OpenGL
		size_t vao_hash; // Keep track of the hash being used
		uint32 vao_id;   // For drawing
		const GLsizei indices_size = 24;

		// Computes the bounding box in local space using the model's vertices.
		// This is called once during initialization
		void compute_bounding_box(const std::vector<Vertex>& vertices) noexcept;
};

