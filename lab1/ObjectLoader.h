//
// Created by HANA on 21.10.2024..
//

#ifndef INC_3DMA_OBJECTLOADER_H
#define INC_3DMA_OBJECTLOADER_H

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

struct Face {
    int vertexIndex[3];
};

class ObjectLoader {
public:
    vector<glm::vec3> vertices;
    vector<Face> faces;
    float minX, maxX, minY, maxY, minZ, maxZ;
    glm::vec3 center;

    ObjectLoader() : minX(0), maxX(0), minY(0), maxY(0), minZ(0), maxZ(0) {}

    void load(const string& filename);
    void normalize(float scale);
    void draw();
    void translate(glm::mat4x4 translation);

};



#endif //INC_3DMA_OBJECTLOADER_H
