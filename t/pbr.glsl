#version 330

precision highp float; //set default precision in glsl es 2.0

in vec2 v_texcoord;
in vec3 v_normal;
in vec3 v_binormal;
in vec3 v_pos;

vec3 lightPos = vec3(4.0, 3.0, -3.0);
vec3 camPos = vec3(4.0, 3.0, -3.0);
out vec4 result;

float ggx(float NdotH, float rough) {
    float m = rough * rough;
    float m2 = m * m;
    float d = (NdotH * m2 - NdotH) * NdotH + 1; // 2 mad
    return m2 / (d*d);
}

float cookTorranceSpecular(
        vec3 lightDirection,
        vec3 viewDirection,
        vec3 surfaceNormal,
        float roughness,
        float fresnel) {
    float VdotN = max(dot(viewDirection, surfaceNormal), 0.0);
    float LdotN = max(dot(lightDirection, surfaceNormal), 0.0);

    //Half angle vector
    vec3 H = normalize(lightDirection + viewDirection);

    //Geometric term
    float NdotH = max(dot(surfaceNormal, H), 0.0);
    float VdotH = max(dot(viewDirection, H), 0.000001);
    float LdotH = max(dot(lightDirection, H), 0.000001);
    float G1 = (2.0 * NdotH * VdotN) / VdotH;
    float G2 = (2.0 * NdotH * LdotN) / LdotH;
    float G = min(1.0, min(G1, G2));

    //Distribution term
    float D = ggx(NdotH, roughness);

    //Fresnel term
    float F = pow(1.0 - VdotN, fresnel);

    //Multiply terms and done
    return  G * F * D / max(3.14159265 * VdotN * LdotN, 0.000001);
}

void main() {
    vec3 lightDir = normalize(lightPos - v_pos);
    vec3 eyeDir = normalize(camPos - v_pos);
    vec3 norm = normalize(-v_normal); // i flipped the normals accidentally
    float power = cookTorranceSpecular(lightDir, eyeDir, norm, 0.7, 0.0);
    result = vec4(power, power, power, 1.0);
}
