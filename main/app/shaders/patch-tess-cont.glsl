#version 410 core

precision highp float;

layout(vertices = 4) out;

uniform float MIN_DIST;
uniform float MAX_DIST;
uniform int MIN_TESS;
uniform int MAX_TESS;

uniform mat4 M;
uniform mat4 V;

void main() {
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

    if (gl_InvocationID == 0)
    {
        vec4 pos00 = gl_in[0].gl_Position * M * V;
        vec4 pos01 = gl_in[1].gl_Position * M * V;
        vec4 pos10 = gl_in[2].gl_Position * M * V;
        vec4 pos11 = gl_in[3].gl_Position * M * V;

        float dist00 = clamp((abs(pos00.z) - MIN_DIST) / (MAX_DIST - MIN_DIST), 0.0, 1.0);
        float dist01 = clamp((abs(pos01.z) - MIN_DIST) / (MAX_DIST - MIN_DIST), 0.0, 1.0);
        float dist10 = clamp((abs(pos10.z) - MIN_DIST) / (MAX_DIST - MIN_DIST), 0.0, 1.0);
        float dist11 = clamp((abs(pos11.z) - MIN_DIST) / (MAX_DIST - MIN_DIST), 0.0, 1.0);

        float tess00 = mix(MAX_TESS, MIN_TESS, min(dist10, dist00));
        float tess01 = mix(MAX_TESS, MIN_TESS, min(dist00, dist01));
        float tess10 = mix(MAX_TESS, MIN_TESS, min(dist01, dist11));
        float tess11 = mix(MAX_TESS, MIN_TESS, min(dist11, dist10));

        gl_TessLevelOuter[0] = tess00;
        gl_TessLevelOuter[1] = tess01;
        gl_TessLevelOuter[2] = tess10;
        gl_TessLevelOuter[3] = tess11;

        gl_TessLevelInner[0] = max(tess01, tess11);
        gl_TessLevelInner[1] = max(tess00, tess10);
    }
}