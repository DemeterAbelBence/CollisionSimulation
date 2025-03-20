#version 330 core

precision highp float;

layout(location = 0) in vec3 b_position;
layout(location = 1) in vec3 b_normal;
layout(location = 2) in vec3 b_tangent;
layout(location = 3) in vec3 b_bitangent;
layout(location = 4) in vec2 b_texcoords;

struct Light {
	vec3 position;
	vec3 powerDensity;
};

uniform Light light;
uniform vec3 camera;

uniform mat4 M;
uniform mat4 MI;
uniform mat4 V;
uniform mat4 P;

out vec2 texcoords;
out vec3 normal_vec;
out mat3 TBN;
out vec3 position;
out vec3 light_dir;
out vec3 view_dir;

void main() {
	vec4 w_position = vec4(b_position, 1) * M;
	gl_Position = w_position * V * P;
	position = vec3(w_position);

	light_dir = normalize(light.position - position);
	view_dir = normalize(camera - position);
	normal_vec = normalize(vec3(MI * vec4(b_normal, 0.0)));
	
	vec3 T = normalize(vec3(MI * vec4(b_tangent, 0.0)));
	vec3 B = normalize(vec3(MI * vec4(b_bitangent, 0.0)));
	vec3 N = normalize(vec3(MI * vec4(b_normal, 0.0)));
	TBN = mat3(T, B, N);

	texcoords = b_texcoords;
}