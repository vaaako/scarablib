#version 330 core

in vec2 texCoord;
out vec4 FragColor;

uniform sampler2D texSampler;
uniform vec4 shapeColor; // Color is defined by user later

// Multiplying by 0.004 gives an approximated result as dividing by 255
vec4 normalized_color(vec4 color) {
	return vec4(color.rgba * 0.004);
}

void main() {
	vec4 tex = texture(texSampler, texCoord);
	if(tex.a == 0.0) {
		discard;
	}

	FragColor =  tex * normalized_color(shapeColor.rgba);
}
