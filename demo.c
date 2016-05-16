// test to make sure the loader works
#include <GL/glut.h>
#include "threedee.h"

void init(void)
{
   glClearColor(0.0, 0.0, 0.2, 0.0);
   glShadeModel(GL_SMOOTH);
   glViewport(0,0,800,600);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(45.0f,(GLfloat)800/(GLfloat)600,1.0f,1000.0f);
   glEnable(GL_DEPTH_TEST);
   glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
}

obj_t *o;
int y = 0;

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -50);
    glRotatef(0, 1.0, 0.0, 0.0);
    glRotatef(y++, 0.0, 1.0, 0.0);
    glRotatef(0, 0.0, 0.0, 1.0);
    glBegin(GL_TRIANGLES);
#define SCALE 20
    for (int i = 0; i < o->vlen; i++)
    {
        glColor3f(1.0,0.0,0.0);
        glVertex3f(
                o->vertex[o->polygon[i].a].x * SCALE,
                o->vertex[o->polygon[i].a].y * SCALE,
                o->vertex[o->polygon[i].a].z * SCALE
                );
        glColor3f(0.0,1.0,0.0);
        glVertex3f(
                o->vertex[o->polygon[i].b].x * SCALE,
                o->vertex[o->polygon[i].b].y * SCALE,
                o->vertex[o->polygon[i].b].z * SCALE
                );
        glColor3f(0.0,0.0,1.0);
        glVertex3f(
                o->vertex[o->polygon[i].c].x * SCALE,
                o->vertex[o->polygon[i].c].y * SCALE,
                o->vertex[o->polygon[i].c].z * SCALE
                );
    }
    glEnd();
    glFlush();
    glutSwapBuffers();
}


int main(int argc, char **argv) {
    obj_t **objs = load_3ds_objs("../../spec/gun.3ds");
    o = objs[0];
    printf("%s\n", o->name);
    for(int i = 0; i < o->vlen; i++)
       printf("%f %f %f\n", o->vertex[i].x, o->vertex[i].y, o->vertex[i].z);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800,600);
    glutInitWindowPosition(0,0);
    glutCreateWindow("display");
    glutDisplayFunc(display);
    glutIdleFunc(display);
    init();
    glutMainLoop();
    return 0;
}
