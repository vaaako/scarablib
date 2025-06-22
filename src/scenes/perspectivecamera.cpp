#include "scarablib/scenes/perspectivecamera.hpp"
#include "scarablib/proper/error.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/geometric.hpp>

PerspectiveCamera::PerspectiveCamera(const Window& window, const float fov, const float sensitivity) noexcept
	: Camera(window), sensitivity(sensitivity), fov(fov), max_fov(fov) {

	this->update_proj_matrix();
}


void PerspectiveCamera::rotate(const Window& mouse) noexcept {
	const vec2<int> move = mouse.relative_move();

	this->yaw   += static_cast<float>(move.x) * this->sensitivity;
	this->pitch += static_cast<float>(-move.y) * this->sensitivity;
	this->pitch  = glm::clamp(this->pitch, -89.0f, 89.0f); // Prevent gimbal lock

	this->update_camera_vectors();
}

void PerspectiveCamera::set_zoom(const float speed) noexcept {
	this->fov = (speed > 0.0f)
		? std::max(fov - speed, min_fov)
		: std::min(fov + speed, max_fov);
	this->update_proj_matrix();
}

// Sets camera near plane. How near to render
void PerspectiveCamera::set_near_plane(const float near_plane) {
	if(near_plane > this->far_plane) {
		throw ScarabError("new near plane value can't be higher than far plane current value");
	}

	this->near_plane = near_plane;
	this->update_proj_matrix();
}

// Sets camera far plane. How far to render
void PerspectiveCamera::set_far_plane(const float far_plane) {
	if(far_plane < this->near_plane) {
		throw ScarabError("new far plane value can't be lower than near plane current value");
	}

	this->far_plane = far_plane;
	this->update_proj_matrix();
}

// Sets cam::era fov
void PerspectiveCamera::set_fov(const float fov) {
	if(fov < this->near_plane || fov > this->far_plane) {
		throw ScarabError("new fov value must be between near plane and far plane current values");
	}
	this->fov = fov;
	this->update_proj_matrix();
}

// Sets minimum amount of fov. Used for zoom out
void PerspectiveCamera::set_min_fov(const float min_fov) {
	if(min_fov < this->max_fov) {
		throw ScarabError("new min fov value can't be higher than max fov current value");
	}
	this->min_fov = min_fov;
}

// Sets maximum amount of fov. Used for zoom in
void PerspectiveCamera::set_max_fov(const float max_fov) {
	if(max_fov < this->min_fov) {
		throw ScarabError("new max fov value can't be lower than min fov current value");
	}
	this->max_fov = max_fov;
}

void PerspectiveCamera::set_rotation(const float yaw, const float pitch) noexcept {
	// Wrap yaw to stay within 360 range
	this->yaw = std::fmod(yaw, 360.0f);
	// Prevent gimbal lock
	this->pitch = glm::clamp(pitch, -89.0f, 89.0f);
	this->update_camera_vectors();
}

void PerspectiveCamera::update_camera_vectors() noexcept {
	const float radpitch = glm::radians(this->pitch);
	const float cospitch = std::cos(radpitch);
	const float radyaw = glm::radians(this->yaw);

	this->forward = glm::normalize(vec3<float> {
		std::cos(radyaw) * cospitch,
		std::sin(radpitch),
		std::sin(radyaw) * cospitch
	});

	// Use fixed up position
	this->right = glm::normalize(glm::cross(this->forward, { 0.0f, 1.0f, 0.0f }));
	this->up   = glm::normalize(glm::cross(this->right, this->forward));
}


void PerspectiveCamera::move(const vec3<float>& dir, const float dt) noexcept {
	// Combine movement direction
	vec3<float> move = this->forward * dir.z +
					   this->up      * dir.y +
					   this->right   * dir.x;

	// Normalize only horizontal (XZ) to prevent faster diagonal movement
	if(dir.x != 0.0f || dir.z != 0.0f) {
		move = glm::normalize(move);
	}

	// Update position
	this->position += move * this->speed * dt;
}


std::string_view PerspectiveCamera::get_cardinal_direction(const float yaw) {
	// Define the cardinal directions
	constexpr std::array<std::string_view, 8> directions = {
		"North (-Z)", "Northeast", "East (+X)", "Southeast",
		"South (+Z)", "Southwest", "West (-X)", "Northwest"
	};

	// Determine the sector index. Normalizing yaw to [0, 360]
	return directions[static_cast<size_t>((std::fmod((yaw + 360.0f), 360.0f) + 22.5f) / 45.0f) % 8];
}


void PerspectiveCamera::update_proj_matrix() noexcept {
	this->proj = glm::mat4(1.0f);
	this->proj = glm::perspective(glm::radians(this->fov),
		(static_cast<float>(this->width) / static_cast<float>(this->height)),
		this->near_plane, this->far_plane);
}
