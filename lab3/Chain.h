//
// Created by HANA on 2.1.2025..
//

#ifndef RACANI_CHAIN_H
#define RACANI_CHAIN_H

#include <GL/glut.h>
#include <glm/vec3.hpp>
#include <vector>


class Chain {
private:
    std::vector<glm::vec3> joints;
public:
    Chain(std::vector<glm::vec3> joints);
    void addJoint(glm::vec3 joint);
    void draw();


};


#endif //RACANI_CHAIN_H
