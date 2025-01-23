//
// Created by HANA on 18.1.2025..
//

#ifndef RACANI_BONE_H
#define RACANI_BONE_H

#include <vector>
#include <algorithm>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>


class ConstraintException : std::exception { };

class Bone {
public:
    Bone* parent;
    std::vector<Bone*> bones;
    float length;
    glm::mat4  M;
    glm::vec3 rotation;
    glm::vec3 coordinates;
private:
    glm::vec3 constraint[2];
public:
    Bone(float l);
    Bone(const Bone& b);
    Bone* add(Bone* b);
    void remove(Bone* b);
    void detach();
    Bone* constraints(float min_x, float max_x, float min_y, float max_y, float min_z, float max_z);
    void checkConstraints(float x, float y, float z);
    Bone* setRotate(float x, float y, float z);
    Bone* rotate(float dx, float dy, float dz);
    Bone* rotateMax(float dx, float dy, float dz);
    glm::vec4 getEndPosition();
    glm::mat4 getTransform();
    void print();
    Bone* getEndEffector();
    void setCoordinates(glm::vec3 translation);

    ~Bone();

};


#endif //RACANI_BONE_H
