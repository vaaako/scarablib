#include "scarablib/scenes/scene3d.hpp"

Scene3D::Scene3D(const Window& window, Camera& camera) : Scene(window), camera(camera) {}

Scene3D::~Scene3D() {
	delete this->shader;
}

void Scene3D::draw_mesh(Shape3D& shape) {
	glEnable(GL_DEPTH_TEST);
	shape.draw(this->camera, *this->shader);
}
