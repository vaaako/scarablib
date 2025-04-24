#pragma once

#include "scarablib/types/vertex.hpp"

// Functions that returns the vertices for different geometries
namespace GeometryFactory {
	// Returns the vertices for all faces of a cube
	std::vector<Vertex> make_cube_vertices() noexcept;
	// Returns the vertices for a plane
	std::vector<Vertex> make_plane_vertices() noexcept;

	// Returns the vertices for a rectangle
	std::vector<Vertex> make_rectangle_vertices() noexcept;
	// Returns the vertices for a triangle
	std::vector<Vertex> make_triangle_vertices() noexcept;
}
