#version 330 core

precision highp float;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texture_coordinate;

uniform mat4 M;
uniform mat4 MI;
uniform mat4 V;
uniform mat4 P;

uniform vec3 eye;
uniform vec3 light;

out vec2 tex_coord;
out vec3 n_vec;
out vec3 l_vec;
out vec3 v_vec;

void main() {
	tex_coord = texture_coordinate;
	n_vec = normalize(vec3(MI * vec4(normal, 0)));
	l_vec = normalize(light - position);
	v_vec = normalize(eye - position);

	vec4 world_position = vec4(position, 1) * M;
	vec4 final_pos = world_position * V * P;
	gl_Position = final_pos;
}