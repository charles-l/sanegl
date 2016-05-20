#include "draw.c"

#define UNPACK3(f) f[0], f[1], f[2]

// used for debug:
#define PRINTVEC(v) printf(#v ": %f %f %f\n", UNPACK3(v));

vec3 dir = {0, 0, 0};
vec3 up = {0};
vec3 position = {0, 0, 5};
float h_angle = 3.14f;
float v_angle = 0.0;
float fov = 45.0f;
float speed = 5.0f;
float mouse_speed = 0.005f;

int width = 800;
int height = 600;

void handle_input(GLFWwindow *w, float dt) {
    double xpos, ypos;
    glfwGetCursorPos(w, &xpos, &ypos);
    glfwSetCursorPos(w, width/2, height/2);
    h_angle += mouse_speed * dt * (float)( width/2 - xpos);
    v_angle += mouse_speed * dt * (float)(height/2 - ypos);

    dir[0] = cos(v_angle) * sin(h_angle);
    dir[1] = sin(v_angle);
    dir[2] = cos(v_angle) * cos(h_angle);

    vec3 r = {
        sin(h_angle - M_PI/2.0f),
        0,
        cos(h_angle - M_PI/2.0f)
    };
    vec3_mul_cross(up, r, dir);

    // ignore this ugliness for now. it'll eventually be crystal code.
    vec3 forward;
    vec3_scale(forward, dir, speed * dt);
    if(glfwGetKey(w, GLFW_KEY_UP) == GLFW_PRESS) {
        vec3_add(position, position, forward);
    }
    if(glfwGetKey(w, GLFW_KEY_DOWN) == GLFW_PRESS) {
        vec3_sub(position, position, forward);
    }

    vec3 strafe;
    vec3_scale(strafe, r, speed * dt);
    if(glfwGetKey(w, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        vec3_add(position, position, strafe);
    }
    if(glfwGetKey(w, GLFW_KEY_LEFT) == GLFW_PRESS) {
        vec3_sub(position, position, strafe);
    }
}

int main() {
    // TODO: WRITE THIS CRAP IN CRSYTAL
    GLFWwindow *w = init(width, height);

    glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    GLfloat box[] = {
        -1.0f,-1.0f,-1.0f, // triangle 1 : begin
        -1.0f,-1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f, // triangle 1 : end
        1.0f, 1.0f,-1.0f, // triangle 2 : begin
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f, // triangle 2 : end
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f
    };

    GLuint vb = create_buf(box, 12 * 3 * 3 * sizeof(float));

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

    mat4x4 proj;
    mat4x4 view;
    mat4x4 mvp;
    mat4x4 model;
    vec3 tmp;
    mat4x4_diag(model, 1.0f);

    double dt;
    double last_time = 0;
    do {
        dt = glfwGetTime() - last_time;
        last_time = glfwGetTime(); // set last time

        printf("dt: %f\n", dt);
        handle_input(w, dt);

        mat4x4_perspective(proj, radians(fov), width / height, 0.1f, 100.0f);
        vec3_add(tmp, position, dir);
        mat4x4_look_at(view,
                position,
                tmp,
                up);

        mat4x4_mul(mvp, proj, view);
        //mat4x4_mul(mvp, mvp, model);

        GLuint mat_id = glGetUniformLocation(p, "MVP");

        clear(0.0, 0.0, 0.3);
        glUseProgram(p);
        glUniformMatrix4fv(mat_id, 1, GL_FALSE, &mvp[0][0]);
        draw_buf(vb, 12 * 3);
        render_frame(w);
        glfwSwapBuffers(w);
        glfwPollEvents();
    } while (glfwGetKey(w, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(w) == 0);
}
