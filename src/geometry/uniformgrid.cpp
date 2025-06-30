#include "scarablib/geometry/uniformgrid.hpp"
#include "scarablib/geometry/model.hpp"
#include "scarablib/proper/log.hpp"

UniformGrid::UniformGrid(const char* path, const vec3<uint32>& griddims, const glm::mat4& transform) noexcept
	: griddims(griddims) {

	const std::vector<MeshTriangle> terrainmesh = Model::get_obj_triangles(path, transform);

	// Calculate the World Bounds to correctly size the Grid
	// loop through all loaded vertices to find the min and max corners
	this->worldmin = vec3<float>(std::numeric_limits<float>::max());
	vec3<float>worldmax = vec3<float>(std::numeric_limits<float>::lowest());

	for(const MeshTriangle& tri : terrainmesh) {
		this->worldmin = glm::min(this->worldmin, tri.v0);
		this->worldmin = glm::min(this->worldmin, tri.v1);
		this->worldmin = glm::min(this->worldmin, tri.v2);

		worldmax = glm::max(worldmax, tri.v0);
		worldmax = glm::max(worldmax, tri.v1);
		worldmax = glm::max(worldmax, tri.v2);
	}

	// Add a little padding
	this->worldmin -= glm::vec3(1.0f);
	worldmax += glm::vec3(1.0f);

	// Initialize the Grid
	this->griddims = griddims;
	this->worldsize = worldmax - worldmin;
	this->cellsize = this->worldsize / (vec3<float>)this->griddims;
	this->cells.resize((size_t)(this->griddims.x * this->griddims.y * this->griddims.z));

	// Populate the Grid
	for (const MeshTriangle& tri : terrainmesh) {
		this->add_triangle(tri);
	}
}


UniformGrid::UniformGrid(const vec3<float>& worldmin, const vec3<float>& worldmax, const vec3<uint32>& griddims) noexcept
	: worldmin(worldmin), griddims(griddims) {

	this->worldsize = worldmax - worldmin;
	this->cellsize = this->worldsize / (vec3<float>)this->griddims;
	this->cells.resize((size_t)(this->griddims.x * this->griddims.y * this->griddims.z));
}

void UniformGrid::add_triangle(const MeshTriangle& tri) noexcept {
	// Find the min/max grid cells the triangle's bounding box overlaps
	const vec3<float> tri_min = glm::min(glm::min(tri.v0, tri.v1), tri.v2);
	const vec3<float> tri_max = glm::max(glm::max(tri.v0, tri.v1), tri.v2);

	const vec3<uint32> min_cell = get_cell_index(tri_min);
	const vec3<uint32> max_cell = get_cell_index(tri_max);

	// Add the triangle to all overlapped cells
	for(uint32 z = min_cell.z; z <= max_cell.z; ++z) {
		for(uint32 y = min_cell.y; y <= max_cell.y; ++y) {
			for(uint32 x = min_cell.x; x <= max_cell.x; ++x) {
				const size_t linearindex = (size_t)(z * this->griddims.x * this->griddims.y + y * this->griddims.x + x);
				this->cells[linearindex].push_back(tri);
			}
		}
	}
}


vec3<uint32> UniformGrid::get_cell_index(const vec3<float>& pos) const noexcept {
	const vec3<float> local_pos = pos - this->worldmin;
	const vec3<uint32> index = glm::floor(local_pos / this->cellsize);
	// Clamp to grid dimensions to handle points exactly on the max boundary
	return glm::clamp(index, vec3<uint32>(0), this->griddims - (uint32)1);
}



Raycast::Rayhit UniformGrid::raycast(const Raycast::Ray& ray) const noexcept {
	Raycast::Rayhit closest_hit;

	// Voxel Traversal Algorithm (Amanatides & Woo)
	// This algorithm efficiently finds every cell a ray passes through

	vec3<uint32> currentcell = get_cell_index(ray.origin);
	LOG_DEBUG("Current cell: (%i, %i, %i)", currentcell.x, currentcell.y, currentcell.z);

	// Handle case where ray starts outside the grid
	if (currentcell.x < 0 || currentcell.x >= this->griddims.x ||
		currentcell.y < 0 || currentcell.y >= this->griddims.y ||
		currentcell.z < 0 || currentcell.z >= this->griddims.z) {
		// A more robust implementation would find the entry point.
		// For this example, we'll just assume rays start inside or don't hit.
		return closest_hit;
	}

	const vec3<int> step = vec3<int>(
		(ray.direction.x > 0) ? 1 : -1,
		(ray.direction.y > 0) ? 1 : -1,
		(ray.direction.z > 0) ? 1 : -1
	);

	const vec3<int> next_boundary = vec3<int>(
		((float)currentcell.x + (step.x > 0 ? 1 : 0)) * this->cellsize.x,
		((float)currentcell.y + (step.y > 0 ? 1 : 0)) * this->cellsize.y,
		((float)currentcell.z + (step.z > 0 ? 1 : 0)) * this->cellsize.z
	);

	vec3<float> tmax   = ((vec3<float>)next_boundary - ray.origin) / ray.direction;
	vec3<float> tdelta = this->cellsize / glm::abs(ray.direction);

	// Transverse the grid
	// Safety break
	for(int i = 0; i < 100; i++) {
		// Test triangles in the current cell
		uint32 linearindex = currentcell.z * this->griddims.x * this->griddims.y + currentcell.y * this->griddims.x + currentcell.x;
		for(const auto& tri : this->cells[(size_t)linearindex]) {
			float dist;
			if(Raycast::intersects_triangle(ray, tri, dist)) {
				if(dist < closest_hit.distance) {
					closest_hit.hit = true;
					closest_hit.distance = dist;
					closest_hit.point = ray.origin + ray.direction * dist;
					closest_hit.normal = tri.normal;
				}
			}
		}

		// If have a hit, dont need to check cells further away
		if(closest_hit.hit && (tmax.x > closest_hit.distance && tmax.y > closest_hit.distance && tmax.z > closest_hit.distance)) {
			break;
		}

		// Move to the next cell
		if(tmax.x < tmax.y) {
			if(tmax.x < tmax.z) {
				currentcell.x += step.x;
				tmax.x += tdelta.x;
			} else {
				currentcell.z += step.z;
				tmax.z += tdelta.z;
			}
		} else {
			if(tmax.y < tmax.z) {
				currentcell.y += step.y;
				tmax.y += tdelta.y;
			} else {
				currentcell.z += step.z;
				tmax.z += tdelta.z;
			}
		}

		// Check if exited the grid
		if (currentcell.x < 0 || currentcell.x >= this->griddims.x ||
			currentcell.y < 0 || currentcell.y >= this->griddims.y ||
			currentcell.z < 0 || currentcell.z >= this->griddims.z) {
			break;
		}
	}

	return closest_hit;
}
