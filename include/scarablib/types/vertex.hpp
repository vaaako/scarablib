#pragma once

#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <vector>


struct Vertex2D {
	glm::vec2 position;
	glm::vec2 texuv;
};

// Used for setting coordinates for vertices position, texture and normals
struct Vertex {
	glm::vec3 position;
	glm::vec2 texuv;

	// bool operator==(const Vertex& other) const {
	// 	return this->position == other.position && this->texuv == other.texuv;
	// }
};

// Used for loading models
struct Face {
	// Init 3 spaces with zero
	std::vector<int> vertex_index = std::vector<int>(3);
	std::vector<int> texuv_index  = std::vector<int>(3);
};


// Be able to use Vertex as key in unordered_map
// namespace std {
// 	template<> struct hash<Vertex> {
// 		size_t operator()(const Vertex& vertex) const {
// 			size_t h1 = hash<float>()(vertex.position.x) ^
// 						(hash<float>()(vertex.position.y) << 1) ^
// 						(hash<float>()(vertex.position.z) << 1);
// 			size_t h2 = hash<float>()(vertex.texuv.x) ^
// 						(hash<float>()(vertex.texuv.y) << 1);
// 			// size_t h3 = hash<float>()(vertex.normal.x) ^
// 			// 			(hash<float>()(vertex.normal.y) << 1) ^
// 			// 			(hash<float>()(vertex.normal.z) << 1);
// 			// return h1 ^ (h2 << 1) ^ (h3 << 1);
// 			return h1 ^ (h2 << 1);
// 		}
// 	};
// }

