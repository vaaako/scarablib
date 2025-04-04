#version 330 core

layout (location = 0) in vec3 aPos;

out vec3 texCoord;

uniform mat4 proj;
uniform mat4 view;

void main() {
	texCoord = aPos;
	vec4 pos = proj * view * vec4(aPos, 1.0);
	// Draw above
	gl_Position = pos.xyww;
}
