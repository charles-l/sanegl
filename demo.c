#include "threedee.c"

#define UNPACK3(f) f[0], f[1], f[2]

// used for debug:
#define PRINTVEC(v) printf(#v ": %f %f %f\n", UNPACK3(v));

vec3 dir = {0, 0, 0};
vec3 up = {0};
vec3 position = {0, 0, 5};
float h_angle = 3.14f;
float v_angle = 0.0;
float fov = 45.0f;
float speed = 5.0f;
float mouse_speed = 0.005f;

int width = 800;
int height = 600;


void calc_mvp(mat4x4 mvp, mat4x4 model, vec3 pos, vec3 goal, vec3 up, float fov, float width, float height) {
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

int main() {
    GLFWwindow *w = init_draw(width, height);

    GLfloat box[] = {
        -1.0f,-1.0f,-1.0f, // triangle 1 : begin
        -1.0f,-1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f, // triangle 1 : end
        1.0f, 1.0f,-1.0f, // triangle 2 : begin
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f, // triangle 2 : end
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f
    };

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

    FILE *imgf = fopen("c/tex.tga", "r");
    img_t *i = loadf_img(imgf);
    fclose(imgf);

    GLuint tex_id;
    glGenTextures(1, &tex_id);
    glBindTexture(GL_TEXTURE_2D, tex_id);
    glTexImage2D(GL_TEXTURE_2D,
            0,
            GL_RGB,
            i->w,
            i->h,
            0,
            GL_BGR,
            GL_UNSIGNED_BYTE,
            i->data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0); // unbind for next texture

    GLuint va = create_va();
    GLuint vb = create_buf(box, 36 * 3 * sizeof(float));
    GLuint vnb = create_buf(box, 36 * 3 * sizeof(float));
    GLuint uvb = create_buf(box_uv, 36 * 2 * sizeof(float));

    const char *v_shader = "#version 330 core \n\
                            layout(location = 0) in vec3 v_pos; \
                            layout(location = 1) in vec3 v_normal; \
                            layout(location = 2) in vec2 v_uv; \
                            uniform mat4 MVP;\
                            out vec2 UV; \
                            void main() { \
                                gl_Position = MVP * vec4(v_pos, 1); \
                                UV = v_uv; \
                            }";

    const char *f_shader = "#version 330 \n\
                            in vec2 UV; \
                            out vec3 color; \
                            uniform sampler2D tex; \
                            void main() { \
                                color = texture(tex, UV).rgb; \
                            }";

    GLuint v = load_shader(v_shader, GL_VERTEX_SHADER);
    GLuint f = load_shader(f_shader, GL_FRAGMENT_SHADER);
    GLuint p = make_program(v, f);

    mat4x4 model = {0};
    mat4x4_diag(model, 1.f);

    double dt;
    double last_time = 0;
    mat4x4 mvp;
    do {
        dt = glfwGetTime() - last_time;
        last_time = glfwGetTime(); // set last time

        calc_mvp(mvp, model, (vec3){4,3,-3}, (vec3){0,0,0}, (vec3){0,1,0}, 45.f, width, height);

        GLuint mvp_id = glGetUniformLocation(p, "MVP");
        glUniformMatrix4fv(mvp_id, 1, GL_FALSE, &mvp[0][0]);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex_id);

        GLuint tex_unit = glGetUniformLocation(p, "tex");
        glUniform1i(tex_unit, 0);

        clear(0.0, 0.1, 0.3);
        glUseProgram(p);
        draw_buf(vb, vnb, uvb, 36);
        update_frame(w);
    } while (glfwGetKey(w, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(w) == 0);
}
