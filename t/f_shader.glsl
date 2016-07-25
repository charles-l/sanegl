#version 330
in vec2 UV;
in vec3 norm;
out vec4 color;
uniform sampler2D tex;
void main() {
    color = vec4(norm, 1.0);
}
