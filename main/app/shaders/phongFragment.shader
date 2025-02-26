#version 330 core

precision highp float;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

uniform sampler2D box_texture;
uniform Material material;
uniform int meshFrame;

in vec3 n_vec;
in vec3 l_vec;
in vec3 v_vec;
in vec2 tex_coord;

out vec4 frag_col;

void main() {

	if (meshFrame == 1) {
		frag_col = vec4(1, 1, 1, 0.1);
		return;
	}

	vec3 tex_col = vec3(texture(box_texture, tex_coord));
	vec3 radiance = tex_col + material.ambient;
	vec3 h_vec = normalize(l_vec + v_vec);
	float light_cos = max(dot(n_vec, l_vec), 0);
	float view_cos = max(dot(n_vec, h_vec), 0);
	radiance += (material.diffuse * light_cos +
		material.specular * pow(view_cos, material.shininess));

	frag_col = vec4(radiance, 1);
}