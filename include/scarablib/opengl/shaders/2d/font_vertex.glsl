#version 330 core

layout (location = 0) in vec4 aPos; // Position and Texture

out vec2 texCoord;

uniform mat4 projection;

void main() {
	gl_Position = projection * vec4(aPos.xy, 0.0, 1.0);
	texCoord    = aPos.zw;
}
