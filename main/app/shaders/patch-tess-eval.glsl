#version 410 core

precision highp float;

layout(quads, fractional_odd_spacing, ccw) in;

uniform mat4 M;
uniform mat4 MI;
uniform mat4 V;
uniform mat4 P;

uniform float amplitude_factor;
uniform float frequency_factor;
uniform float phase_factor;
uniform int iterations;

out vec2 m_texcoords;
out vec3 m_position;
out vec3 m_normal;

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
    float tex_x = u / p_len;
    float tex_y = v / p_len;
    m_texcoords = vec2(tex_x, tex_y);

    // - - - - - - - - - - - - - - - - - - - - - - - - - //

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
    m_normal = vec3(nx, 1.0, nz);
    m_position = vec3(p.x, height, p.z);
}