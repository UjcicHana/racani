#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

#include "Particle.h"


ParticleGenerator* particleGenerator;
float deltaTime = 0.016f;

GLuint width = 600, height = 600;
int kut = 0;

typedef struct _Ociste {
    GLdouble	x;
    GLdouble	y;
    GLdouble	z;
} Ociste;

Ociste	ociste = { 0.0f, 0.0f, 2.0f };

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
    printf("Tipka: a/d - pomicanje ocista po x os +/-\n");
    printf("Tipka: w/s - pomicanje ocista po y os +/-\n");
    printf("Tipka: r - pocetno stanje\n");
    printf("esc: izlaz iz programa\n");

    particleGenerator = new ParticleGenerator(10);

    lastTime = std::chrono::high_resolution_clock::now();

    glutMainLoop();

    delete particleGenerator;
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
    particleGenerator->Render();
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

    if (particleGenerator) {
        particleGenerator->Update(deltaTime.count());
    }

    glutPostRedisplay();
}


/**#include <GL/glut.h>
#include <iostream>

#include "Particle.h"

int width = 800, height = 600;

ParticleGenerator* particleGenerator;
float deltaTime = 0.016f; // Approx. 60 FPS

glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

void RenderParticles() {
    particleGenerator->Render();
}

void UpdateParticles(int value) {
    particleGenerator->Update(deltaTime);
    glutPostRedisplay();
    glutTimerFunc(16, UpdateParticles, 0); // Call this function again after 16ms
}

void InitializeOpenGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black background
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_POINT_SMOOTH);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z,
              cameraPos.x + cameraFront.x, cameraPos.y + cameraFront.y, cameraPos.z + cameraFront.z,
              cameraUp.x, cameraUp.y, cameraUp.z);
}

int main(int argc, char** argv) {
    particleGenerator = new ParticleGenerator(10); // 1000 particles

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Particle Engine");

    InitializeOpenGL();

    // Register callbacks
    glutDisplayFunc(RenderParticles);
    glutTimerFunc(16, UpdateParticles, 0);

    // Start the main loop
    glutMainLoop();

    // Clean up (this will never be reached because glutMainLoop is infinite)
    delete particleGenerator;

    return 0;
}

**/