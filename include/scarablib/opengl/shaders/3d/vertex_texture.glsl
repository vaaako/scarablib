#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
// layout (location = 2) in float aSha; // Shader (shadow)

out vec2 texCoord;
uniform mat4 mvp;

void main() {
	gl_Position = mvp * vec4(aPos, 1.0);
	texCoord    = aTex;
	// shadingValue = aSha;
}
