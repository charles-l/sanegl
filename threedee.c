#include "threedee.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define ENSURE(m, e) if(!(m)) {fprintf(stderr, #m ": %s\n", e); return NULL;}

static size_t get_fsize(FILE *f) {
    fseek(f, 0L, SEEK_END);
    size_t fsize = ftell(f);
    fseek(f, 0L, SEEK_SET);
    return fsize;
}

static obj_t *new_obj() {
    obj_t *o = malloc(sizeof(obj_t));
    o->polygon = malloc(sizeof(vec3) * MAX_POLYS);
    o->vertex = malloc(sizeof(vec3) * MAX_VERTS);
    o->mapcoord = malloc(sizeof(mapco_t) * MAX_VERTS);
    o->vlen = 0;
    o->plen = 0;
    o->tex_id = 0;
    return o;
}

obj_t **load_3ds_objs(char *fname) {
    FILE *f;
    unsigned short chunk_id;
    unsigned int chunk_len;
    unsigned short qty;
    unsigned short face_flags;
    if((f = fopen(fname, "rb")) == NULL) return NULL;

    int curo = -1; // hehehe the cleverness is overwhelming
    obj_t **objs = calloc(MAX_OBJS, sizeof(obj_t *));

    size_t fsize = get_fsize(f);

    while(ftell(f) < fsize) {
        int i;
        char c;

        fread(&chunk_id, 2, 1, f);
        fread(&chunk_len, 4, 1, f);

        switch(chunk_id)
        {
            case 0x4d4d: // main chunk
                break;
            case 0x3d3d: // 3d editor chunk
                break;
            case 0x4000: // object block
                i = 0;
                objs[++curo] = new_obj();
                do {
                    fread(&c, 1, 1, f);
                    objs[curo]->name[i] = c;
                } while (c != '\0' && i++ < 20);
                printf("%s\n", objs[curo]->name);
                break;
            case 0x4100: // triangle mesh (empty node)
                break;
            case 0x4110: // vertices list
                fread(&qty, sizeof(unsigned short), 1, f);
                objs[curo]->vlen = qty;
                fprintf(stderr, "%d vertices\n", qty);
                for(i = 0; i < qty; i++) {
                    fread(&objs[curo]->vertex[i][0], sizeof(float), 1, f);
                    fread(&objs[curo]->vertex[i][1], sizeof(float), 1, f);
                    fread(&objs[curo]->vertex[i][2], sizeof(float), 1, f);
                }
                break;
            case 0x4120: // chunk faces description
                fread(&qty, sizeof(unsigned short), 1, f);
                objs[curo]->plen = qty;
                fprintf(stderr, "%d polys\n", qty);
                for(i = 0; i < qty; i++) {
                    fread(&objs[curo]->polygon[i].a, sizeof(unsigned short), 1, f);
                    fread(&objs[curo]->polygon[i].b, sizeof(unsigned short), 1, f);
                    fread(&objs[curo]->polygon[i].c, sizeof(unsigned short), 1, f);
                    fread(&face_flags, sizeof(unsigned short), 1, f);
                }
                break;
            case 0x4140: // mapping coordinate list
                fread(&qty, sizeof(unsigned short), 1, f);
                for(i = 0; i < qty; i++) {
                    fread(&objs[curo]->mapcoord[i].u, sizeof(float), 1, f);
                    fread(&objs[curo]->mapcoord[i].v, sizeof(float), 1, f);
                }
                break;
            default:
                fseek(f, chunk_len - 6, SEEK_CUR);
        }
    }
    fclose(f);
    return objs;
}

void free_obj(obj_t *o) {
    if(!o) return;
    free(o);
}

void free_objs(obj_t **o) {
    for(int i = 0; o[i] != NULL; i++) {
        free(o[i]);
    }
    free(o);
}

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

// returns a GLFWwindow
void *init_draw(int width, int height) {
    ENSURE(glfwInit(), "failed to initialize GLFW");
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *w;
    ENSURE((w = glfwCreateWindow(width, height, "game", NULL, NULL)), "window failed");
    glfwMakeContextCurrent(w);
    glewExperimental = 1;
    ENSURE(glewInit() == GLEW_OK, "failed to init glew");
    glfwSetInputMode(w, GLFW_STICKY_KEYS, GL_TRUE);
    //glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    return (void *) w;
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

void update_frame(void *w) {
    glfwSwapBuffers((GLFWwindow *) w);
    glfwPollEvents(); // hehe. do something sane with this plz
}

void calc_mvp(mat4x4 mvp, mat4x4 model, vec3 pos, vec3 dir, vec3 up, float fov, float width, float height) {
    // this horrible mess is what makes objects move in 3D space. Hoorah. CAN YOU FEEL THE MATH YET?!!!
    mat4x4 proj;
    mat4x4 view;
    mat4x4_perspective(proj, radians(fov), width / height, 0.1f, 100.0f);
    vec3 tmp;
    vec3_add(tmp, pos, dir);
    mat4x4_look_at(view,
            pos,
            tmp,
            up);
    mat4x4_mul(mvp, proj, view);
    mat4x4_mul(mvp, mvp, model);
}
