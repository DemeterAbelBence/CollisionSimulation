#version 330 core

precision highp float;

layout(location = 0) in vec3 position;

uniform mat4 V;
uniform mat4 P;
uniform int overrideZ;

void main() {

	vec4 result = vec4(position, 1) * V * P;

	if(overrideZ == 1) {
		result.z = 0.000001;
	}

	gl_Position = result;
}