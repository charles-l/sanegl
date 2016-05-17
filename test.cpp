#include "draw.cpp"

int main() {
    // TODO: WRITE THIS CRAP IN CRSYTAL
    GLFWwindow *w = init();
    GLfloat x[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };
    GLfloat y[] = {
        -1.0f, 1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };
    GLuint vb = create_buf(x, 9 * sizeof(float));
    GLuint vb2 = create_buf(y, 9 * sizeof(float));

    // TODO: move to files or something
    const char *v_shader = "#version 330 core \n\
                            layout(location = 0) in vec3 vertexPosition_modelspace;\
                            void main() { \
                                gl_Position.xyz = vertexPosition_modelspace; \
                                    gl_Position.w = 1.0; \
                            }";

    const char *f_shader = "#version 330 \n\
                            out vec3 color; \
                            void main() { \
                                color = vec3(1.0, 1.0, 1.0); \
                            }";

    GLuint v = load_shader(v_shader, GL_VERTEX_SHADER);
    GLuint f = load_shader(f_shader, GL_FRAGMENT_SHADER);
    GLuint p = make_program(v, f);

    do {
        clear(0.0, 1.0, 0.0);
        glUseProgram(p);
        draw_buf(vb2, 3);
        draw_buf(vb, 3);
        render_frame(w);
        glfwSwapBuffers(w);
        glfwPollEvents();
    } while (glfwGetKey(w, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(w) == 0);
}
