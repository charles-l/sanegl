// The main header file for the C OpenGL wrapper.
//
// The API aims to remove a lot of OpenGL boilerplate to clean up code, and make development easier and faster.
#ifndef LIB3DS_H
#define LIB3DS_H

#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linmath.h"

typedef struct {
    GLuint vao;
    size_t vn; // number of vertices
} mesh_t;

typedef struct {
    // Raw image data
    char *data;
    int w;
    int h;
} img_t;

// Create an OpenGL shader from a string and return the shader id.
GLuint load_shader(const char *s, int type);
// Create a program from a compiled vertex shader and fragment shader. `glDelete`s the two shaders passed to it. Returns the program id.
GLuint make_program(GLuint v_shader, GLuint f_shader);
// Initialize the OpenGL scene (assuming a context has been created)
void init_threedee();
// Create a texture buffer from image data
GLuint load_texi(img_t *i);
// Create a texture buffer from a filename
GLuint load_tex(char *filename);
// Draw an object from a VAO
void draw_vao(GLuint vao, unsigned int len);
// Clear the screen with a color
void clear(GLfloat r, GLfloat g, GLfloat b);
// Load an image from a file.
img_t *loadf_img(FILE *f);
// Load an image from a buffer in memory.
img_t *loadb_img(char *d, int len);
// Free an image
void free_img(img_t *i);

// ** Low Level Functionality **
// This stuff is used internally, but exposed in case you need/want low-level control

// Create a vertex array object (must be called before `create_buf`). Returns the VAO id.
GLuint create_vao();

// Create a vertex buffer object. Returns the VBO's id.
//
// * `data` - the raw vertex data in a one dimensional array
// * `size` - the actual size of data
// * `vertex_size` - the number of floats per vertex (3 for position/normal, 2 for uv)
// * `attribi` - the attrib pointer index of the new buffer
//
GLuint create_buf(GLfloat *data, size_t size, GLuint vertex_size, GLuint attribi);

#endif
