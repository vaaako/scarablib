#include "scarablib/scenes/scene3d.hpp"
#include "scarablib/shapes/3d/cube.hpp"

Scene3D::Scene3D(const Window& window, Camera& camera) : Scene(window), camera(camera) {}

Scene3D::~Scene3D() {
	delete this->shader;
}

void Scene3D::draw_cube(Cube& cube) {
cube.draw(this->camera, *this->shader);
	// glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//
	// // Init matrices
	// glm::mat4 model = glm::mat4(1.0f);
	// glm::mat4 view = glm::mat4(1.0f);
	// glm::mat4 proj = glm::mat4(1.0f);
	//
	// // Model matrix //
	// model = glm::translate(model, glm::vec3(cube.position.x, cube.position.y, cube.position.z));
	// model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	// model = glm::scale(model, glm::vec3(cube.size.x, cube.size.y, cube.size.z)); // Resize
	//
	// // View matrix //
	// view = glm::lookAt(camera.position, (camera.position + camera.orientation), camera.up);
	//
	// // Add perspective
	// proj = glm::perspective(glm::radians(camera.fov),
	// 		(static_cast<float>(camera.get_width()) / static_cast<float>(camera.get_height())), camera.near_plane, camera.far_plane);
	//
	// this->shader->use();
	// // Apply changes
	// this->shader->set_matrix4f("mvp", proj * view * model);
	// this->shader->set_vector4f("shapeColor", { 1.0f, 1.0f, 1.0f, 1.0f });
	//
	// cube.vao->bind();
	// glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
	// cube.vao->unbind();
}
