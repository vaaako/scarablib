#version 330

// OpenGL 4.2+
// layout(binding = 0) uniform samplerCube skybox;

out vec4 FragColor;

in vec3 texCoord;

uniform samplerCube skybox;

void main() {
	FragColor = texture(skybox, texCoord);
}
