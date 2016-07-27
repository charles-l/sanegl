#version 330
// taken from wikipedia

in vec2 v_texcoord;
in vec3 v_normal;
in vec3 v_binormal;
in vec3 v_pos;

out vec4 result;

const vec3 lightPos = vec3(1.0,1.0,1.0);
const vec3 ambientColor = vec3(0.1, 0.0, 0.0);
const vec3 diffuseColor = vec3(0.5, 0.0, 0.0);
const vec3 specColor = vec3(1.0, 1.0, 1.0);
const float shininess = 16.0;
const float screenGamma = 2.2; // Assume the monitor is calibrated to the sRGB color space

float lambert(vec3 lightDir, vec3 normal) {
    return max(0.0, dot(lightDir, normal));
}

float ggx(float rough, float noh) {
    float m = rough * rough;
    float m2 = m * m;
    float d = (noh * m2 - noh) * noh + 1;
    return m2 / (d*d);
}

void main() {
    vec3 normal = normalize(v_normal);
    vec3 lightDir = normalize(lightPos - v_pos);

    float lambertian = max(dot(lightDir,normal), 0.0);
    float specular = 0.0;

    if(lambertian > 0.0) {

        vec3 V = normalize(-v_pos);
        vec3 L = normalize(lightPos - v_pos);
        vec3 H = normalize(L + V);

        float specAngle = max(dot(H, normal), 0.0);
        specular = ggx(shininess, specAngle);
    }
    vec3 colorLinear = ambientColor +
        lambertian * diffuseColor +
        specular * specColor;
    // apply gamma correction (assume ambientColor, diffuseColor and specColor
    // have been linearized, i.e. have no gamma correction in them)
    vec3 colorGammaCorrected = pow(colorLinear, vec3(1.0/screenGamma));
    // use the gamma corrected color in the fragment
    result = vec4(colorGammaCorrected, 1.0);
}
