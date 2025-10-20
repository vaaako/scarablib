#include "scarablib/gfx/3d/plane.hpp"
#include "scarablib/geometry/geometry_factory.hpp"
#include "scarablib/typedef.hpp"

Plane::Plane(const Plane::Type type) noexcept : Model(), type(type) {
	switch (type) {
		case Plane::Type::SINGLE_PLANE:
			this->set_geometry(
				GeometryFactory::make_plane_vertices(),
				std::vector<uint8> {
					0, 1, 2, 0, 2, 3
				}
			);
			break;
		case Plane::Type::CROSSED_PLANE:
			this->set_geometry(
				GeometryFactory::make_crossedplane_vertices(),
				std::vector<uint8> {
					0, 1, 2, 2, 3, 0, // First quad
					4, 5, 6, 6, 7, 4  // Second quad
				}
			);
			break;
		case Plane::Type::FOUR_CROSSED_PLANE:
			this->set_geometry(
				GeometryFactory::make_fourcrossedplane_vertices(),
				std::vector<uint8> {
					 0,  1,  2,  0,  2,  3, // Quad 1
					 4,  5,  6,  4,  6,  7, // Quad 2
					 8,  9, 10,  8, 10, 11, // Quad 3
					12, 13, 14, 12, 14, 15  // Quad 4
				}
			);
			break;

		default:
			break;
	}
}



void Plane::draw_logic(const Camera& camera) noexcept {
	// Enable cullface if fourcrossed else disable it
	bool enabled     = glIsEnabled(GL_CULL_FACE);
	bool fourcrossed = this->type == Plane::Type::FOUR_CROSSED_PLANE;
	if(fourcrossed) {
		glEnable(GL_CULL_FACE);
	} else {
		glDisable(GL_CULL_FACE);
	}

	this->update_model_matrix();
	this->material->shader->set_matrix4f("mvp", (camera.get_proj_matrix() * camera.get_view_matrix()) * this->model);
	glDrawElements(GL_TRIANGLES, this->vertexarray->get_length(), this->vertexarray->get_indices_type(), (void*)0);

	// Cullface back to original state
	if(fourcrossed && !enabled) {
		glDisable(GL_CULL_FACE);
	} else if(enabled) {
		glEnable(GL_CULL_FACE);
	}
}
