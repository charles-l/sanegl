// references
// http://blog.selfshadow.com/publications/s2013-shading-course/karis/s2013_pbs_epic_slides.pdf
// http://blog.selfshadow.com/publications/s2013-shading-course/karis/s2013_pbs_epic_notes_v2.pdf
// http://blog.selfshadow.com/publications/s2013-shading-course/lazarov/s2013_pbs_black_ops_2_notes.pdf
// http://blog.wolfire.com/2015/10/Physically-based-rendering

// took most code right out of: http://www.trentreed.net/blog/physically-based-shading-and-image-based-lighting/

#version 330
precision highp float; //set default precision in glsl es 2.0

#define PI  3.141592653
#define PI2 6.283185307
#define MAXBIAS 10
#define XAxis vec3(1.0, 0.0, 0.0)
#define YAxis vec3(0.0, 1.0, 0.0)
#define ZAxis vec3(0.0, 0.0, 1.0)

in vec2 v_texcoord;
in vec3 v_normal;
in vec3 v_pos;

uniform samplerCube env;
uniform vec3 cam_pos;

vec3 light_pos = vec3(50.0, 3.0, -3.0);
vec3 light_col = vec3(1.0, 1.0, 1.0);
vec3 base = vec3(1.0, 1.0, 1.0);

out vec4 result;

float i_metallic = 0.4;
float roughness = 0.1;

float pow2(float n) {
    return pow(n, 2);
}

vec3 diffuse_brdf(vec3 base) {
    return base / PI;
}

// GGX
float D(float ndh) {
    float a = pow2(roughness);
    float a2 = pow2(a);
    return a2 / (PI * pow2(pow2(ndh) * (a2 - 1) + 1));
}

vec3 MakeSample(vec2 E) {
    float SineTheta = sin(E.x);

    float x = cos(E.y) * SineTheta;
    float y = sin(E.y) * SineTheta;
    float z = cos(E.x);

    return vec3(x, y, z);
}

// GGX
vec3 S(vec2 E) {
    float a = pow2(roughness);
    float Theta = atan(sqrt((a * E.x) / (1.0 - E.x)));
    float Phi = PI2 * E.y;
    return MakeSample(vec2(Theta, Phi));
}

// cook-torrance
float G(float ndv, float ndh, float ndl, float vdh) {
    return min(1.0, min(2 * ((ndv * ndh) / vdh), 2 * ((ndl * ndh) / vdh)));
}

// Schlick
float F(float ndv, float metallic) {
    return metallic + (1 - metallic) * pow(1 - ndv, 5);
}

float cook_torrance(vec3 norm, vec3 light_dir, vec3 view_dir, float metallic) {
    vec3 h = normalize(light_dir + view_dir);
    float ndl = dot(norm, light_dir);
    float ndv = dot(norm, view_dir);
    float ndh = dot(norm, h);
    float vdh = dot(view_dir, h);
    return (D(ndh) * F(ndv, metallic) * G(ndv, ndh, ndl, vdh)) / (4 * ndl * ndv);
}

float radicalInverse_VdC(uint bits) {
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}

vec2 hammersley(uint i, uint N) {
    return vec2(float(i)/float(N), radicalInverse_VdC(i));
}

vec3 radiance(vec3 N, vec3 V, float metallic) {
    // Precalculate rotation for +Z Hemisphere to microfacet normal.
    vec3 UpVector = abs(N.z) < 0.999 ? ZAxis : XAxis;
    vec3 TangentX = normalize(cross(UpVector, N));
    vec3 TangentY = cross(N, TangentX);

    float NoV = abs(dot(N, V));

    // Approximate the integral for lighting contribution.
    vec3 fColor = vec3(0.0);
    const uint NumSamples = uint(20);
    for (uint i = uint(0); i < NumSamples; ++i)
    {
        vec2 Xi = hammersley(i, NumSamples);
        vec3 Li = S(Xi); // Defined elsewhere as subroutine
        vec3 H  = normalize(Li.x * TangentX + Li.y * TangentY + Li.z * N);
        vec3 L  = normalize(-reflect(V, H));

        // Calculate dot products for BRDF
        float NoL = abs(dot(N, L));
        float NoH = abs(dot(N, H));
        float VoH = abs(dot(V, H));

        float F_ = F(NoV, metallic); // Defined elsewhere as subroutine
        float G_ = G(NoV, NoH, NoL, VoH); // Defined elsewhere as subroutine
        vec3 LColor = texture(env, L, roughness * 10.f).rgb;

        // Since the sample is skewed towards the Distribution, we don't need
        // to evaluate all of the factors for the lighting equation. Also note
        // that this function is calculating the specular portion, so we absolutely
        // do not add any more diffuse here.
        fColor += F_ * G_ * LColor * VoH / (NoH * NoV);
    }

    // Average the results
    return fColor / float(NumSamples);
}

vec3 blend(vec3 diff, vec3 spec, vec3 base, float metallic) {
    float r = smoothstep(0.2, 0.45, metallic);
    vec3 dielectric = diff + spec;
    vec3 metal = spec * base;
    return mix(dielectric, metal, r);
}

void main() {
    vec3 light_dir = normalize(light_pos - v_pos);
    vec3 view_dir = normalize(cam_pos - v_pos);
    vec3 norm = normalize(v_normal);
    float m = clamp(i_metallic, 0.02, 0.99);

    vec3 spec = radiance(norm, view_dir, m);
    vec3 diff = diffuse_brdf(base) * texture(env, norm, MAXBIAS).rgb;

    result = vec4(blend(diff, spec, base, m), 1.0);
}
