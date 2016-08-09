#include "sgl_draw.h"
#include "sgl_mesh_loader.h"
#include <assert.h>
#include <err.h>
#include <GLFW/glfw3.h>

#define UNPACK3(f) f[0], f[1], f[2]

// used for debug:
#define PRINTVEC(v) printf(#v ": %f %f %f\n", UNPACK3(v));

v3 dir = (0);
v3 up = (0);
v3 position = {0, 0, 5};
float h_angle = 3.14f;
float v_angle = 0.0;
float fov = 45.0f;
float speed = 5.0f;
float mouse_speed = 0.005f;

int width = 800;
int height = 600;

struct materal_t {
    GLuint prog;
    float reflectivity;
    float diffuse;
    float roughness;
    float fresnal;
};

void calc_mvp(mat4x4 mvp, mat4x4 model, v3 pos, v3 goal, v3 up, float fov, float width, float height) {
    // this horrible mess is what makes objects move in 3D space. Hoorah. CAN YOU FEEL THE MATH YET?!!!
    mat4x4 proj;
    mat4x4 view;
    mat4x4_perspective(proj, radians(fov), width / height, 0.1f, 100.0f);
    mat4x4_look_at(view,
            pos,
            goal,
            up);
    mat4x4_mul(mvp, proj, view);
    mat4x4_mul(mvp, mvp, model);
}

void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

int main() {

    /////

    glfwSetErrorCallback(error_callback);
    assert(glfwInit());
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow *w = glfwCreateWindow(width, height, "game", NULL, NULL);
    glfwMakeContextCurrent(w);
    init_threedee(width, height);

    ////

    // MODELS

    GLfloat box_uv[] = {
        0.000059f, 1.0f-0.000004f,
        0.000103f, 1.0f-0.336048f,
        0.335973f, 1.0f-0.335903f,
        1.000023f, 1.0f-0.000013f,
        0.667979f, 1.0f-0.335851f,
        0.999958f, 1.0f-0.336064f,
        0.667979f, 1.0f-0.335851f,
        0.336024f, 1.0f-0.671877f,
        0.667969f, 1.0f-0.671889f,
        1.000023f, 1.0f-0.000013f,
        0.668104f, 1.0f-0.000013f,
        0.667979f, 1.0f-0.335851f,
        0.000059f, 1.0f-0.000004f,
        0.335973f, 1.0f-0.335903f,
        0.336098f, 1.0f-0.000071f,
        0.667979f, 1.0f-0.335851f,
        0.335973f, 1.0f-0.335903f,
        0.336024f, 1.0f-0.671877f,
        1.000004f, 1.0f-0.671847f,
        0.999958f, 1.0f-0.336064f,
        0.667979f, 1.0f-0.335851f,
        0.668104f, 1.0f-0.000013f,
        0.335973f, 1.0f-0.335903f,
        0.667979f, 1.0f-0.335851f,
        0.335973f, 1.0f-0.335903f,
        0.668104f, 1.0f-0.000013f,
        0.336098f, 1.0f-0.000071f,
        0.000103f, 1.0f-0.336048f,
        0.000004f, 1.0f-0.671870f,
        0.336024f, 1.0f-0.671877f,
        0.000103f, 1.0f-0.336048f,
        0.336024f, 1.0f-0.671877f,
        0.335973f, 1.0f-0.335903f,
        0.667969f, 1.0f-0.671889f,
        1.000004f, 1.0f-0.671847f,
        0.667979f, 1.0f-0.335851f
    };

    GLfloat skybox_data[] = {
        // Positions
        -5.0f,  5.0f, -5.0f,
        -5.0f, -5.0f, -5.0f,
        5.0f, -5.0f, -5.0f,
        5.0f, -5.0f, -5.0f,
        5.0f,  5.0f, -5.0f,
        -5.0f,  5.0f, -5.0f,

        -5.0f, -5.0f,  5.0f,
        -5.0f, -5.0f, -5.0f,
        -5.0f,  5.0f, -5.0f,
        -5.0f,  5.0f, -5.0f,
        -5.0f,  5.0f,  5.0f,
        -5.0f, -5.0f,  5.0f,

        5.0f, -5.0f, -5.0f,
        5.0f, -5.0f,  5.0f,
        5.0f,  5.0f,  5.0f,
        5.0f,  5.0f,  5.0f,
        5.0f,  5.0f, -5.0f,
        5.0f, -5.0f, -5.0f,

        -5.0f, -5.0f,  5.0f,
        -5.0f,  5.0f,  5.0f,
        5.0f,  5.0f,  5.0f,
        5.0f,  5.0f,  5.0f,
        5.0f, -5.0f,  5.0f,
        -5.0f, -5.0f,  5.0f,

        -5.0f,  5.0f, -5.0f,
        5.0f,  5.0f, -5.0f,
        5.0f,  5.0f,  5.0f,
        5.0f,  5.0f,  5.0f,
        -5.0f,  5.0f,  5.0f,
        -5.0f,  5.0f, -5.0f,

        -5.0f, -5.0f, -5.0f,
        -5.0f, -5.0f,  5.0f,
        5.0f, -5.0f, -5.0f,
        5.0f, -5.0f, -5.0f,
        -5.0f, -5.0f,  5.0f,
        5.0f, -5.0f,  5.0f
    };


    // IMAGE DATA
    img_t cubemap[6];
    for(int i = 0; i < 6; i++) {
        char name[64];
        name[0] = '\0';
        strcat(name, "t/skybox/");
        sprintf(name, "%s%i.tga", name, i);
        puts(name);
        FILE *f = fopen(name, "r");
        cubemap[i] = *(loadf_img(f));
        fclose(f);
    }

    raw_mesh_t *o = load_3ds_objs("t/monkey.3ds");
    gen_normals(&(o[0]));

    float x[9 * o[0].plen];
    float nx[9 * o[0].plen];
    float bnx[9 * o[0].plen];
    size_t xn = sizeof(x) / (sizeof(float) * 3);
    float *v = x - 1;
    float *n = nx - 1;
    float *bn = bnx - 1;
    for(int i = 0; i < o[0].plen; i++) {
        polygon_t p = o[0].polygons[i];
        *(++v) = o[0].vertices[p.a][0];
        *(++v) = o[0].vertices[p.a][1];
        *(++v) = o[0].vertices[p.a][2];

        *(++v) = o[0].vertices[p.b][0];
        *(++v) = o[0].vertices[p.b][1];
        *(++v) = o[0].vertices[p.b][2];

        *(++v) = o[0].vertices[p.c][0];
        *(++v) = o[0].vertices[p.c][1];
        *(++v) = o[0].vertices[p.c][2];

        ///

        *(++n) = o[0].normals[p.a][0];
        *(++n) = o[0].normals[p.a][1];
        *(++n) = o[0].normals[p.a][2];

        *(++n) = o[0].normals[p.b][0];
        *(++n) = o[0].normals[p.b][1];
        *(++n) = o[0].normals[p.b][2];

        *(++n) = o[0].normals[p.c][0];
        *(++n) = o[0].normals[p.c][1];
        *(++n) = o[0].normals[p.c][2];

        ///

        *(++bn) = o[0].binormals[p.a][0];
        *(++bn) = o[0].binormals[p.a][1];
        *(++bn) = o[0].binormals[p.a][2];

        *(++bn) = o[0].binormals[p.b][0];
        *(++bn) = o[0].binormals[p.b][1];
        *(++bn) = o[0].binormals[p.b][2];

        *(++bn) = o[0].binormals[p.c][0];
        *(++bn) = o[0].binormals[p.c][1];
        *(++bn) = o[0].binormals[p.c][2];

        //printf("%i: p.a %f %f %f\n", i, UNPACK3(o[0].normals[p.a]));
        //printf("%i: p.b %f %f %f\n", i, UNPACK3(o[0].normals[p.b]));
        //printf("%i: p.c %f %f %f\n", i, UNPACK3(o[0].normals[p.c]));
    }

    free(o);

    mesh_t monkey = create_mesh(x, nx, bnx, NULL, xn);
    tex_t monkey_tex = load_tex("t/tex.tga");

    mesh_t skybox = create_mesh(skybox_data, skybox_data, NULL, NULL, 36);
    tex_t skybox_texture = load_cubemapi(cubemap);

    // TODO: normal map
    // TODO: shadow map
    GLuint p = make_program(load_shader("t/v_shader.glsl", GL_VERTEX_SHADER), load_shader("t/pbr.glsl", GL_FRAGMENT_SHADER));
    GLuint p2 = make_program(load_shader("t/v_skybox.glsl", GL_VERTEX_SHADER), load_shader("t/skybox.glsl", GL_FRAGMENT_SHADER));

    mat4x4 model = {0};
    mat4x4_diag(model, 1.f);

    v3 pos = (v3){4.f, 3.f, -3.f};
    double dt;
    double last_time = 0;
    mat4x4 mvp;
    do {
        dt = glfwGetTime() - last_time;
        printf("%fms\n", dt * 1000);
        last_time = glfwGetTime(); // set last time

        pos.x = sin(glfwGetTime());
        pos.y = sin(glfwGetTime());
        calc_mvp(mvp, model, pos, (v3){0,0,0}, (v3){0,1,0}, 45.f, width, height);

        clear(0.0, 0.1, 0.3);

        // skybox
        glUseProgram(p2);
        glUniformMatrix4fv(glGetUniformLocation(p2, "MVP"), 1, GL_FALSE, (void *) &mvp[0]);
        bind_tex(p2, "skybox", skybox_texture, 0);
        draw_skybox(&skybox);

        glUseProgram(p);
        glUniformMatrix4fv(glGetUniformLocation(p, "MVP"), 1, GL_FALSE, (void *) &mvp[0]);

        bind_tex(p, "tex", monkey_tex, 0);
        bind_tex(p, "env", skybox_texture, 1);
        draw_mesh(&monkey);
        glfwSwapBuffers((GLFWwindow *) w);
    } while (glfwGetKey(w, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(w) == 0);
    return 0;
}
