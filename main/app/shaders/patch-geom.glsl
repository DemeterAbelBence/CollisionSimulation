#version 330 core

precision highp float;

struct Light {
	vec3 position;
	vec3 powerDensity;
};

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec2 m_texcoords[];
in vec3 m_position[];
in vec3 m_normal[];

out vec2 texcoords;
out vec3 position;
out vec3 light_dir;
out vec3 view_dir;
out mat3 TBN;

uniform mat4 M;
uniform mat4 MI;
uniform mat4 V;
uniform mat4 P;

uniform vec3 eye;
uniform Light light;

void main() {
	vec3 pos1 = m_position[0];
	vec3 pos2 = m_position[1];
	vec3 pos3 = m_position[2];

	vec2 uv1 = m_texcoords[0];
	vec2 uv2 = m_texcoords[1];
	vec2 uv3 = m_texcoords[2];

	vec3 edge1 = pos2 - pos1;
	vec3 edge2 = pos3 - pos1;
	vec2 deltaUV1 = uv2 - uv1;
	vec2 deltaUV2 = uv3 - uv1;

	float f = 1.0 / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	vec3 tangent, bitangent;
	tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

	bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
	bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
	bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

	for(int i = 0; i < 3; i++) {
		vec4 w_position = vec4(m_position[i], 1) * M;
		gl_Position = w_position * V * P;
		position = vec3(w_position);

		light_dir = normalize(light.position - position);
		view_dir = normalize(eye - position);

		vec3 T = normalize(vec3(MI * vec4(tangent, 0.0)));
		vec3 B = normalize(vec3(MI * vec4(bitangent, 0.0)));
		vec3 N = normalize(vec3(MI * vec4(m_normal[i], 0.0)));
		TBN = mat3(T, B, N);

		texcoords = m_texcoords[i];
		EmitVertex();
	}
	EndPrimitive();
}