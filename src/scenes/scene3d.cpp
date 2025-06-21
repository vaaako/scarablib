#include "scarablib/scenes/scene3d.hpp"
#include <GL/glew.h>

Scene3D::Scene3D(Camera& camera) noexcept
	: IScene(camera) {}

