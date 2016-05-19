#include "draw.c"

vec3 position = (vec3) {0, 0, 5};
float h_angle = 3.14f;
float v_angle = 0.0;
float fov = 45.0f;
float speed = 3.0f;
float mouse_speed = 0.005f;

int width = 800;
int height = 600;

void handle_input(GLFWwindow *w) {
    double xpos, ypos;
    glfwGetCursorPos(w, &xpos, &ypos);
    h_angle += mouse_speed * (width/2 - xpos); //*dt
    v_angle += mouse_speed * (height/2 - ypos); //*dt
    vec3 dir;
    dir[0] = cos(v_angle) * sin(h_angle);
    dir[1] = sin(v_angle);
    dir[2] = cos(v_angle) * cos(h_angle);

    vec3 r;
    r[0] = sin(h_angle - M_PI/2.0f);
    r[1] = 0;
    r[0] = cos(h_angle - M_PI/2.0f);
    vec3 up;
    vec3_mul_cross(up, r, dir);
    // TODO: keyboard input
}

int main() {
    // TODO: WRITE THIS CRAP IN CRSYTAL
    GLFWwindow *w = init(width, height);

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

    // TODO: move to files or something
    const char *v_shader = "#version 330 core \n\
                            layout(location = 0) in vec3 vertexPosition_modelspace;\
                            uniform mat4 MVP;\
                            void main() { \
                                gl_Position = MVP * vec4(vertexPosition_modelspace, 1); \
                            }";

    const char *f_shader = "#version 330 \n\
                            out vec3 color; \
                            void main() { \
                                color = vec3(1.0, 1.0, 1.0); \
                            }";

    GLuint v = load_shader(v_shader, GL_VERTEX_SHADER);
    GLuint f = load_shader(f_shader, GL_FRAGMENT_SHADER);
    GLuint p = make_program(v, f);

    mat4x4 proj;
    mat4x4 view;
    mat4x4 mvp;
    mat4x4 model;
    mat4x4_diag(model, 1.0f);
    mat4x4_perspective(proj, radians(45), width / height, 0.1f, 100.0f);

    mat4x4_look_at(view,
            (vec3){4, 3, 3},
            (vec3){0, 0, 0},
            (vec3){0, 1, 0}
            );

    mat4x4_mul(mvp, proj, view);
    mat4x4_mul(mvp, mvp, model);

    GLuint mat_id = glGetUniformLocation(p, "MVP");

    do {
        handle_input(w);
        clear(0.0, 0.0, 0.3);
        glUseProgram(p);
        glUniformMatrix4fv(mat_id, 1, GL_FALSE, &mvp[0][0]);
        draw_buf(vb, 12 * 3);
        render_frame(w);
        glfwSwapBuffers(w);
        glfwPollEvents();
    } while (glfwGetKey(w, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(w) == 0);
}
