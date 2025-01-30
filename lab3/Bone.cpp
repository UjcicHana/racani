//
// Created by HANA on 18.1.2025..
//

#include "Bone.h"
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
#include <cstdio>

Bone::Bone(float l) {
    length = l;
    M = glm::mat4(1.0f);
    parent = NULL;
    rotation = glm::vec3(0.0f);
    coordinates = glm::vec3(0.0f);
    constraint[0] = glm::vec3(-360.0f);
    constraint[1] = glm::vec3(360.0f);
}

Bone::Bone(const Bone &b) {
    length = b.length;
    M = glm::mat4(b.M);
    parent = NULL;
    rotation = glm::vec3(b.rotation);
    coordinates = glm::vec3(b.coordinates);
    constraint[0] = glm::vec3(b.constraint[0]);
    constraint[1] = glm::vec3(b.constraint[1]);

    for (std::vector< Bone* >::const_iterator it = b.bones.begin(); it != b.bones.end(); ++it) {
        add(new Bone(**it));
    }
}

Bone* Bone::add(Bone *b) {
    b->parent = this;
    bones.push_back(b);

    return b;
}

void Bone::remove(Bone *b) {
    b->parent = NULL;
    bones.erase(std::remove(bones.begin(), bones.end(), b), bones.end());
}

void Bone::detach() {
    parent->bones.erase(std::remove(bones.begin(), bones.end(), this), bones.end());
    parent = NULL;

}

glm::vec4 Bone::getEndPosition() {
    glm::mat4 transform = getTransform();
    return glm::translate(transform, glm::vec3(0.0f, 0.0f, length)) * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
}

glm::mat4 Bone::getTransform() {
    glm::mat4 P = glm::mat4(1.0f);

    if (parent != NULL) {
        P = glm::translate(P, glm::vec3(0.0f, 0.0f, parent->length));


        glm::vec3 rot = glm::radians(rotation);



        P = glm::rotate(P, rot.x, glm::vec3(P*glm::vec4(1.0f, 0.0f, 0.0f, 0.0f)));
        P = glm::rotate(P, rot.y, glm::vec3(P*glm::vec4(0.0f, 1.0f, 0.0f, 0.0f)));
        P = glm::rotate(P, rot.z, glm::vec3(P*glm::vec4(0.0f, 0.0f, 1.0f, 0.0f)));

        P = parent->getTransform() * P;
        //std::cout << "Apply parent rotations" << glm::to_string(P) << std::endl;
    } else {
        P = glm::rotate(P, 90.0f, glm::vec3(-1.0f, 0.0f, 0.0f));
        P = glm::translate(P, coordinates);
        //std::cout << "Apply rotations in else" << glm::to_string(P) << std::endl;
    }

    return P;
}

Bone* Bone::constraints(float min_x, float max_x, float min_y, float max_y, float min_z, float max_z) {
    constraint[0] = glm::vec3(min_x, min_y, min_z);
    constraint[1] = glm::vec3(max_x, max_y, max_z);

    return this;
}

void Bone::checkConstraints(float x, float y, float z) {
    if (x < constraint[0].x || x > constraint[1].x ||
    y < constraint[0].y || y > constraint[1].y ||
    z < constraint[0].z || z > constraint[1].z) {
        printf("%f, %f, %f\n(%f,%f), (%f,%f), (%f,%f)\n",x,y,z,constraint[0].x,constraint[1].x,constraint[0].y,constraint[1].y,constraint[0].z,constraint[1].z);
        throw new ConstraintException();
    }
}

Bone* Bone::setRotate(float x, float y, float z) {
    float rx = fmod(x, 360.0f),
          ry = fmod(y, 360.0f),
          rz = fmod(z, 360.0f);

    checkConstraints(rx, ry, rz);

    rotation.x = rx;
    rotation.y = ry;
    rotation.z = rz;

    return this;
}

Bone *Bone::rotate(float dx, float dy, float dz) {
    return setRotate(rotation.x + dx, rotation.y + dy, rotation.z + dz);

}

Bone *Bone::rotateMax(float dx, float dy, float dz) {
    if(dx+rotation.x > 180) dx -= 360;
    if(dy+rotation.y > 180) dy -= 360;
    if(dz+rotation.z > 180) dz -= 360;
    if(dx+rotation.x < -180) dx += 360;
    if(dy+rotation.y < -180) dy += 360;
    if(dz+rotation.z < -180) dz += 360;
    if(dx+rotation.x > constraint[1].x) dx = constraint[1].x-rotation.x - 0.1f;
    if(dx+rotation.x < constraint[0].x) dx = constraint[0].x-rotation.x+0.1f;
    if(dy+rotation.y > constraint[1].y) dy = constraint[1].y-rotation.y-0.1f;
    if(dy+rotation.y < constraint[0].y) dy = constraint[0].y-rotation.y+0.1f;
    if(dz+rotation.z > constraint[1].z) dz = constraint[1].z-rotation.z-0.1f;
    if(dz+rotation.z < constraint[0].z) dz = constraint[0].z-rotation.z+0.1f;
    rotate(dx,dy,dz);
    return this;
}

void Bone::print() {
    std::cout << "Bone: " << length << " " << glm::to_string(rotation) << std::endl;
    for (std::vector<Bone*>::iterator it = bones.begin(); it != bones.end(); it++) {
        (**it).print();
    }
}

Bone::~Bone() {
    for (std::vector<Bone*>::iterator it = bones.begin(); it != bones.end(); it++) {
        delete *it;
    }
}

Bone* Bone::getEndEffector() {
    Bone* endEffector = this;

    if (!endEffector->bones.empty()) {
        endEffector = bones.front()->getEndEffector();
    }



    return endEffector;
}

void Bone::setCoordinates(glm::vec3 translation) {
    coordinates = translation;
}

