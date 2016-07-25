#version 330
in vec3 pos;
out vec3 color;
uniform samplerCube skybox;
void main() {
    color = texture(skybox, pos).rgb;
}
