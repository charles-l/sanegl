#version 330 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 binormal;
layout(location = 3) in vec2 uv;

uniform mat4 MVP;

out vec2 v_texcoord;
out vec3 v_normal;
out vec3 v_binormal;
out vec3 v_pos;

void main() {
    gl_Position = MVP * vec4(pos, 1);

    v_pos = gl_Position.xyz;
    v_normal = normalize(normal);
    v_binormal = normalize(binormal);
    v_texcoord = uv;
}
