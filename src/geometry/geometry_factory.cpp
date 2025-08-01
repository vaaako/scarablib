#include "scarablib/geometry/geometry_factory.hpp"

std::vector<Vertex> GeometryFactory::make_cube_vertices() noexcept {
	return {
		// Vertices: 24
		// Front face
		Vertex { .position = glm::vec3(-0.5f, -0.5f, 0.5f), .texuv = glm::vec2(0.0f, 0.0f) }, // Bottom left
		Vertex { .position = glm::vec3( 0.5f, -0.5f, 0.5f), .texuv = glm::vec2(1.0f, 0.0f) }, // Bottom right
		Vertex { .position = glm::vec3( 0.5f,  0.5f, 0.5f), .texuv = glm::vec2(1.0f, 1.0f) }, // Top right
		Vertex { .position = glm::vec3(-0.5f,  0.5f, 0.5f), .texuv = glm::vec2(0.0f, 1.0f) }, // Top left

		// Back face
		Vertex { .position = glm::vec3(-0.5f, -0.5f, -0.5f), .texuv = glm::vec2(1.0f, 0.0f) }, // 4
		Vertex { .position = glm::vec3(-0.5f,  0.5f, -0.5f), .texuv = glm::vec2(1.0f, 1.0f) }, // 5
		Vertex { .position = glm::vec3( 0.5f,  0.5f, -0.5f), .texuv = glm::vec2(0.0f, 1.0f) }, // 6
		Vertex { .position = glm::vec3( 0.5f, -0.5f, -0.5f), .texuv = glm::vec2(0.0f, 0.0f) }, // 7

		// Top face
		Vertex { .position = glm::vec3(-0.5f, 0.5f, -0.5f), .texuv = glm::vec2(0.0f, 1.0f) }, // 8
		Vertex { .position = glm::vec3(-0.5f, 0.5f,  0.5f), .texuv = glm::vec2(0.0f, 0.0f) }, // 9
		Vertex { .position = glm::vec3( 0.5f, 0.5f,  0.5f), .texuv = glm::vec2(1.0f, 0.0f) }, // 10
		Vertex { .position = glm::vec3( 0.5f, 0.5f, -0.5f), .texuv = glm::vec2(1.0f, 1.0f) }, // 11

		// Bottom face
		Vertex { .position = glm::vec3(-0.5f, -0.5f, -0.5f), .texuv = glm::vec2(0.0f, 0.0f) }, // 12
		Vertex { .position = glm::vec3( 0.5f, -0.5f, -0.5f), .texuv = glm::vec2(1.0f, 0.0f) }, // 13
		Vertex { .position = glm::vec3( 0.5f, -0.5f,  0.5f), .texuv = glm::vec2(1.0f, 1.0f) }, // 14
		Vertex { .position = glm::vec3(-0.5f, -0.5f,  0.5f), .texuv = glm::vec2(0.0f, 1.0f) }, // 15

		// Right face
		Vertex { .position = glm::vec3(-0.5f, -0.5f, -0.5f), .texuv = glm::vec2(0.0f, 0.0f) }, // 16
		Vertex { .position = glm::vec3(-0.5f, -0.5f,  0.5f), .texuv = glm::vec2(1.0f, 0.0f) }, // 17
		Vertex { .position = glm::vec3(-0.5f,  0.5f,  0.5f), .texuv = glm::vec2(1.0f, 1.0f) }, // 18
		Vertex { .position = glm::vec3(-0.5f,  0.5f, -0.5f), .texuv = glm::vec2(0.0f, 1.0f) }, // 19

		// Left face
		Vertex { .position = glm::vec3(0.5f, -0.5f, -0.5f), .texuv = glm::vec2(1.0f, 0.0f) }, // 20
		Vertex { .position = glm::vec3(0.5f,  0.5f, -0.5f), .texuv = glm::vec2(1.0f, 1.0f) }, // 21
		Vertex { .position = glm::vec3(0.5f,  0.5f,  0.5f), .texuv = glm::vec2(0.0f, 1.0f) }, // 22
		Vertex { .position = glm::vec3(0.5f, -0.5f,  0.5f), .texuv = glm::vec2(0.0f, 0.0f) }  // 23
	};
}


std::pair<std::vector<Vertex>, std::vector<uint8>> GeometryFactory::make_cube_faces(const uint8 face_mask) noexcept {
	// Source vertices
	const std::vector<Vertex> vertices = GeometryFactory::make_cube_vertices();

	// New vertices
	std::vector<Vertex> final_vertices;
	std::vector<uint8> indices;

	// Indices for one face
	constexpr uint8 indices_index[6] = { 0, 1, 2, 0, 2, 3 };

	// Offset of one mesh to another
	uint32 vertex_offset = 0;

	// Each face adds 6 vertices, 6 indices
	for(uint32 v = 0; v < 6; v++) {
		// Hide face if included
		if((face_mask & (1 << v)) == 0) {
			continue;
		}

		// Where this face's vertices are in cube vertices
		const uint32 start_index = v * 4; // Vertices per face

		final_vertices.insert(
			final_vertices.end(),              // Insert at the end
			vertices.begin() + start_index,    // Start copying from here
			vertices.begin() + start_index + 4 // Stop copying here
		);

		// Build indices
		for(int i = 0; i < 6; i++) {
			indices.push_back(static_cast<uint8>(indices_index[i] + vertex_offset));
		}
		// Amount of vertices to skip (next face)
		vertex_offset += 4;
	}

	return {
		std::move(final_vertices),
		std::move(indices)
	};
}


std::vector<Vertex> GeometryFactory::make_plane_vertices() noexcept {
	return {
		// Front face
		Vertex { .position = glm::vec3(-0.5f, -0.5f, 0.0f), .texuv = glm::vec2(0.0f, 0.0f) }, // Bottom left
		Vertex { .position = glm::vec3( 0.5f, -0.5f, 0.0f), .texuv = glm::vec2(1.0f, 0.0f) }, // Bottom right
		Vertex { .position = glm::vec3( 0.5f,  0.5f, 0.0f), .texuv = glm::vec2(1.0f, 1.0f) }, // Top right
		Vertex { .position = glm::vec3(-0.5f,  0.5f, 0.0f), .texuv = glm::vec2(0.0f, 1.0f) }, // Top left
	};
}


std::vector<Vertex> GeometryFactory::make_rectangle_vertices() noexcept {
	return {
		// Central vertex
		Vertex { .position = glm::vec3(0.0f, 0.0f, 0.0f), .texuv = glm::vec2(0.0f, 1.0f) }, // Bottom Left
		// CW fan around center
		Vertex { .position = glm::vec3(0.0f, 1.0f, 0.0f), .texuv = glm::vec2(0.0f, 0.0f) }, // Top Left
		Vertex { .position = glm::vec3(1.0f, 1.0f, 0.0f), .texuv = glm::vec2(1.0f, 0.0f) }, // Top Right
		Vertex { .position = glm::vec3(1.0f, 0.0f, 0.0f), .texuv = glm::vec2(1.0f, 1.0f) }, // Bottom Right
	};
}

std::vector<Vertex> GeometryFactory::make_triangle_vertices() noexcept {
	return {
		// CCW
		Vertex { .position = glm::vec3(0.5f, 0.0f, 0.0f), .texuv = glm::vec2(0.5f, 1.0f) }, // Middle
		Vertex { .position = glm::vec3(0.0f, 1.0f, 0.0f), .texuv = glm::vec2(0.0f, 0.0f) }, // Bottom Left
		Vertex { .position = glm::vec3(1.0f, 1.0f, 0.0f), .texuv = glm::vec2(1.0f, 0.0f) }  // Bottom Right
	};
}

