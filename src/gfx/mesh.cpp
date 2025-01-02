#include "scarablib/gfx/mesh.hpp"
#include "scarablib/opengl/ebo.hpp"
#include "scarablib/opengl/vao.hpp"
#include "scarablib/opengl/vbo.hpp"
#include "scarablib/proper/error.hpp"
#include "scarablib/utils/string.hpp"
#include <fstream>
#include <sstream>
#include <unordered_map>

Mesh::Mesh(const MeshConf& conf, const VAO* vao, const uint32 indices_length)
	: vao(vao), indices_length(indices_length), isdirty(true),
	conf(conf) {

	this->update_min_and_max();
}

Mesh::Mesh(const char* path) {
	std::vector<uint32> indices;
	std::vector<Vertex> vertices = load_obj(path, indices, this->conf.size);

	this->indices_length = static_cast<uint32>(indices.size());

	this->vao = new VAO();
	this->vao->bind();

	// Gen VBO and EBO
	VBO vbo = VBO();
	EBO ebo = EBO(indices); // segfault here idk

	vbo.bind();
	vbo.make_from_vertex(vertices, 3);

	// Unbind vao
	this->vao->unbind();
	vbo.unbind();
	ebo.unbind();
}

Mesh& Mesh::set_texture(Texture* texture) {
	if(texture == nullptr){
		this->texture = &this->get_deftex();
		return *this;
	}

	this->texture = texture;
	return *this;
}

Mesh& Mesh::set_rotation(const float angle, const vec3<bool> axis) {
	this->conf.angle = angle;
	// At least one axis need to be true to work
	if(axis == vec3<bool>(false)) {
		this->conf.axis = (vec3<float>)axis;
		return *this;
	}
	this->conf.axis = (vec3<float>)axis;
	this->isdirty = true;
	return *this;
}

Mesh& Mesh::set_orientation(const float angle, const vec3<bool> axis) {
	this->conf.orient_angle = angle;
	this->conf.orient_axis = (vec3<float>)axis;

	// At least one axis need to be true to work
	if(axis == vec3<bool>(false)) {
		return *this;
	}

	this->isdirty = true;
	return *this;
}

void Mesh::draw(Camera& camera, const Shader& shader) {
	if(this->isdirty) {
		this->model = glm::mat4(1.0f);

		// Model matrix
		this->model = glm::translate(this->model, this->conf.position)
					* glm::rotate(this->model, glm::radians(this->conf.orient_angle), this->conf.orient_axis)
					* glm::rotate(this->model, glm::radians(this->conf.angle), this->conf.axis)
					* glm::scale(this->model, this->conf.scale);

		this->isdirty = false;
	}
	
	// View matrix
	glm::mat4 view = camera.get_view_matrix();

	// Add perspective
	glm::mat4 proj = camera.get_proj_matrix();

	// NOTE: "is dirty" for color wouldn't work because would set the last color updated for all meshes (using this later maybe)
	shader.set_color("shapeColor", this->conf.color);
	shader.set_matrix4f("mvp", (proj * view) * this->model);

	this->texture->bind();
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(this->indices_length), GL_UNSIGNED_INT, (void*)0);
	this->texture->unbind();
}


vec3<float> calc_size(const std::vector<Vertex>& vertices) {
	// Init with largest and smallest values
	vec3<float> min = vec3<float>(FLT_MAX);
	vec3<float> max = vec3<float>(-FLT_MAX);

	for(const Vertex& vertex : vertices) {
		min = glm::min(min, vertex.position);
		max = glm::max(max, vertex.position);
	}

	return max - min;
}

// Not happy with this here, but whatever
std::vector<Vertex> load_obj(const std::string& path, std::vector<uint32>& out_indices, vec3<float>& size) {
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

			/*
			* Cases:
			* 1: f v/vt/vn v/vt/vn v/vt/vn
			* 2: f v/vt v/vt v/vt
			* 3: f v//vn v//vn v//vn
			*/
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
