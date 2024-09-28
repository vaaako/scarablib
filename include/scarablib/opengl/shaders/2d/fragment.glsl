#version 330 core

out vec4 FragColor;
uniform vec4 shapeColor; // Color is defined by user later

void main() {
	FragColor = vec4(shapeColor.rgba);
}

