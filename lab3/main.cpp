//
// Created by HANA on 2.1.2025..
//


#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <time.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Chain.h"
#include "Bone.h"
#include "CCDAlgorithm.h"

#define WIDTH 600
#define HEIGHT 600

glm::vec3 target = glm::vec3(0.0, 4.0, 0.0);
float speed_x = 0;
float speed_y = 0;
int lastTime = 0;
float angle_x = -40;
float angle_y = 0;
float zoom = 40.0f;
float cdx, cdy;

GLfloat pod_color[] = { 0.0, 0.0, 0.0, 0.8 };
GLfloat bone_color[] = { 0.0, 0.0, 1.0, 1.0 };
GLfloat joint_color[] = {1.0, 0.0, 0.0, 1.0 };
GLfloat tip_color[] = { 1.0,1.0,0.0, 1.0 };
GLfloat ball_color[] = {255.0,140.5,25.0, 0.8 };

Bone* root;
Bone* root2;
bool isSetRoot2 = false;

Bone* loadChain(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return nullptr;
    }

    std::string line;
    Bone* rootToLoad;
    Bone* currentBone = nullptr;
    Bone* temp = nullptr;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string command;

        iss >> command;

        if (command == "b") {
            float length;
            iss >> length;
            rootToLoad = new Bone(length);
            currentBone = rootToLoad;
        } else if (command == "a") {
            float length;
            iss >> length;

            if (currentBone != nullptr) {
                temp = new Bone(length);
                currentBone->add(temp);
                currentBone = temp;
            } else {
                std::cerr << "Error: Add command before root is initialized." << std::endl;
            }
        } else if (command == "r") {
            float x, y, z;
            iss >> x >> y >> z;

            if (currentBone != nullptr) {
                currentBone->rotate(x, y, z);
            } else {
                std::cerr << "Error: Rotate command before root is initialized." << std::endl;
            }
        } else if (command == "t") {
            float x, y, z;
            iss >> x >> y >> z;

            if (rootToLoad != nullptr) {
                rootToLoad->setCoordinates(glm::vec3(x, y, z));
            } else {
                std::cerr << "Error: Translate command before root is initialized." << std::endl;
            }
        } else {
            std::cerr << "Error: Unknown command '" << command << "' in file." << std::endl;
        }
    }
    file.close();
    return rootToLoad;
}


void drawBones(Bone* b) {
    glPushMatrix();
    GLfloat s[] = {0};

    glm::mat4 previous = b->M;
    glm::vec3 rot = glm::radians(b->rotation);

    if (b->parent != NULL) {
        b->M = glm::translate(b->M, glm::vec3(0.0f, 0.0f, b->parent->length));

        b->M = glm::rotate(b->M, rot.x, glm::vec3(b->M * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f)));
        b->M = glm::rotate(b->M, rot.y, glm::vec3(b->M * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f)));
        b->M = glm::rotate(b->M, rot.z, glm::vec3(b->M * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f)));

        b->M = b->parent->M * b->M;
    } else {
        b->M = glm::rotate(b->M, rot.x, glm::vec3(1.0f, 0.0f, 0.0f));
        b->M = glm::rotate(b->M, rot.y, glm::vec3(0.0f, 1.0f, 0.0f));
        b->M = glm::rotate(b->M, rot.z, glm::vec3(0.0f, 0.0f, 1.0f));
        b->M = glm::translate(b->M, b->coordinates);
    }

    glLoadMatrixf(glm::value_ptr(b->M));

    if (b->parent != NULL) {
        s[0] = 250;
        glMaterialfv(GL_FRONT, GL_SPECULAR, joint_color);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, joint_color);
        glMaterialfv(GL_FRONT, GL_SHININESS, s);
        glutSolidSphere(0.1f, 32, 32);
    }

    glMaterialfv(GL_FRONT, GL_SPECULAR, bone_color);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, bone_color);
    s[0] = 11;
    glMaterialfv(GL_FRONT, GL_SHININESS, s);

    GLUquadricObj* q = gluNewQuadric();
    if (b->bones.empty())
        gluCylinder(q, 0.1f, 0.1f, b->length - 0.5f, 32, 32);
    else
        gluCylinder(q, 0.1f, 0.1f, b->length, 32, 32);
    gluDeleteQuadric(q);

    glPopMatrix();

    if (b->bones.empty()) {
        glPushMatrix();

        glLoadMatrixf(glm::value_ptr(glm::translate(b->M, glm::vec3(0.0f, 0.0f, b->length - 0.5f))));
        glMaterialfv(GL_FRONT, GL_SPECULAR, tip_color);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, tip_color);
        s[0] = 250;
        glMaterialfv(GL_FRONT, GL_SHININESS, s);

        gluCylinder(q, 0.2f, 0.0f, 0.5f, 32, 32);

        glPopMatrix();
    }

    for (std::vector<Bone*>::iterator it = b->bones.begin(); it != b->bones.end(); it++) {
        drawBones(*it);
    }

    b->M = previous;


}

void myDisplay()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::vec3 cameraEye = glm::vec3(0.0f, 5.0f, -zoom);
    glm::vec3 cameraCenter = glm::vec3(0.0f + cdx, 0.0f + cdy, 0.0);
    glm::vec3 cameraNose = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::mat4 V = glm::lookAt(cameraEye, cameraCenter, cameraNose);
    V = glm::rotate(V, angle_x, glm::vec3(1.0f, 0.0f, 0.0f));
    V = glm::rotate(V, angle_y, glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 P = glm::perspective(50.0f, 1.0f, 1.0f, 50.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(P));
    glMatrixMode(GL_MODELVIEW);


    glm::mat4 M = glm::mat4(1.0f);
    glLoadMatrixf(glm::value_ptr(V*M));

    GLfloat s[] = { 128 };
    glPushMatrix();
    glMaterialfv(GL_FRONT, GL_SPECULAR, ball_color);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, ball_color);
    glMaterialfv(GL_FRONT, GL_SHININESS, s);
    ball_color[3] = 0.5;
    glMaterialfv(GL_FRONT, GL_EMISSION, ball_color);
    ball_color[3] = 1.0;
    glLoadMatrixf(glm::value_ptr(glm::translate(M*V, target)));
    glutSolidSphere(0.1f, 32, 32);
    glPopMatrix();

    GLfloat t[] = { 0.0,0.0, 0.0,1.0 };
    glPushMatrix();
    glMaterialfv(GL_FRONT, GL_SPECULAR, pod_color);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, pod_color);
    s[0] = 0;
    glMaterialfv(GL_FRONT, GL_SHININESS, s);
    glMaterialfv(GL_FRONT, GL_EMISSION, t);
    glScalef(5.9f, 0.1f, 5.9f);
    glutSolidCube(1.0f);
    glPopMatrix();

    root->M = glm::rotate(M*V, -90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    root->setRotate(root->rotation.x, root->rotation.y, root->rotation.z);


    drawBones(root);

    if(isSetRoot2) {
        root2->M = glm::rotate(M*V, -90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        root2->setRotate(root2->rotation.x, root2->rotation.y, root2->rotation.z);
        drawBones(root2);
    }

    glutSwapBuffers();
}

void nextFrame(void) {
    int actTime = glutGet(GLUT_ELAPSED_TIME);
    float interval = actTime - lastTime;
    lastTime = actTime;
    angle_x += speed_x*interval / 700.0;
    angle_y += speed_y*interval / 700.0;

    if (angle_x>360) angle_x -= 360;
    if (angle_x>360) angle_x += 360;
    if (angle_y>360) angle_y -= 360;
    if (angle_y>360) angle_y += 360;

    glutPostRedisplay();
}

void myReshape(int width, int height)
{
    if (height == 0) height = 1;

    glViewport(0, 0, width, height);

    float aspect = static_cast<float>(width) / static_cast<float>(height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glm::mat4 P = glm::perspective(50.0f, aspect, 1.0f, 50.0f);
    glLoadMatrixf(glm::value_ptr(P));

    glMatrixMode(GL_MODELVIEW);
}

void keyDown(unsigned char c, int x, int y) {
    try {
        switch (c) {
            case '-':
                zoom += 0.5f;
                break;
            case '=':
            case '+':
                zoom -= 0.5f;
                break;
            case 'q':
                target.z -= 0.2f;
                break;
            case 'e':
                target.z += 0.2f;
                break;
            case 'w':
                target.y += 0.2;
                break;
            case 's':
                target.y -= 0.2;
                break;
            case 'a':
                target.x -= 0.2;
                break;
            case 'd':
                target.x += 0.2;
                break;
            case 13:
                printf("Target at: (%f, %f, %f)\n", target.x, target.y, target.z);
                ccd::findNewAngles(root->getEndEffector(), target);
                if (isSetRoot2) ccd::findNewAngles(root2->getEndEffector(), target);
                break;
        }
    }
    catch (ConstraintException* e) {
        printf("Cannot move further!\n");
    }
}

void specKeyDown(int c, int x, int y) {
    switch (c) {
        case GLUT_KEY_LEFT:
            cdx -= 0.25;
            break;
        case GLUT_KEY_RIGHT:
            cdx += 0.25;
            break;
        case GLUT_KEY_UP:
            cdy -= 0.25;
            break;
        case GLUT_KEY_DOWN:
            cdy += 0.25;
            break;
    }
}


int main(int argc, char **argv)
{
    if (argc < 2) {
        std::cerr << "Not enough arguments!" << std::endl;
        return 0;
    }
    std::string f1 = argv[1], f2;
    if (argc >= 3) {
        isSetRoot2 = true;
        f2 = argv[2];
    }


    srand(time(0));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(400, 50);
    glutCreateWindow("Inverse Kinematics - Mechanical Chain");
    glutDisplayFunc(myDisplay);
    glutReshapeFunc(myReshape);
    glutIdleFunc(nextFrame);

    glutSpecialFunc(specKeyDown);
    glutKeyboardFunc(keyDown);

    glShadeModel(GL_SMOOTH);

    GLfloat light_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
    GLfloat light_position[] = { 1.0, 0.0, 0.0, 1.0 };
    GLfloat light_diffuse[] = { 0.5, 0.5, 0.5, 1.0 };
    GLfloat light_specular[] = { 0.5, 0.5, 0.5, 1.0 };


    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);


    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    glEnable(GL_DEPTH_TEST);

    root = loadChain(f1);

    root->print();
    if (isSetRoot2) {
        root2 = loadChain(f2);
        root2->print();
    }


    glutMainLoop();

    delete root;

    return 0;
}