#version 330 core
layout(location = 0) in vec3 v_pos;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec3 v_binormal;
layout(location = 3) in vec2 v_uv;
uniform mat4 MVP;
out vec2 UV;
out vec3 norm;
void main() {
    gl_Position = MVP * vec4(v_pos, 1);
    norm = normalize(v_normal);
    UV = v_uv;
}
