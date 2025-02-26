#version 330 core

precision highp float;

layout(location = 0) in vec2 vertexPosition;

uniform vec3 rectColor;
out vec3 color;

void main() {
	color = rectColor;
	vec4 finalPosition = vec4(vertexPosition, 0, 1);
	gl_Position = finalPosition;
}