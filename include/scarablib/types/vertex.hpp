#pragma once

#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <vector>

/* NOTE
 * I am just using union here so when making Mesh for 2D Shape, I can use "position2d" and "vec2"
 * so it is a bit less confusing.
 *  
 * However, since union members share the same memory space, when using on OpenGL
 * is used as a vec3 anyway, because OpenGL read the data in memory, not the data itself.
 * This was leading to the shape2D not be drawing correctly
 * (since it ignored the 2D offset and used the 3D offset, using texuv as vertice)
 *  
 * To solve this, I am using shape2d as 3D anyway, and just using XY and ignoring Z
 * even in the shader I had to change aPos to vec3
 *  
 * So the only use for this union is the label and use vec2
 * (even though is later read as vec3)
 */



// Used for setting coordinates for vertices position, texture and normals
struct Vertex {
	union {
		glm::vec3 position;
		glm::vec2 position2d;
	};

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

