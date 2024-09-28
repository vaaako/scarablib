#version 330 core

out vec4 FragColor;
in vec2 texCoord;

uniform sampler2D texSampler;

uniform vec4 shapeColor; // Color is defined by user later

void main() {
	FragColor = texture(texSampler, texCoord) * vec4(shapeColor.rgba);
}
