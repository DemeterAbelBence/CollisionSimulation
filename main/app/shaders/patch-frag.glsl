#version 330 core

precision highp float;

out vec4 frag_col;

in vec2 texcoords;
in vec3 position;
in vec3 light_dir;
in vec3 view_dir;
in mat3 TBN;

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

uniform Material material;
uniform Light light;
uniform sampler2D terr_ambient;
uniform sampler2D terr_normal;

void main() {	
    vec3 normal = vec3(texture(terr_normal, texcoords));
	normal = normalize(normal * 2.0 - 1.0);
	normal = normalize(normal * TBN); 

	vec3 half_dir = normalize(light_dir + view_dir);
    float light_cos = max(dot(normal, light_dir), 0.0);
	float view_cos = max(dot(normal, half_dir), 0.0);

	vec3 ka = 0.3 * vec3(texture(terr_ambient, texcoords));
	vec3 kd = material.diffuse;
	vec3 ks = material.specular;
	float sh = material.shininess;

	float dist = length(position - light.position); 
	vec3 radiance = light.powerDensity / (pow(dist, 1.5));
	vec3 final = ka + radiance * kd * light_cos + ks * pow(view_cos, sh);

	frag_col = vec4(final, 1.0);
}