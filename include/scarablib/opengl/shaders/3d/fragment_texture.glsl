#version 330 core

in vec2 texCoord;
out vec4 FragColor;

uniform sampler2D texSampler;
uniform vec4 shapeColor; // Color is defined by user later

void main() {
	vec4 texColor = texture(texSampler, texCoord) * vec4(shapeColor.rgba);
	FragColor = texColor;

	// FragColor = texColor * shadingValue // Shadow
}
