#version 330 core

precision highp float;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texture_coordinate;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

uniform int meshFrame;

out vec2 tex_coord;

void main() {
	tex_coord = texture_coordinate;

	vec4 world_position = vec4(position, 1) * M;
	vec4 final_pos = world_position * V * P;
	gl_Position = final_pos;
}