#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
using namespace glm;
#define ENSURE(m, e) if(!(m)) {fprintf(stderr, #m ": %s\n", e); return NULL;}

typedef struct {
    GLuint vb;
    // attrib array maybe?
} obj_t;

GLFWwindow *init() {
    ENSURE(glfwInit(), "failed to initialize GLFW");
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *w;
    ENSURE((w = glfwCreateWindow(1280, 720, "game", NULL, NULL)), "window failed");
    glfwMakeContextCurrent(w);
    glewExperimental = true;
    ENSURE(glewInit() == GLEW_OK, "failed to init glew");
    glfwSetInputMode(w, GLFW_STICKY_KEYS, GL_TRUE);

    return w;
}

GLuint create_buf(GLfloat *data, size_t size) {
    GLuint va_id;
    glGenVertexArrays(1, &va_id);
    glBindVertexArray(va_id);

    GLuint vb;
    glGenBuffers(1, &vb);
    glBindBuffer(GL_ARRAY_BUFFER, vb);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    return vb;
}

void draw_buf(GLuint vb, unsigned int len) {
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vb);
    glVertexAttribPointer(0, len, GL_FLOAT, GL_FALSE, 0, NULL);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(0);
}

void clear() {
    glClearColor(0.0, 0.4, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
}

void render_frame(GLFWwindow *w) {
    glfwSwapBuffers(w);
    glfwPollEvents();
}
