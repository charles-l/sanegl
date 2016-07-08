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

    GLuint vb = create_buf(box, 12 * 3 * 3 * sizeof(float));
    GLuint vnb = create_buf(box, 12 * 3 * 3 * sizeof(float));

    // TODO: move to files or something
    const char *v_shader = "#version 330 core \n\
                            layout(location = 0) in vec3 v_pos; \
                            layout(location = 1) in vec3 v_normal; \
                            layout(location = 2) in vec3 uv; \
                            uniform mat4 M;\
                            uniform mat4 V;\
                            uniform mat4 P;\
                            void main() { \
                                gl_Position = vec4(v_pos, 1); \
                            }";

    const char *f_shader = "#version 330 \n\
                            out vec3 color; \
                            void main() { \
                                color = vec3(1.0, 1.0, 1.0); \
                            }";

    GLuint v = load_shader(v_shader, GL_VERTEX_SHADER);
    GLuint f = load_shader(f_shader, GL_FRAGMENT_SHADER);
    GLuint p = make_program(v, f);

    mat4x4 model = {0};
    mat4x4 view;
    mat4x4 proj;
    mat4x4_diag(model, 1.0f);
    mat4x4_look_at(view, (vec3){4, 3, -3}, (vec3){0, 0, 0}, (vec3){0, 1, 0});
    mat4x4_perspective(proj, radians(45.0), 4.0f/3.0f, 0.1f, 100.0f);

    double dt;
    double last_time = 0;
    do {
        dt = glfwGetTime() - last_time;
        last_time = glfwGetTime(); // set last time

        GLuint model_id = glGetUniformLocation(p, "M");
        GLuint view_id = glGetUniformLocation(p, "V");
        GLuint proj_id = glGetUniformLocation(p, "P");
        glUniformMatrix4fv(model_id, 1, GL_FALSE, &model[0][0]);
        glUniformMatrix4fv(view_id, 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(proj_id, 1, GL_FALSE, &proj[0][0]);

        clear(0.0, 0.1, 0.3);
        glUseProgram(p);
        draw_buf(vb, vnb, 0, 36);
        update_frame(w);
    } while (glfwGetKey(w, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(w) == 0);
}
