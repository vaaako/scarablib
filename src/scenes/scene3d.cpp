#include "scarablib/scenes/scene3d.hpp"

Scene3D::Scene3D(const Window& window, Camera& camera) : Scene(window), camera(camera) {}

Scene3D::~Scene3D() {
	delete this->shader;
}

void Scene3D::draw_mesh(Shape3D& shape) {
	glEnable(GL_DEPTH_TEST);

	shape.get_vao().bind();
	this->shader->use();

	shape.draw(this->camera, *this->shader);

	shape.get_vao().unbind();
}


void Scene3D::draw_all(const DrawMode drawmode) {
	glPolygonMode(GL_FRONT_AND_BACK, drawmode);
	glEnable(GL_DEPTH_TEST); // I couldn't find out why Font gets a background when this is enabled and why 2D shapes draws below 3D shapes when this is disabled (i know how DEPTH TEST works, but i dont know why this is happening here and not on the code pre-revamp)

	this->shader->use();

	for(auto& shape : this->scene) {
		shape->draw(this->camera, *this->shader); // If circle it will use the shader in the circle struct
	}
}

void Scene3D::draw_all(const std::vector<Shape3D*>& shapes, const DrawMode drawmode) {
	glPolygonMode(GL_FRONT_AND_BACK, drawmode);
	glEnable(GL_DEPTH_TEST); // I couldn't find out why Font gets a background when this is enabled and why 2D shapes draws below 3D shapes when this is disabled (i know how DEPTH TEST works, but i dont know why this is happening here and not on the code pre-revamp)

	// Get shape VAO
	VAO& vao = shapes.at(0)->get_vao();

	this->shader->use();
	vao.bind();

	for(auto shape : shapes) {
		shape->draw(this->camera, *this->shader); // If circle it will use the shader in the circle struct
	}

	vao.unbind();
}