#include "draw.cpp"

int main() {
    float width = 800;
    float height = 600;

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

    glm::mat4 proj = glm::perspective(glm::radians(45.0f), width / height, 0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(
            glm::vec3(4, 3, 3),
            glm::vec3(0, 0, 0),
            glm::vec3(0, 1, 0)
            );

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 mvp = proj * view * model;

    GLuint mat_id = glGetUniformLocation(p, "MVP");

    do {
        clear(0.0, 0.0, 0.3);
        glUseProgram(p);
        glUniformMatrix4fv(mat_id, 1, GL_FALSE, &mvp[0][0]);
        draw_buf(vb2, 3);
        draw_buf(vb, 3);
        render_frame(w);
        glfwSwapBuffers(w);
        glfwPollEvents();
    } while (glfwGetKey(w, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(w) == 0);
}
