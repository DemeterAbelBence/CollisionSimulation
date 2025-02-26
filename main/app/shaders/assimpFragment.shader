#version 330 core

precision highp float;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

uniform sampler2D texture_ambient;
uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;
uniform sampler2D normal_map;

uniform Material material;
uniform int meshFrame;

in vec3 n_vec;
in vec3 l_vec;
in vec3 v_vec;
in vec2 tex;

out vec4 frag_col;

void main() {
	if (meshFrame == 1) {
		frag_col = vec4(1.0, 1.0, 1.0, 0.1);
		return;
	}

	vec3 t_amb = vec3(texture(texture_ambient, tex));
	vec3 t_dif = vec3(texture(texture_diffuse, tex));
	vec3 t_spe = vec3(texture(texture_specular, tex));
	vec3 t_col = 0.3*t_amb + 0.3*t_dif + 0.3*t_spe;

	vec3 h_vec = normalize(l_vec + v_vec);
	float light_cos = max(dot(n_vec, l_vec), 0.0);
	float view_cos = max(dot(n_vec, h_vec), 0.0);

	vec3 ka = material.ambient;
	vec3 kd = material.diffuse;
	vec3 ks = material.specular;
	float sh = material.shininess;

	vec3 lighting = kd * light_cos + ks * pow(view_cos, sh);
	vec3 final = 0.5*t_col + 1.5*lighting;
	frag_col = vec4(final, 1.0);
}