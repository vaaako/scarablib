#pragma once

#include "scarablib/opengl/bufferbundle.hpp"
#include "scarablib/opengl/shader.hpp"
#include "scarablib/scenes/camera.hpp"

class Model;

// This class is for the bounding box of a 3D model.
// It can be used for hitbox and collision detection
class BoundingBox {
	// Acess min, max and value
	friend Model;

	public:
		struct Size {
			vec3<float> size; // Minimum corner of the bounding box
			vec3<float> min;  // Maximum corner of the bounding box
			vec3<float> max;  // Size of the bounding box (max - min)
		};

		// Computes the bounding box size using the model's size, min and max
		// This initializes the bounding box in local space.
		// NOTE: Boxsize will have values changed in update_world_bounding_box.
		// NOTE: Model matrix will is used just for debug (draw method)
		BoundingBox(BoundingBox::Size& boxsize, const glm::mat4& model_matrix) noexcept;
		~BoundingBox() noexcept;

		// Returns the 8 corners of the bounding box in world space.
		// These corners are used for rendering, collision detection, and transformations
		inline std::vector<vec3<float>> get_bounding_corners() const noexcept {
			return {
				vec3<float>(this->boxsize.min.x, this->boxsize.min.y, this->boxsize.min.z),
				vec3<float>(this->boxsize.min.x, this->boxsize.min.y, this->boxsize.max.z),
				vec3<float>(this->boxsize.min.x, this->boxsize.max.y, this->boxsize.min.z),
				vec3<float>(this->boxsize.min.x, this->boxsize.max.y, this->boxsize.max.z),
				vec3<float>(this->boxsize.max.x, this->boxsize.min.y, this->boxsize.min.z),
				vec3<float>(this->boxsize.max.x, this->boxsize.min.y, this->boxsize.max.z),
				vec3<float>(this->boxsize.max.x, this->boxsize.max.y, this->boxsize.min.z),
				vec3<float>(this->boxsize.max.x, this->boxsize.max.y, this->boxsize.max.z)
			};
		}

		// Updates the bounding box in world space using the model's transformation matrix.
		// This ensures the bounding box aligns with the model's position, scale, and rotation
		void update_world_bounding_box(const glm::mat4& model_matrix) noexcept;

		// Draws the bounding box as a wireframe for debugging purposes
		void draw(const Camera& camera, const Shader& shader) const noexcept;

		// Computes the bounding box in local space using the model's vertices.
		static BoundingBox::Size calculate_size_from_vertices(const std::vector<Vertex>& vertices) noexcept {
			// Init with largest and smallest values
			BoundingBox::Size boxsize {
				.size = vec3<float>(1.0f),
				.min = vec3<float>(FLT_MAX),
				.max = vec3<float>(-FLT_MAX)
			};

			for(const Vertex& vertex : vertices) {
				boxsize.min = glm::min(boxsize.min, vertex.position);
				boxsize.max = glm::max(boxsize.max, vertex.position);
			}

			// Model's boxsize
			boxsize.size = boxsize.max - boxsize.min;
			return boxsize;
		}

	private:
		// Minimum and maximum corners of the bounding box (AABB).
		// These define the extents of the bounding box in local or world space
		BoundingBox::Size& boxsize; // Reference so it doesn't need to be copied

		// Updates on init and compute_bounding_box
		glm::mat4 model;

		// OpenGL
		BufferBundle bundle = BufferBundle(); // Bundle of VAO, VBO and EBO
		static constexpr GLsizei indices_size = 24;
};

