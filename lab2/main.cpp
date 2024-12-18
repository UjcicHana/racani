#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <GL/glut.h>

#include "Particle.h"
#include "ObjectLoader.h"


std::vector<ParticleGenerator*> particleGenerators;
//ParticleGenerator* particleGenerator;
float delta = 0.0016f;
float passedTime = 0.0f;
ObjectLoader obj;

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

bool checkParticlesInside(ParticleGenerator* generator, ObjectLoader& obj);

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

    //particleGenerator = new ParticleGenerator(100);

    obj.load(argv[1]);
    obj.normalize(0.1f);

    int i = 0;
    for (const auto& vertex : obj.getVertices()) {
        ParticleGenerator* generator = new ParticleGenerator(100, vertex.position, vertex.normal);
        particleGenerators.push_back(generator);
        i++;
        //if (i > 3) break;
    }

    lastTime = std::chrono::high_resolution_clock::now();

    glutMainLoop();

    //delete particleGenerator;

    for (auto generator : particleGenerators) {
        delete generator;
    }

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

    obj.draw();

    //particleGenerator->Render();
    for (auto generator : particleGenerators) {
        generator->Render();
    }
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
        case 'r': ociste.x = 0.0; ociste.y = 0.0, ociste.z = 2.0;
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
        //particleGenerator->Update(deltaTime.count());
        for (auto generator : particleGenerators) {
            generator->Update(deltaTime.count());
            /*if (checkParticlesInside(generator, obj)) {
                std::cout << "Particle inside object!" << std::endl;
            }*/
        }
    }

    glutPostRedisplay();
}

bool isParticleInside(const Particle& particle, const glm::vec3& vertex, const glm::vec3& normal) {
    // Compute the vector from the vertex to the particle position
    glm::vec3 toParticle = particle.position - vertex;

    // Dot product between the vector to the particle and the vertex normal
    float dotProduct = glm::dot(toParticle, normal);

    // If dotProduct < 0, the particle is inside or moving towards the object
    return dotProduct < 0.0f;
}

bool checkParticlesInside(ParticleGenerator* generator, ObjectLoader& obj) {
    for (int i = 0; i < generator->getParticleCount(); ++i) {
        const Particle& particle = generator->getParticle(i);

        // Loop through all vertices of the object
        for (size_t j = 0; j < obj.vertices.size(); ++j) {
            const glm::vec3& vertex = obj.vertices[j].position;
            const glm::vec3& normal = obj.vertices[j].normal;

            if (isParticleInside(particle, vertex, normal)) {
                return true; // A particle is inside
            }
        }
    }
    return false; // No particle is inside
}
