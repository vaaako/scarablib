#include "scarablib/scenes/scene.hpp"

Scene::Scene(const Window& window)
	: width(window.get_width()), height(window.get_height()) {}

