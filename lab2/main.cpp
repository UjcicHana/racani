#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

#include "Particle.h"


ParticleGenerator* particleGenerator1;
ParticleGenerator* particleGenerator2;
float delta = 0.00016f;
float passedTime = 0.0f;

GLuint width = 600, height = 600;

typedef struct _Ociste {
    GLdouble	x;
    GLdouble	y;
    GLdouble	z;
} Ociste;

Ociste	ociste = { 0.0f, 0.0f, 3.0f };

std::chrono::high_resolution_clock::time_point lastTime;

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

    particleGenerator1 = new ParticleGenerator(1000);
    particleGenerator2 = new ParticleGenerator(1000);
    particleGenerator2->UpdateSource(glm::vec3(1.0, 0.0, 0.0));

    lastTime = std::chrono::high_resolution_clock::now();

    glutMainLoop();

    delete particleGenerator1, particleGenerator1;
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
    particleGenerator1->Render();
    particleGenerator2->Render();
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
    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    passedTime += deltaTime.count();
    if (passedTime >= delta) {
        passedTime = 0.0f;

        if (particleGenerator1) {
            particleGenerator1->Update(deltaTime.count());
        }
        if (particleGenerator2) {
            particleGenerator2->Update(deltaTime.count());
        }
    }

    glutPostRedisplay();
}