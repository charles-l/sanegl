#version 330 core
layout(location = 0) in vec3 v_pos;
uniform mat4 MVP;
out vec3 pos;
void main() {
    gl_Position = (MVP * vec4(v_pos, 1)).xyzw;
    pos = v_pos;
}
