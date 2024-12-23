#include "scarablib/scenes/scene3d.hpp"
#include <GL/glew.h>

Scene3D::Scene3D(const Window& window, Camera& camera) : Scene(window), camera(camera) {}

Scene3D::~Scene3D() {
	delete this->shader;
}

void Scene3D::add_to_scene(const std::string& key, Mesh* mesh) {
	if(!mesh) {
		throw ScarabError("Try to add null object '%s'", mesh);
	}

	this->scene.emplace(key, mesh);
	this->vao_groups[mesh->get_vao().get_id()].push_back(mesh);
}

void Scene3D::draw_all() const {
	this->shader->use();

	for(const auto& [vao, meshes] : this->vao_groups) {
		glBindVertexArray(vao);

		for(Mesh* mesh : meshes) {
			mesh->draw(this->camera, *this->shader);
			#ifdef SCARAB_DEBUG_DRAWCALL
			this->drawcalls += 1;
			#endif
		}
	}
	glBindVertexArray(0);

	this->shader->unbind();
}

