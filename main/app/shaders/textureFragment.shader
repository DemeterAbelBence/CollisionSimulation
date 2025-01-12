#version 330 core

precision highp float;

uniform sampler2D sun_texture;
uniform int meshFrame;

in vec2 tex_coord;
out vec4 frag_col;

void main() {
	if (meshFrame == 1) {
		frag_col = vec4(1, 1, 1, 0.4);
		return;
	}
	vec3 tex_col = vec3(texture(sun_texture, tex_coord));
	frag_col = vec4(tex_col, 1);
}