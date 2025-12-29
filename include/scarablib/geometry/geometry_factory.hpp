#pragma once

#include "scarablib/geometry/vertex.hpp"
#include <utility>

// Functions that returns the vertices for different geometries
namespace GeometryFactory {
	// Returns the vertices for all faces of a cube
	std::vector<Vertex> make_cube_vertices() noexcept;
	// Returns a pair containing the vertices and indices from a cube
	std::pair<std::vector<Vertex>, std::vector<uint8>> make_cube_faces(const uint8 face_mask) noexcept;
	// Returns the vertices for a plane
	std::vector<Vertex> make_plane_vertices() noexcept;
	// Returns the vertices for a crossed quad
	std::vector<Vertex> make_crossedplane_vertices() noexcept;
	// Returns the vertices for a four crossed quad
	std::vector<Vertex> make_fourcrossedplane_vertices() noexcept;

	// Returns the vertices for a rectangle
	std::vector<Vertex2D> make_rectangle_vertices() noexcept;
	// Returns the vertices for a triangle
	std::vector<Vertex2D> make_triangle_vertices() noexcept;
}
