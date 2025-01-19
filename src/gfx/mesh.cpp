#include "scarablib/gfx/mesh.hpp"
#include "scarablib/proper/error.hpp"
#include <unordered_map>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tinyobjloader/tiny_obj_loader.h>

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32>& indices)
	: indices_length(static_cast<uint32>(indices.size())) {

	this->make_buffers(vertices, indices);
}

Mesh::Mesh(const GLuint& vao_id, const std::vector<Vertex>& vertices, const std::vector<uint32>& indices)
	: indices_length(static_cast<uint32>(indices.size())) {

	this->make_buffers(vao_id, vertices, indices);
}

Mesh::Mesh(const char* path) {
	// Data containers
	tinyobj::attrib_t attrib; // Mesh information
	std::vector<tinyobj::shape_t> shapes; // Mesh shapes
	std::vector<tinyobj::material_t> materials; // Mesh materials

	// Load obj
	std::string err;
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path);

	if(!err.empty()) {
		throw ScarabError("Error loading obj (%s): %s", path, err.c_str());
	}

	if(!ret) {
		throw ScarabError("Failed to load/parse (%s) file", path);
	}

	// Make and Reserve space
	std::vector<uint32> indices;
	size_t total_indices = 0;
	for(const tinyobj::shape_t& shape : shapes) {
		total_indices += shape.mesh.indices.size();
	}

	this->vertices.reserve(total_indices);
	indices.reserve(total_indices);

	// Decompress
	std::unordered_map<Vertex, uint32_t> uniq_vertices;
	for(const tinyobj::shape_t& shape : shapes) {
		for(const tinyobj::index_t& index : shape.mesh.indices) {
			// Check if vertex is not negative and is not out of bounds
			if(index.vertex_index < 0 || static_cast<size_t>(index.vertex_index * 3 + 2) >= attrib.vertices.size()) {
				throw ScarabError("Invalid vertex index (%d)", index.vertex_index);
			}

			// WARNING: Dont push normal by now

			// vec3<float> normal;
			// if(index.normal_index > 0) {
			// 	normal = {
			// 		attrib.normals[static_cast<size_t>(index.normal_index * 3)],
			// 		attrib.normals[static_cast<size_t>(index.normal_index * 3 + 1)],
			// 		attrib.normals[static_cast<size_t>(index.normal_index * 3 + 2)]
			// 	};
			// }

			vec2<float> texuv = { 0.0f, 0.0f };
			// Check if is valid and inside range
			if(index.texcoord_index >= 0 && static_cast<size_t>(index.texcoord_index * 2 + 1) < attrib.texcoords.size()) {
				texuv = {
					attrib.texcoords[static_cast<size_t>(index.texcoord_index * 2)],
					attrib.texcoords[static_cast<size_t>(index.texcoord_index * 2 + 1)],
				};
			}

			Vertex vertex = {
				.position = {
					attrib.vertices[static_cast<size_t>(index.vertex_index * 3)],
					attrib.vertices[static_cast<size_t>(index.vertex_index * 3 + 1)],
					attrib.vertices[static_cast<size_t>(index.vertex_index * 3 + 2)],
				},

				.texuv = texuv,
				// .normal = normal
			};

			// Push unique vertices only
			if(uniq_vertices.find(vertex) == uniq_vertices.end()) {
				uniq_vertices[vertex] = static_cast<uint32_t>(vertices.size());
				this->vertices.push_back(vertex);
			}
			indices.push_back(uniq_vertices[vertex]);
		}
	}

	this->indices_length = indices.size();
	this->make_buffers(this->vertices, indices);
}


Mesh::~Mesh() {
	delete this->vao;
	glDeleteBuffers(1, &this->vbo_id);
	glDeleteBuffers(1, &this->ebo_id);
}


void Mesh::set_texture(Texture* texture) {
	if(texture == nullptr){
		this->texture = &this->get_deftex();
		return;
	}

	this->texture = texture;
}

void Mesh::make_buffers(const std::vector<Vertex>& vertices, const std::vector<uint32>& indices) {
	this->get_vao().bind();

	// Gen and bind EBO
	glGenBuffers(1, &this->ebo_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizei>(indices.size() * sizeof(uint32)), indices.data(), GL_STATIC_DRAW);

	// Gen and bind VBO
	glGenBuffers(1, &this->vbo_id);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo_id);
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(Vertex)), vertices.data(), GL_STATIC_DRAW);
	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);
	// Normal
	// glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	// glEnableVertexAttribArray(1);
	// TexUV
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texuv));
	glEnableVertexAttribArray(1);

	// Unbind all
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::make_buffers(const GLuint& vao_id, const std::vector<Vertex>& vertices, const std::vector<uint32>& indices) {
	// Gen and bind VAO
	glBindVertexArray(vao_id);

	// Gen and bind EBO
	glGenBuffers(1, &this->ebo_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizei>(indices.size() * sizeof(uint32)), indices.data(), GL_STATIC_DRAW);

	// Gen and bind VBO
	glGenBuffers(1, &this->vbo_id);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo_id);
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(Vertex)), vertices.data(), GL_STATIC_DRAW);
	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);
	// Normal
	// glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	// glEnableVertexAttribArray(1);
	// TexUV
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texuv));
	glEnableVertexAttribArray(1);

	// Unbind all
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}



/*
// Not happy with this here, but whatever
std::vector<Vertex> Mesh::load_obj(const std::string& path, std::vector<uint32>& out_indices, vec3<float>& size) {
	if(StringHelper::file_extension(path) != "obj") {
		throw ScarabError("Only .obj files are supported");
	}

	std::ifstream file = std::ifstream(path);
	if(!file.is_open()) {
		throw ScarabError("File %s was not opened", path.c_str());
	}

	// Init with largest and smallest values
	vec3<float> min = vec3<float>(FLT_MAX);
	vec3<float> max = vec3<float>(-FLT_MAX);

	// To build vertices
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_texuvs;
	// std::vector<glm::vec3> temp_normals;

	// To build vertices and indices
	std::vector<Face> faces;

	std::string line;
	while(std::getline(file, line)) {
		std::istringstream iss = std::istringstream(line);
		std::string header; // First word

		iss >> header;

		// Case: v x y z
		if(header == "v") {
			glm::vec3 vertex;
			iss >> vertex.x >> vertex.y >> vertex.z;
			temp_vertices.push_back(vertex);

			// Update bounding box
			min = glm::min(min, vertex);
			max = glm::max(max, vertex);
		}

		// Case: vt u v
		else if(header == "vt") {
			glm::vec2 texuv;
			iss >> texuv.x >> texuv.y;
			temp_texuvs.push_back(texuv);
		}

		// Case: vn x y z


		// Case: faces, see below
		else if(header == "f") {
			std::string token;
			std::vector<std::string> face_tokens;

			// Get all tokens in face (split by space)
			while(iss >> token) {
				// printf("token: %s\n", (token).c_str());
				face_tokens.push_back(token);
			}

			// token example: 1/1/1 or 1 1 1 or 1/1 etc

			if(face_tokens.size() != 3) {
				throw ScarabError("Only triangular faces are supported. Try changing model export configuration");
			}


			// Cases:
			// 1: f v/vt/vn v/vt/vn v/vt/vn
			// 2: f v/vt v/vt v/vt
			// 3: f v//vn v//vn v//vn
			Face face;
			if(face_tokens.at(0).find('/') != std::string::npos) {
				for(size_t i = 0; i < face_tokens.size(); i++) {
					std::istringstream sub = std::istringstream(face_tokens.at(i));
					std::string str_vertex_index, str_texuv_index;

					// Get vertex and add to face
					std::getline(sub, str_vertex_index, '/');
					face.vertex_index[i] = std::stoi(str_vertex_index); 

					// Case 3 has no texture indice
					if(std::getline(sub, str_texuv_index, '/')) {
						face.texuv_index[i] = std::stoi(str_texuv_index);
					}

					// Case 2 has no normal map indice
					// if(std::getline(sub, str_normal_index, '/')) {
					// 	face.normal_index[i] = std::stoi(str_normal_index);
					// }
				}

			// Case: f v1 v2 v3
			} else {
				face.vertex_index[0] = std::stoi(face_tokens.at(0));
				face.vertex_index[1] = std::stoi(face_tokens.at(1));
				face.vertex_index[2] = std::stoi(face_tokens.at(2));
			}

			faces.push_back(face);

			// NOTE -- i could build vertices and indices here
		}
	} // end while loop

	file.close();

	// Calc size
	size = max - min;

	// Check if have texuvs
	bool has_texuv = !temp_texuvs.empty();

	// Build vertices and generate indices
	std::unordered_map<Vertex, uint32> vertex_map; // Store unique vertices and corresponding indices
	std::vector<Vertex> out_vertices;
	for(const Face& face : faces) {
		for(size_t i = 0; i < 3; i++) {
			// Build vertices
			Vertex vertex = {
				.position = temp_vertices.at(static_cast<uint32>(face.vertex_index.at(i) - 1)),

				// TODO: Build vt if dont has
				//
				// Check for case 2 or 3 of face
				.texuv = (has_texuv && face.texuv_index[i] != 0)
					// If have
					? temp_texuvs.at(static_cast<uint32>(face.texuv_index.at(i) - 1))
					// If dont
					: glm::vec2(0) 
				// Do the same for normal map
			};

			// Generate indices

			// Check if vertex is alredy calculated
			auto it = vertex_map.find(vertex);

			// Found
			if(it != vertex_map.end()) {
				uint32 index = it->second; // Use existing index ("newindex" below)
				out_indices.push_back(index);

			// Not found
			} else {
				out_vertices.push_back(vertex);

				// Calculate indice
				uint32 newindex = static_cast<uint32>(out_vertices.size() - 1);
				out_indices.push_back(newindex);
				vertex_map[vertex] = newindex; // Register vertex
			}
		}
	}

	return out_vertices;
}
*/
