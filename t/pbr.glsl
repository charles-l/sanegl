#version 330
precision highp float; //set default precision in glsl es 2.0

#define PI 3.141592653

in vec2 v_texcoord;
in vec3 v_normal;
in vec3 v_binormal;
in vec3 v_pos;

vec3 light_pos = vec3(4.0, 3.0, -3.0);
vec3 cam_pos = vec3(4.0, 3.0, -3.0);
vec3 light_col = vec3(0.3, 1.0, 1.0);
vec3 base = vec3(1.0, 1.0, 1.0);
out vec4 result;

float metallic = 0.4;

float ggx(float NdotH, float rough) {
    float m = rough * rough;
    float m2 = m * m;
    float d = (NdotH * m2 - NdotH) * NdotH + 1; // 2 mad
    return m2 / (d*d);
}

float lambertDiffuse(vec3 light_direction, vec3 surface_norm) {
    return max(0.0, dot(light_direction, surface_norm));
}

// returns cook-torrance power (needs to be multiplied with color/skybox)
float cookTorranceSpecular(
        vec3 light_direction,
        vec3 view_dir,
        vec3 surface_norm,
        float roughness,
        float fresnel) {
    float VdotN = max(dot(view_dir, surface_norm), 0.0);
    float LdotN = max(dot(light_direction, surface_norm), 0.0);

    // Half angle vector
    vec3 H = normalize(light_direction + view_dir);

    // Geometric term
    float NdotH = max(dot(surface_norm, H), 0.0);
    float VdotH = max(dot(view_dir, H), 0.000001);
    float LdotH = max(dot(light_direction, H), 0.000001);
    float G1 = (2.0 * NdotH * VdotN) / VdotH;
    float G2 = (2.0 * NdotH * LdotN) / LdotH;
    float G = min(1.0, min(G1, G2));

    // Distribution term
    float D = ggx(NdotH, roughness);

    // Fresnel term
    float F = pow(1.0 - VdotN, fresnel);

    // Multiply terms and done
    return G * F * D / max(PI * VdotN * LdotN, 0.000001);
}

void main() {
    vec3 light_dir = normalize(light_pos - v_pos);
    vec3 eye_dir = normalize(cam_pos - v_pos);
    vec3 norm = normalize(v_normal);
    float spec_power = cookTorranceSpecular(light_dir, eye_dir, norm, 0.7, 0.0);
    float diffuse_power = lambertDiffuse(light_dir, norm);

    vec3 ref_light = vec3(0); // reflected light
    vec3 dif_light = vec3(0);

    ref_light += spec_power * light_col;
    dif_light += diffuse_power * light_col;

    // TODO: add ibl

    result = vec4(dif_light * mix(base, vec3(0.0), metallic) + ref_light, 1.0);
}
