#include "scarablib/gfx/3d/boundingbox.hpp"
#include "scarablib/opengl/vao_manager.hpp"

BoundingBox::BoundingBox(BoundingBox::Size& boxsize, const glm::mat4& model_matrix) noexcept
	: boxsize(boxsize), model(model_matrix) {

	static std::vector<uint32> indices = {
		0, 1, 1, 3, 3, 2, 2, 0, // Bottom face
		4, 5, 5, 7, 7, 6, 6, 4, // Top face
		0, 4, 1, 5, 2, 6, 3, 7  // Vertical edges
	};

	// Get hash and create VAO
	this->vao_hash = VAOManager::get_instance().compute_hash(this->get_bounding_corners(), indices);
	this->vao_id = VAOManager::get_instance().make_vao(
		this->get_bounding_corners(), indices
	);
}

BoundingBox::~BoundingBox() noexcept {
	// Release current vao
	VAOManager::get_instance().release_vao(vao_hash);
}

void BoundingBox::update_world_bounding_box(const glm::mat4& model_matrix) noexcept {
	this->model = model_matrix;

	// Transform all 8 corners of the local bounding box
	std::vector<vec3<float>> corners = this->get_bounding_corners();

	// Compute new world-space bounding box
	this->boxsize.min = vec3<float>(FLT_MAX);
	this->boxsize.max = vec3<float>(-FLT_MAX);

	for(const vec3<float>& corner : corners) {
		vec3<float> world_corner = model_matrix * vec4<float>(corner, 1.0f);
		this->boxsize.min = glm::min(this->boxsize.min, world_corner);
		this->boxsize.max = glm::max(this->boxsize.max, world_corner);
	}

	this->boxsize.size = this->boxsize.max - this->boxsize.min; // Update bounding box size
}

void BoundingBox::draw(const Camera& camera, const Shader& shader) const noexcept {
	shader.use();
	// If used the model matrix, the box would have the model's "new model", which would not be corresponding
	shader.set_matrix4f("mvp", camera.get_proj_matrix() * camera.get_view_matrix() * this->model);
	// shader.set_color("shapeColor", Colors::RED); // Will not work, since its using the shader with texture

	glBindVertexArray(this->vao_id);
	glDrawElements(GL_LINES, this->indices_size, GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);
}

