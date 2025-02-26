#version 410 core

precision highp float;

layout(quads, fractional_odd_spacing, ccw) in;

// uniform variables
uniform mat4 M;
uniform mat4 MI;
uniform mat4 V;
uniform mat4 P;

uniform float MIN_DIST;
uniform float MAX_DIST;

uniform vec3 eye;
uniform vec3 light;

uniform float amplitude_factor;
uniform float frequency_factor;
uniform float phase_factor;
uniform int iterations;

// out variables
out vec3 n_vec;
out vec3 l_vec;
out vec3 v_vec;
out vec2 tex_coord;
out float height;
out float bottom;

//terrain functions
float rand(float x, float z) {
    return abs(cos(length(vec2(x, z))));
}

float calculate_amplitude(float f1, float f2, float A0) {
    float A;
    float s = sqrt(f1 * f1 + f2 * f2);

    if (s > 0) A = A0 / s;
    else A = A0;
    return amplitude_factor * A;
}

float calculate_phase(float x, float z, float r) {
    return phase_factor * r;
}

void main() {
    // vertex patch position and texture coordinate calculation
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    vec4 p00 = gl_in[0].gl_Position;
    vec4 p01 = gl_in[1].gl_Position;
    vec4 p10 = gl_in[2].gl_Position;
    vec4 p11 = gl_in[3].gl_Position;

    vec4 p0 = (p01 - p00) * u + p00;
    vec4 p1 = (p11 - p10) * u + p10;
    vec4 p = (p1 - p0) * v + p0;

    float p_len = abs(p01.x - p00.x);
    tex_coord.x = u / p_len;
    tex_coord.y = v / p_len;

    // vertex height position and normal calculation
    int itr = iterations;
    float fre = frequency_factor;

    float nx = 0;
    float nz = 0;
    float height = 0;

    for (int f1 = 0; f1 < itr; f1++) {
        for (int f2 = 0; f2 < itr; f2++) {
            float amp = calculate_amplitude(f1, f2, rand(f1, f2));
            float pha = calculate_phase(f1, f2, rand(f1, f2));
            float cos_param = fre * (f1 * p.x + f2 * p.z) + pha;
            height += amp * cos(cos_param);

            float sin_param = fre * (f1 * p.x + f2 * p.z) + pha;
            nx += amp * fre * f1 * sin(sin_param);
            nz += amp * fre * f2 * sin(sin_param);
        }
    }

    p.y += height;
    vec3 n = vec3(nx, 1.0, nz);

    // final calculations and out variables  
    vec3 normal = normalize(vec3(MI * vec4(n, 1)));
    vec4 world_position = p * M;
    gl_Position = world_position * V * P;

    n_vec = normal;
    l_vec = normalize(light.xyz - world_position.xyz);
    v_vec = normalize(eye - world_position.xyz);
}