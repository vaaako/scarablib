#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;

out vec2 texCoord;

uniform mat4 view;
uniform mat4 proj;
uniform vec3 billboardPos;
uniform float billboardSize;

void main() {
	// Extract the right and up vectors from the view matrix
	vec3 right = normalize(vec3(view[0].x, view[1].x, view[2].x));
	vec3 up = normalize(vec3(view[0].y, view[1].y, view[2].y));

	// Reconstruct the billboarded position
	vec3 billboardedPos = billboardPos + (aPos.x * right * billboardSize + aPos.y * up * billboardSize);

	gl_Position = proj * view * vec4(billboardedPos, 1.0);
	texCoord    = aTex;
}
