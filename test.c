#include "draw.c"

typedef mat4x4 mat4;
int main() {
    float width = 800;
    float height = 600;

    // TODO: WRITE THIS CRAP IN CRSYTAL
    GLFWwindow *w = init();

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

    mat4 proj;
    mat4x4_perspective(proj, radians(45.0f), width / height, 0.1f, 100.0f);
    mat4 view;
    mat4x4_look_at(view,
            (vec3){4, 3, 3},
            (vec3){0, 0, 0},
            (vec3){0, 1, 0}
            );

    mat4 model = {1.0f};
    mat4 mvp;
    mat4x4_mul(mvp, proj, view);

    GLuint mat_id = glGetUniformLocation(p, "MVP");

    do {
        clear(0.0, 0.0, 0.3);
        glUseProgram(p);
        glUniformMatrix4fv(mat_id, 1, GL_FALSE, &mvp[0][0]);
        draw_buf(vb, 12 * 3);
        render_frame(w);
        glfwSwapBuffers(w);
        glfwPollEvents();
    } while (glfwGetKey(w, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(w) == 0);
}
