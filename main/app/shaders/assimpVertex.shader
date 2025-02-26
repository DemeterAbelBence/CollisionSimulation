#version 330 core

precision highp float;

layout(location = 0) in vec3 v_pos;
layout(location = 1) in vec3 v_nor;
layout(location = 2) in vec2 v_tex;

uniform mat4 M;
uniform mat4 MI;
uniform mat4 V;
uniform mat4 P;

uniform vec3 eye;
uniform vec3 light;

out vec2 tex;
out vec3 n_vec;
out vec3 l_vec;
out vec3 v_vec;

void main() {
	vec4 worldPosition = vec4(v_pos, 1) * M;
	vec4 worldNormal = MI * vec4(v_nor, 0);

	tex = v_tex;
	n_vec = normalize(vec3(worldNormal));
	l_vec = normalize(light - vec3(worldPosition));
	v_vec = normalize(eye - vec3(worldPosition));

	gl_Position = worldPosition * V * P;
}