//
// Created by HANA on 2.1.2025..
//


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

#include "Chain.h"

Chain *chain;

std::vector<glm::vec3> joints = {glm::vec3(0.0f, 0.0f, 0.0f),
                                 glm::vec3(1.0f, 0.0f, 0.0f),
                                 glm::vec3(1.5f, 1.0f, 0.0f)};

GLuint width = 600, height = 600;

typedef struct _Ociste {
    GLdouble	x;
    GLdouble	y;
    GLdouble	z;
} Ociste;

Ociste	ociste = { 0.0f, 0.0f, 3.0f };

void myDisplay();
void myReshape(int width, int height);
void myRenderScene();
void idle();
void myKeyboard(unsigned char theKey, int mouseX, int mouseY);

int main(int argc, char **argv)
{

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Particle engine");
    glutDisplayFunc(myDisplay);
    glutReshapeFunc(myReshape);
    glutIdleFunc(idle);
    glutKeyboardFunc(myKeyboard);



    chain = new Chain(joints);

    glutMainLoop();

    return 0;
}

void myDisplay()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    myRenderScene();
    glutSwapBuffers();
}

void myReshape(int w, int h)
{
    width = w; height = h;
    //glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glOrtho(-1, 1, -1, 1, 1, 5);
    glFrustum(-1, 1, -1, 1, 1, 5);
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(ociste.x, ociste.y, ociste.z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);	// ociste x,y,z; glediste x,y,z; up vektor x,y,z
}

void myRenderScene()
{
    glPushMatrix();
    chain->draw();


    glPopMatrix();
}

void myKeyboard(unsigned char theKey, int mouseX, int mouseY)
{
    switch (theKey)
    {
        case 'a': ociste.x = ociste.x+0.2f;
            break;
        case 'd': ociste.x = ociste.x-0.2f;
            break;
        case 'w': ociste.y = ociste.y + 0.2f;
            break;
        case 's': ociste.y = ociste.y - 0.2f;
            break;
        case 'r': ociste.x = 0.0; ociste.y = 0.0, ociste.z = 0.0;
            break;
        case 'q': ociste.z = ociste.z+0.2f;
            break;
        case 'e': ociste.z = ociste.z-0.2f;
            break;
        case 27:  exit(0);
            break;
    }

    myReshape(width, height);
    glutPostRedisplay();
}

void idle() {

    glutPostRedisplay();
}