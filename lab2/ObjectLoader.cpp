//
// Created by HANA on 15.12.2024..
//

#include "ObjectLoader.h"

void ObjectLoader::load(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening .obj file" << endl;
        exit(1);
    }

    string line;
    bool firstVertex = true;

    while (getline(file, line)) {
        if (line.substr(0, 2) == "v ") {
            istringstream s(line.substr(2));
            float x, y, z;
            s >> x >> y >> z;
            glm::vec3 v = {x, y, z};
            vertices.push_back(v);

            if (firstVertex) {
                minX = maxX = v.x;
                minY = maxY = v.y;
                minZ = maxZ = v.z;
                firstVertex = false;
            } else {
                if (v.x < minX) minX = v.x;
                if (v.x > maxX) maxX = v.x;
                if (v.y < minY) minY = v.y;
                if (v.y > maxY) maxY = v.y;
                if (v.z < minZ) minZ = v.z;
                if (v.z > maxZ) maxZ = v.z;
            }
        } else if (line.substr(0, 2) == "f ") {
            istringstream s(line.substr(2));
            Face f;
            s >> f.vertexIndex[0] >> f.vertexIndex[1] >> f.vertexIndex[2];
            faces.push_back(f);
        }
    }

    file.close();

    normalize(10.0f);
}

void ObjectLoader::normalize(float scale) {
    float centerX = (minX + maxX) / 2.0f;
    float centerY = (minY + maxY) / 2.0f;
    float centerZ = (minZ + maxZ) / 2.0f;

    float scaleX = maxX - minX;
    float scaleY = maxY - minY;
    float scaleZ = maxZ - minZ;
    float maxDimension = max(max(scaleX, scaleY), scaleZ);

    for (auto& v : vertices) {
        v.x = (v.x - centerX) * scale / maxDimension;
        v.y = (v.y - centerY) * scale / maxDimension;
        v.z = (v.z - centerZ) * scale / maxDimension;
    }

    center = {0.0f, 0.0f, 0.0f};
}

void ObjectLoader::draw() {
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    for (const Face& face : faces) {
        for (int j = 0; j < 3; j++) {
            const auto v = vertices[face.vertexIndex[j] - 1];
            glVertex3f(v.x, v.y, v.z);
        }
    }
    glEnd();
}

void ObjectLoader::translate(glm::mat4x4 transformMatrix) {
    for (auto& v : vertices) {
        glm::vec4 vertexVec(v.x, v.y, v.z, 1.0f);
        glm::vec4 transformedVec = transformMatrix * vertexVec;

        v.x = transformedVec.x;
        v.y = transformedVec.y;
        v.z = transformedVec.z;
    }
}
