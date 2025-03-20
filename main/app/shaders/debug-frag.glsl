#version 330 core

precision highp float;

uniform vec3 color;

out vec4 fragmentColor;

void main() {
	fragmentColor = vec4(color, 1);
}