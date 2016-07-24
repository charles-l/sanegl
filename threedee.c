#include "threedee.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define ENSURE(m, r, e, ...) if(!(m)) {fprintf(stderr, "%s:%i (" #m "): %s\n", __FILE__, __LINE__, e, ##__VA_ARGS__); r;}

static void show_info_log( // "da frick is this ugly thing", you ask? welcome to opengl error reporting >:)
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
    glDeleteShader(v_shader);
    glDeleteShader(f_shader);

    GLint program_ok;
    glGetProgramiv(program, GL_LINK_STATUS, &program_ok);
    if(!program_ok) { // lovely. More bloated error checking.
        show_info_log(program, glGetProgramiv, glGetProgramInfoLog);
        glDeleteProgram(program);
        return 0;
    }
    return program;
}

// returns a GLFWwindow
void init_threedee() {
    glewExperimental = 1;
    ENSURE(glewInit() == GLEW_OK, return, "failed to init glew");
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}

mesh_t create_mesh(float *vertices, float *normals, float *binormals, float *uvs, size_t vn) {
    mesh_t r;
    r.vn = vn;
    r.vao = create_vao();
    GLuint v = 0, n = 0, u = 0, bn = 0;
    v = create_buf(vertices, 3 * vn * sizeof(float), 3, 0);
    if(normals) n = create_buf(normals, 3 * vn * sizeof(float), 3, 1);
    if(binormals) bn = create_buf(binormals, 3 * vn * sizeof(float), 3, 2);
    if(uvs) u = create_buf(uvs, 2 * vn * sizeof(float), 2, 3);
    glBindVertexArray(0);
    glDeleteBuffers(1, &v); // cleanup i guess. i actually have no idea if this speeds things up. MY COPY PASTA IS SHOWING!
    glDeleteBuffers(1, &n);
    glDeleteBuffers(1, &u);
    glDeleteBuffers(1, &bn);
    return r;
}

GLuint create_vao() {
    GLuint va_id;
    glGenVertexArrays(1, &va_id);
    glBindVertexArray(va_id);
    return va_id;
}

// TODO: vbo indexing
GLuint create_buf(GLfloat *data, size_t size, GLuint vertex_size, GLuint attribi) {
    GLuint vb;
    glGenBuffers(1, &vb);
    glBindBuffer(GL_ARRAY_BUFFER, vb);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    glVertexAttribPointer(
            attribi,
            vertex_size, // number of elements per vertex (probably 3)
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0, // stride
            NULL);
    glEnableVertexAttribArray(attribi);
    return vb;
}

void draw_mesh(mesh_t *m) {
    glBindVertexArray(m->vao);
    glDrawArrays(GL_TRIANGLES, 0, m->vn);
    glBindVertexArray(0);
}

GLuint load_texi(img_t *i) { // load a texture from image data
    GLuint tex_id;
    glGenTextures(1, &tex_id);
    glBindTexture(GL_TEXTURE_2D, tex_id);
    glTexImage2D(GL_TEXTURE_2D,
            0,
            GL_RGB,
            i->w,
            i->h,
            0,
            GL_BGR,
            GL_UNSIGNED_BYTE,
            i->data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0); // unbind for next texture
    return tex_id;
}

GLuint load_tex(char *fname) { // load a texture from a filename
    FILE *imgf = fopen("t/tex.tga", "r");
    img_t *i = loadf_img(imgf);
    fclose(imgf);

    return load_texi(i);
}

void clear(GLfloat r, GLfloat g, GLfloat b) {
    glClearColor(r, g, b, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

img_t *loadf_img(FILE *f) {
    ENSURE(f, return NULL, "failed to load image");
    img_t *i = malloc(sizeof(img_t));
    int n;
    i->data = stbi_load_from_file(f, &(i->w), &(i->h), &n, 3);
    return i;
}

img_t *loadb_img(char *d, int len) {
    img_t *i = malloc(sizeof(img_t));
    int n;
    i->data = stbi_load_from_memory(d, len, &(i->w), &(i->h), &n, 3);
    return i;
}

void free_img(img_t *i) {
    stbi_image_free(i->data);
    free(i);
}
