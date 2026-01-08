#pragma once

#include "scarablib/geometry/submesh.hpp"
#include "scarablib/opengl/vertexarray.hpp"
#include <filesystem>
#include <vector>

namespace ScarabModel {
	// Load a wavefront-obj file and return all submeshes and a VAO from submeshes
	std::pair<std::vector<SubMesh>, std::shared_ptr<VertexArray>> load_obj(const char* path);

	// Load a wavefront-obj file and return the overall Vertices and Indices
	// Deprecated
	std::pair<std::vector<Vertex>, std::vector<uint32>> load_obj_old(const char* path);

	// Sometimes the texname inside MTL contains a "/" at the start
	inline std::filesystem::path treat_texname(const std::filesystem::path& path) {
		return (path.is_absolute()) ? path.relative_path() : path;
	}
};
