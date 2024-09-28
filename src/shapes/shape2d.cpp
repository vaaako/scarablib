#include "scarablib/shapes/shape2d.hpp"
#include "scarablib/opengl/scoped_bind.hpp"
#include "scarablib/proper/error.hpp"
#include "scarablib/opengl/vao.hpp"
#include "scarablib/opengl/vbo.hpp"
#include "scarablib/opengl/ebo.hpp"
#include <optional>


#define POSITION_DIMENSION 2 // 2D
#define TEXTURE_DIMENSION 2
// #define NORMALMAP_DIMENSION 3

// TODO -- Making this every new shape is not correct (this is just for tests yet)
Shape2D::Shape2D(const std::vector<Vertex>& vertices, const std::vector<uint32>& indices) 
	: indices_size(static_cast<GLsizei>(indices.size())) {

	// LOG_INFO("Indicies size: %d", this->indices_size);
	if(vertices.empty() || indices.empty()) {
		throw ScarabError("\"vertices\" or \"indices\" of Shape2D is empty");
	}

	// Calculate total dimension size
	const int total_size = (POSITION_DIMENSION + TEXTURE_DIMENSION) * sizeof(float); // need to be float

	// Bind vao to make all changes
	this->vao->bind();

	// Create VBO and EBO
	VBO vbo = VBO();
	EBO ebo = EBO(indices);

	// Build VBO
	vbo.alloc_data(vertices.size() * sizeof(Vertex), vertices.data());
	vbo.link_attrib(0, POSITION_DIMENSION, total_size, 0);
	vbo.link_attrib(1, TEXTURE_DIMENSION, total_size, POSITION_DIMENSION);

	// Unbind all
	this->vao->unbind(); // VAO must unbind first
	vbo.unbind();
	ebo.unbind();
}


Shape2D::~Shape2D() {
	delete this->vao;
	// NOTE -- texture in this case is actually a "reference"
}


// TODO -- model math and shapeColor should only be done when values are changed
//   + Maybe moving this for each equivalent method?
void Shape2D::draw(const Shader& shader, const DrawMode drawmode, const DrawType drawtype) const {
	glPolygonMode(GL_FRONT_AND_BACK, drawmode);
	shader.use();

	glm::mat4 model = glm::mat4(1.0f);

	// Apply transformations (move, resize, rotate)
	model = glm::translate(model, glm::vec3(this->position.x, this->position.y, 0.0f))         // Translate position
		  * glm::translate(model, glm::vec3(0.5f * this->size.x, 0.5f * this->size.y, 0.0f))   // Origin from top-lef to center (to apply rotation on center)
		  * glm::rotate(model, glm::radians(this->angle), glm::vec3(0.0f, 0.0f, 1.0f))         // Rotate
		  * glm::translate(model, glm::vec3(-0.5f * this->size.x, -0.5f * this->size.y, 0.0f)) // Origin back to top-left (Back to top-left to apply resize)
		  * glm::scale(model, glm::vec3(this->size.x, this->size.y, 1.0f));                    // Resize

	// Set the model matrix
	shader.set_matrix4f("model", model);

	// Set the color
	shader.set_vector4f("shapeColor", color.normalize());


	// Bind VAO using RAII
	ScopedBind<VAO> scoped_vao_bind(*this->vao);

	// RAII for texture binding
	std::optional<ScopedBind<Texture>> scoped_texture_bind;

	// TODO -- Put this check on shader
	if(this->texture != nullptr) {
		scoped_texture_bind.emplace(*this->texture);
	}

	glDrawElements(drawtype, this->indices_size, GL_UNSIGNED_INT, (void*)0);

	// scoped_vao_bind and scoped_texture_bind (if binded) are automatically unbound when exiting this scope
}

