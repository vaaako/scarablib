#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex; // Always pass texture coordinates

out vec2 texCoord;

uniform mat4 model;
uniform mat4 projection;

void main() {
	gl_Position = projection * model * vec4(aPos.xy, 0.0f, 1.0);
	texCoord    = aTex; // Pass texture coordinates to fragment shader

	// texCoord = 0.5 * (aPos.xy + vec(1.0)); // Texture coords
	// texCoord = veec2(0.5, -0.5) * (aPos.xy + vec(1.0)); // Flip texture
	// Decrease 0.5 and -0.5 to zoom
}
