#include "sgl_draw.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void _bind_tex(GLuint prog, char *uname, GLenum type, GLuint id, int slot);

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

GLuint load_shader(const char *filename, int type) {
    FILE *f = fopen(filename, "r");
    assert(f);
    fseek(f, 0, SEEK_END);
    int fsize = ftell(f);
    rewind(f);
    char *s = malloc(sizeof(char) * fsize);
    fread(s, sizeof(char), fsize, f);
    fclose(f);

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, (const GLchar **) &s, &fsize); // HAHAHAH SO OPENGL DOESNT KNOW HOW TO USE STRINGS WITH NULL TERMINATORS HYSTERICAL
    glCompileShader(shader);

    free(s);

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

void init_sgl() {
    glewExperimental = 1;
    assert(glewInit() == GLEW_OK);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    glDepthFunc(GL_LESS);
}

gl_mesh_t create_gl_mesh(float *vertices, float *normals, float *uvs, size_t vn) {
    gl_mesh_t r;
    r.vn = vn;
    r.vao = create_vao();
    GLuint v = 0, n = 0, u = 0, bn = 0;
    v = create_buf(vertices, 3 * vn * sizeof(float), 3, 0);
    if(normals) n = create_buf(normals, 3 * vn * sizeof(float), 3, 1);
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

void draw_mesh(gl_mesh_t *m) {
    glBindVertexArray(m->vao);
    glDrawArrays(GL_TRIANGLES, 0, m->vn);
    glBindVertexArray(0);
}

void draw_skybox(gl_mesh_t *skybox) {
    glDepthMask(GL_FALSE);
    glBindVertexArray(skybox->vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthMask(GL_TRUE);
}

tex_t load_cubemapi(img_t imgs[6]) {
    tex_t t;
    t.type = GL_TEXTURE_CUBE_MAP;
    t.image = imgs;
    glGenTextures(1, &t.id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, t.id);
    for(GLuint i = 0; i < 6; i++) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, imgs[i].w, imgs[i].h, 0, GL_RGB, GL_UNSIGNED_BYTE, imgs[i].data);
    }
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0); // unbind for next texture
    return t;
}

tex_t load_texi(img_t *i) { // load a texture from image data
    tex_t t;
    t.type = GL_TEXTURE_2D;
    t.image = i;
    glGenTextures(1, &t.id);
    glBindTexture(GL_TEXTURE_2D, t.id);
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
    return t;
}

tex_t load_tex(char *fname) { // load a texture from a filename
    FILE *imgf = fopen("t/tex.tga", "r");
    img_t *i = loadf_img(imgf);
    fclose(imgf);

    return load_texi(i);
}

void bind_mat4x4(GLuint prog, char *uname, void *data) {
    glUniformMatrix4fv(glGetUniformLocation(prog, uname), 1, GL_FALSE, data);
}

void bind_v3(GLuint prog, char *uname, float *data) {
    glUniform3f(glGetUniformLocation(prog, uname), data[0], data[1], data[2]);
}

void bind_tex(GLuint prog, char *uname, tex_t tex, int slot) {
    _bind_tex(prog, uname, tex.type, tex.id, slot);
}

void _bind_tex(GLuint prog, char *uname, GLenum type, GLuint id, int slot) {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(type, id);
    glUniform1i(glGetUniformLocation(prog, uname), 0); // TODO: maybe move to `program_t` stuff once more is implemented (with multiple slots handled, etc)
}

void clear(GLfloat r, GLfloat g, GLfloat b) {
    glClearColor(r, g, b, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

img_t *loadf_img(FILE *f) {
    assert(f);
    img_t *i = malloc(sizeof(img_t));
    int n;
    i->data = stbi_load_from_file(f, &(i->w), &(i->h), &n, 3);
    return i;
}

img_t *loadb_img(unsigned char *d, int len) {
    img_t *i = malloc(sizeof(img_t));
    int n;
    i->data = stbi_load_from_memory(d, len, &(i->w), &(i->h), &n, 3);
    return i;
}

void free_img(img_t *i) {
    stbi_image_free(i->data);
    free(i);
}
