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
    do {
        clear();
        draw_buf(vb2, 3);
        draw_buf(vb, 3);
        render_frame(w);
        glfwSwapBuffers(w);
        glfwPollEvents();
    } while (glfwGetKey(w, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(w) == 0);
}
