#version 330

// OpenGL 4.2+
// layout(binding = 0) uniform samplerCube skybox;

out vec4 FragColor;

in vec3 texCoord;

uniform samplerCube samplerSkybox;

void main() {
	FragColor = texture(samplerSkybox, texCoord);
}
