#version 330
precision highp float; //set default precision in glsl es 2.0

#define PI  3.141592653
#define MAXBIAS 10

in vec2 v_texcoord;
in vec3 v_normal;
in vec3 v_pos;

uniform samplerCube env;
uniform sampler2D shadow_map;
uniform vec3 cam_pos;

vec3 light_pos = vec3(10.0, 3.0, -3.0);
vec3 light_col = vec3(1.0, 1.0, 1.0);
vec3 base = vec3(1.0, 1.0, 1.0);

#define METALLIC 0.5

out vec4 result;

float schlick(in float ndv) {
    float r = METALLIC + (1 - METALLIC) * pow(1 - ndv, 5);
    if (r > 0.9) {
        return 1.0;
    } else if(r > 0.6) {
        return 0.7;
    } else {
        return r;
    }
}

void main() {
    vec3 light_dir = normalize(light_pos - v_pos);
    vec3 view_dir = normalize(cam_pos - v_pos);
    vec3 norm = normalize(v_normal);

    float ndv = dot(norm, view_dir);

    result = vec4(vec3(schlick(ndv)), 1.0);
}
