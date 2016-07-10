// The main header file for the C OpenGL wrapper.
//
// The API aims to remove a lot of OpenGL boilerplate to clean up code, and make development easier and faster.
#ifndef LIB3DS_H
#define LIB3DS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linmath.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Some of these `#define`s might need to be tweaked, depending on the complexity of your scenes.
#define MAX_VERTS 20000
#define MAX_POLYS 20000
// Max number of objects in a single file
#define MAX_OBJS 16

// **Polygon type**
typedef struct {
    // `a`, `b`, and `c` map to the indexes of vertices in another array
    int a, b, c;
} poly_t;

// **Texture Map coordinates**
typedef struct {
    float u, v;
} mapco_t;

// **3D Object**
typedef struct {
    poly_t *polygon;
    vec3 *vertex;
    mapco_t *mapcoord;
    // Number of vertices
    int vlen;
    // Number of polygons
    int plen;
    // Texture id if the file contains multiple images
    int tex_id;
    // name of the object
    char name[20];
} obj_t;

// ## Image type
typedef struct {
    // Raw image data
    char *data;
    int w;
    int h;
} img_t;

// Loads an array of 3D objects from a `.3ds` file, and returns the array.
obj_t **load_3ds_objs(char *fname);
// Free a single object
void free_obj(obj_t *o);
// Free the whole returned obj array
void free_objs(obj_t **o);
// Create an OpenGL shader from a string and return the shader id.
GLuint load_shader(const char *s, int type);
// Create a program from a compiled vertex shader and fragment shader. Returns the program id.
GLuint make_program(GLuint v_shader, GLuint f_shader);
// Initialize the OpenGL scene, and returns a window. (TODO: remove the windowing code, and just initailize the OpenGL context to make it usable with any GL toolkit).
void *init_draw(int width, int height);
// Create a vertex array object (must be called before `create_buf`). Returns the VAO id.
GLuint create_va();
// Create a vertex buffer object. Returns the VBO id.
GLuint create_buf(GLfloat *data, size_t size);
// Enable a vertex attribute (handled internally)
void enable_attrib(GLuint attrib, GLuint buf_id, GLint len);
// Draw an object from a vertex buffer, normal buffer and uv buffer
void draw_buf(GLuint vb, GLuint nb, GLuint uvb, unsigned int len);
// Clear the screen with a color
void clear(GLfloat r, GLfloat g, GLfloat b);
// Redraw the screen (flip buffer, update window, etc)
void update_frame(void *w);
// Load an image from a file.
img_t *loadf_img(FILE *f);
// Load an image from a buffer in memory.
img_t *loadb_img(char *d, int len);
// Free an image
void free_img(img_t *i);

#endif
