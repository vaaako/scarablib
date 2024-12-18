#include "scarablib/scenes/scene3d.hpp"
#include <iostream>

Scene3D::Scene3D(const Window& window, Camera& camera) : Scene(window), camera(camera) {}

Scene3D::~Scene3D() {
	delete this->shader;
}

void Scene3D::draw_mesh(Mesh& shape) {
	// glEnable(GL_DEPTH_TEST); // I couldn't find out why Font gets a background when this is enabled and why 2D shapes draws below 3D shapes when this is disabled (i know how DEPTH TEST works, but i dont know why this is happening here and not on the code pre-revamp)

	this->shader->use();
	shape.get_vao().bind();

	shape.draw(this->camera, *this->shader);

	shape.get_vao().unbind();
	this->shader->unbind();

#ifdef SCARAB_DEBUG_DRAWCALL
	this->drawcalls += 1;
#endif
}


void Scene3D::draw_all() {
	// glDepthFunc(GL_LESS);

	this->shader->use();

	for(const auto& [key, shape] : this->scene) {
		shape->get_vao().bind();

		shape->draw(this->camera, *this->shader);

		shape->get_vao().unbind();

	#ifdef SCARAB_DEBUG_DRAWCALL
		this->drawcalls += 1;
	#endif
	}

	this->shader->unbind();
}

