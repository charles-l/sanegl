#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "linmath.h"
#include "threedee.h"
#define ENSURE(m, e) if(!(m)) {fprintf(stderr, #m ": %s\n", e); return NULL;}

static void show_info_log( // da frick is this ugly thing you ask? welcome to opengl error reporting >:)
        GLuint object,
        PFNGLGETSHADERIVPROC glGet__iv,
        PFNGLGETSHADERINFOLOGPROC glGet__InfoLog
        )
{
    GLint log_length;
    char *log;

    glGet__iv(object, GL_INFO_LOG_LENGTH, &log_length);
    log = (char *) malloc(log_length);
    glGet__InfoLog(object, log_length, NULL, log);
    fprintf(stderr, "%s", log);
    free(log);
}

GLuint load_shader(const char *s, int type) {
    GLuint shader = glCreateShader(type);
    GLint len = strlen(s);
    glShaderSource(shader, 1, (const GLchar **) &s, &len); // HAHAHAH SO OPENGL DOESNT KNOW HOW TO USE STRINGS WITH NULL TERMINATORS HYSTERICAL
    glCompileShader(shader);

    // yes. Error checking takes lots of lines. Goodbye low sloc. Thank you OpenGL >:(
    int status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE) {
        show_info_log(shader, glGetShaderiv, glGetShaderInfoLog);
        glDeleteShader(shader);
        return 0; // failed.
    }
    return shader;
}

GLuint make_program(GLuint v_shader, GLuint f_shader) {
    GLuint program = glCreateProgram();
    glAttachShader(program, v_shader);
    glAttachShader(program, f_shader);
    glLinkProgram(program);

    GLint program_ok;
    glGetProgramiv(program, GL_LINK_STATUS, &program_ok);
    if(!program_ok) { // lovely. More bloated error checking.
        show_info_log(program, glGetProgramiv, glGetProgramInfoLog);
        glDeleteProgram(program);
        return 0;
    }
    return program;
}

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
    glewExperimental = 1;
    ENSURE(glewInit() == GLEW_OK, "failed to init glew");
    glfwSetInputMode(w, GLFW_STICKY_KEYS, GL_TRUE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glDrawArrays(GL_TRIANGLES, 0, len);
    glDisableVertexAttribArray(0);
}

void clear(GLfloat r, GLfloat g, GLfloat b) {
    glClearColor(r, g, b, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void render_frame(GLFWwindow *w) {
    glfwSwapBuffers(w);
    glfwPollEvents();
}
