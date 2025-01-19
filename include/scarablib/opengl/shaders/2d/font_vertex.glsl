#version 330 core

layout (location = 0) in vec4 aPos; // <vec4 pos> (x, y, s, t)

out vec2 texCoord;

uniform mat4 model;
uniform mat4 projection;

void main() {
	gl_Position = projection * model * vec4(aPos.xy, 0.0, 1.0);
	texCoord    = aPos.xy;
}

