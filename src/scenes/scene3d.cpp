#include "scarablib/scenes/scene3d.hpp"
#include <GL/glew.h>

Scene3D::Scene3D(const Window& window, Camera& camera) : Scene(window), camera(camera) {}

Scene3D::~Scene3D() {
	delete this->shader;
}

void Scene3D::add_to_scene(const std::string& key, Mesh* mesh) {
	if(!mesh) {
		throw ScarabError("Attempted to add a null Mesh to the scene with key '%s'", key.c_str());
	}

	std::shared_ptr<Mesh> shared_mesh = std::shared_ptr<Mesh>(mesh);
	this->scene.emplace(key, shared_mesh); // will not be used here, but is used for get_by_key()
	this->vao_groups[mesh->get_vao().get_id()].push_back(shared_mesh);
}

void Scene3D::draw_all() const {
	this->shader->use();

	for(const auto& [vao, meshes] : this->vao_groups) {
		glBindVertexArray(vao);

		for(std::shared_ptr<Mesh> mesh : meshes) {
			mesh->draw(this->camera, *this->shader);
		}
	}
	glBindVertexArray(0);

	this->shader->unbind();
}

