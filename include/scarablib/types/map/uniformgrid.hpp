#pragma once

#include "scarablib/types/raycast.hpp"
#include <vector>

class UniformGrid {
	public:
		// Creates a grid from a model.
		// Automatically calculates and adds the triangles to the grid
		UniformGrid(const char* path, const vec3<uint32>& griddims, const glm::mat4& transform = glm::mat4(1.0f)) noexcept;
		UniformGrid(const vec3<float>& worldmin, const vec3<float>& worldmax, const vec3<uint32>& griddims) noexcept;

		// Adds a triangle to the grid. It calculates which cells the triangle
		// overlaps and adds it to all of them.
		void add_triangle(const MeshTriangle& tri) noexcept;

		// Performs an optimized raycast against the grid.
		// It only checks for intersections with triangles in the cells the ray passes through
		Raycast::Rayhit raycast(const Raycast::Ray& ray) const noexcept;

	private:
		vec3<float> worldmin;
		vec3<float> worldsize;

		vec3<uint32> griddims;
		vec3<float> cellsize;

		std::vector<std::vector<MeshTriangle>> cells;

		// Returns the index of the cell the point is in
		vec3<uint32> get_cell_index(const vec3<float>& pos) const noexcept;
};
