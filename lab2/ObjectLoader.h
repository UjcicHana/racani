//
// Created by HANA on 15.12.2024..
//

#ifndef RACANI_OBJECTLOADER_H
#define RACANI_OBJECTLOADER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <GL/glut.h>
#include <vec3.hpp>
#include <geometric.hpp>
#include <mat4x4.hpp>
#include <gtx/string_cast.hpp>

using namespace std;

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
};

struct Face {
    int vertexIndex[3];
};

class ObjectLoader {
public:
    vector<Vertex> vertices;
    vector<Face> faces;
    float minX, maxX, minY, maxY, minZ, maxZ;
    glm::vec3 center;

    ObjectLoader() : minX(0), maxX(0), minY(0), maxY(0), minZ(0), maxZ(0) {}

    void load(const string& filename);
    void normalize(float scale);
    void calculateNormals();
    void draw();
    void translate(glm::mat4x4 translation);
    vector<Vertex> getVertices();

};


#endif //RACANI_OBJECTLOADER_H
