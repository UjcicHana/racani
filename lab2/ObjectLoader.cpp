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
            Vertex v;
            s >> x >> y >> z;
            glm::vec3 pos = {x, y, z};
            v.position = pos;
            vertices.push_back(v);

            if (firstVertex) {
                minX = maxX = v.position.x;
                minY = maxY = v.position.y;
                minZ = maxZ = v.position.z;
                firstVertex = false;
            } else {
                if (v.position.x < minX) minX = v.position.x;
                if (v.position.x > maxX) maxX = v.position.x;
                if (v.position.y < minY) minY = v.position.y;
                if (v.position.y > maxY) maxY = v.position.y;
                if (v.position.z < minZ) minZ = v.position.z;
                if (v.position.z > maxZ) maxZ = v.position.z;
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
        v.position.x = (v.position.x - centerX) * scale / maxDimension;
        v.position.y = (v.position.y - centerY) * scale / maxDimension;
        v.position.z = (v.position.z - centerZ) * scale / maxDimension;
    }

    center = {0.0f, 0.0f, 0.0f};
}

void ObjectLoader::calculateNormals() {
    for (auto& vertex : vertices) {
        vertex.normal = glm::vec3(0.0f);
    }

    // Compute face normals and accumulate them to vertices
    for (const auto& face : faces) {
        // Get vertices of the face
        glm::vec3 v1 = vertices[face.vertexIndex[0]].position;
        glm::vec3 v2 = vertices[face.vertexIndex[1]].position;
        glm::vec3 v3 = vertices[face.vertexIndex[2]].position;

        // Calculate face normal
        glm::vec3 edge1 = v2 - v1;
        glm::vec3 edge2 = v3 - v1;
        glm::vec3 faceNormal = glm::normalize(glm::cross(edge1, edge2));

        // Accumulate face normal to each vertex of the face
        vertices[face.vertexIndex[0]].normal += faceNormal;
        vertices[face.vertexIndex[1]].normal += faceNormal;
        vertices[face.vertexIndex[2]].normal += faceNormal;
    }

    for (auto& vertex : vertices) {
        vertex.normal = glm::normalize(vertex.normal);
    }
}

void ObjectLoader::draw() {
    glColor4f(0.5f, 0.0f, 0.5f, 0.8f);
    glBegin(GL_TRIANGLES);
    for (const Face& face : faces) {
        for (int j = 0; j < 3; j++) {
            const auto v = vertices[face.vertexIndex[j] - 1];
            glVertex3f(v.position.x, v.position.y, v.position.z);
        }
    }
    glEnd();
}

void ObjectLoader::translate(glm::mat4x4 transformMatrix) {
    for (auto& v : vertices) {
        glm::vec4 vertexVec(v.position.x, v.position.y, v.position.z, 1.0f);
        glm::vec4 transformedVec = transformMatrix * vertexVec;

        v.position.x = transformedVec.x;
        v.position.y = transformedVec.y;
        v.position.z = transformedVec.z;
    }
}

vector<Vertex> ObjectLoader::getVertices() {
    return vertices;
}
