#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <GL/glut.h>

#include "ObjectLoader.h"
#include "BSpline.h"

using namespace std;

GLuint width = 600, height = 600;
int pos = 1;

// Use struct for ociste (viewpoint)
struct Ociste {
    float x, y, z;
    Ociste(float x = 0.0f, float y = -140.0f, float z = 2.0f) : x(x), y(y), z(z) {}
};

Ociste ociste;  // Initialize ociste

// Create an instance of the ObjLoader class
ObjectLoader obj;
BSpline spline;

void myDisplay();
void myReshape(int width, int height);
void myRenderScene();
void idle();
void myKeyboard(unsigned char theKey, int mouseX, int mouseY);
glm::mat4 createRotationMatrix(const glm::vec3& originalCenter, const glm::vec3& newCenter);

int main(int argc, char** argv) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <obj file>" << endl;
        return 1;
    }

    // Load the .obj file
    obj.load(argv[1]);
    spline.load(argv[2], width, height);

    glm::vec3 first = spline.splinePoints[0];
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), first);


    obj.translate(translationMatrix);

    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("B Spline");
    glutDisplayFunc(myDisplay);
    glutReshapeFunc(myReshape);
    glutIdleFunc(idle);
    glutKeyboardFunc(myKeyboard);

    printf("Tipka: a/d - pomicanje ocista po x os +/-\n");
    printf("Tipka: w/s - pomicanje ocista po y os +/-\n");
    printf("Tipka: q/e - pomicanje ocista po z os +/-\n");
    printf("Tipka: r - pocetno stanje\n");
    printf("esc: izlaz iz programa\n");

    glutMainLoop();
    return 0;
}

void myDisplay() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    myRenderScene();
    glutSwapBuffers();
}

void myReshape(int w, int h) {
    width = w;
    height = h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLdouble)width / (GLdouble)height, 1.0, 1000.0);
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(ociste.x, ociste.y, ociste.z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void myRenderScene() {
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -3.0f);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    spline.draw();
    obj.draw();
    glPopMatrix();
}

void myKeyboard(unsigned char theKey, int mouseX, int mouseY) {
    switch (theKey) {
        case 'a': ociste.x += 0.2f; break;
        case 'd': ociste.x -= 0.2f; break;
        case 'w': ociste.y += 0.2f; break;
        case 's': ociste.y -= 0.2f; break;
        case 'q': ociste.z += 0.2f; break;
        case 'e': ociste.z -= 0.2f; break;
        case 'r': ociste.x = 0.0; ociste.y = 0.0; break;
        case 27: exit(0);
    }

    // std::printf("%f %f %f", ociste.x, ociste.y, ociste.z);

    myReshape(width, height);
    glutPostRedisplay();
}

void idle() {
    if (pos < spline.splinePoints.size()) {
        static float timeFactor = 0.0f;
        timeFactor += 0.01f;

       glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), spline.splinePoints[pos] - spline.splinePoints[pos - 1]);

        obj.translate(translationMatrix);
        pos++;
    }

    glutPostRedisplay();
}

