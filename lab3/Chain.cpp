//
// Created by HANA on 2.1.2025..
//

#include "Chain.h"

Chain::Chain(std::vector<glm::vec3> joints) {
    this->joints = joints;
}

void Chain::addJoint(glm::vec3 joint) {
    joints.push_back(joint);
}

void Chain::draw() {
    glBegin(GL_LINE_STRIP);
    glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
    for (auto j : joints) {
        glVertex3f(j.x, j.y, j.z);
    }

    glEnd();
}
