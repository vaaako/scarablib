#version 330 core

out vec4 FragColor;
in vec2 texCoord;

uniform sampler2D texSampler;
// uniform sampler2D mask; // Texture to go above

uniform vec4 shapeColor; // Color defined by user later
uniform bool useTexture; // To toggle texture usage

void main() {
	// vec3 baseColor = texture(texSampler, texCoord).rgb;
	// float alpha = texture(mask, texCoord).r;
	// FragColor = vec4(baseColor, alpha);
	// texture->bind(0) // default, texture
	// texture->bind(1) // mask, draw above

	if (useTexture) {
		FragColor = texture(texSampler, texCoord) * vec4(shapeColor.rgba); // Use texture
	} else {
		FragColor = vec4(shapeColor.rgba); // Use solid color
	}
}
