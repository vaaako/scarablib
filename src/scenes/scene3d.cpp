#include "scarablib/scenes/scene3d.hpp"

Scene3D::Scene3D(const Window& window, Camera& camera) : Scene(window), camera(camera) {}

Scene3D::~Scene3D() {
	delete this->shader;
}

void Scene3D::draw_mesh(Mesh& shape) {
	glEnable(GL_DEPTH_TEST); // I couldn't find out why Font gets a background when this is enabled and why 2D shapes draws below 3D shapes when this is disabled (i know how DEPTH TEST works, but i dont know why this is happening here and not on the code pre-revamp)

	this->shader->use();
	shape.get_vao().bind();

	shape.draw(this->camera, *this->shader);

	shape.get_vao().unbind();
	this->shader->unbind();
}


void Scene3D::draw_all() {
	glEnable(GL_DEPTH_TEST);

	this->shader->use();

	for(Mesh* shape : this->scene) {
		shape->draw(this->camera, *this->shader); // If circle it will use the shader in the circle struct
	}

	this->shader->unbind();
}

void Scene3D::draw_all(const std::vector<Mesh*>& shapes) {
	glEnable(GL_DEPTH_TEST);

	// Get shape VAO
	const VAO& vao = shapes.at(0)->get_vao();

	this->shader->use();
	vao.bind();

	for(auto shape : shapes) {
		shape->draw(this->camera, *this->shader); // If circle it will use the shader in the circle struct
	}

	vao.unbind();
	this->shader->unbind();
}
