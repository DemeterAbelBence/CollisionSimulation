#version 330 core

precision highp float;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light {
	vec3 position;
	vec3 powerDensity;
};

uniform sampler2D texture_ambient1;
uniform sampler2D texture_ambient2;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;
uniform sampler2D texture_normal1;

uniform Material material;
uniform Light light;
uniform int meshFrame;

uniform float w_amb;
uniform float w_dif;
uniform float w_spe;

in vec2 texcoords;
in vec3 normal_vec;
in mat3 TBN;
in vec3 position;
in vec3 light_dir;
in vec3 view_dir;

out vec4 frag_col;

void main() {
	if (meshFrame == 1) {
		frag_col = vec4(1.0, 1.0, 1.0, 0.1);
		return;
	}

	/*vec3 normal_vec = vec3(texture(texture_normal1, texcoords));
	normal_vec = normalize(normal_vec * 2.0 - 1.0);
	normal_vec = normalize(normal_vec * TBN); 
	normal_vec.y *= -1;*/

	vec3 half_dir = normalize(light_dir + view_dir);
	float light_cos = max(dot(normal_vec, light_dir), 0.0);
	float view_cos = max(dot(normal_vec, half_dir), 0.0);

	vec3 amb1 = vec3(texture(texture_ambient1, texcoords));
	vec3 amb2 = vec3(texture(texture_ambient2, texcoords));
	vec3 ka = w_amb * (amb1 + amb2);

	vec3 dif1 = vec3(texture(texture_diffuse1, texcoords));
	vec3 dif2 = vec3(texture(texture_diffuse2, texcoords));
	vec3 kd = w_dif * (dif1 + dif2);

	vec3 spe1 = vec3(texture(texture_specular1, texcoords));
	vec3 spe2 = vec3(texture(texture_specular2, texcoords));
	vec3 ks = w_spe * (spe1 + spe2);

	float sh = 10.0;
	float dist = length(position - light.position); 
	vec3 rad = light.powerDensity / (pow(dist, 1.5));
	vec3 final = ka + rad*kd*light_cos + rad*ks*pow(view_cos, sh);
	
	frag_col = vec4(final, 1.0);
}