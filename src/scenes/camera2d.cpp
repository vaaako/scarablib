#include "scarablib/scenes/camera2d.hpp"

Camera2D::Camera2D(const Window& window)
	: width(static_cast<float>(window.get_width())), height(static_cast<float>(window.get_height())) {

	this->update_proj_matrix();
	this->update_view_matrix();
}
