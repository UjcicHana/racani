//
// Created by HANA on 20.1.2025..
//

#include "CCDAlgorithm.h"
#include <cstdio>
#include <iostream>

void ccd::findNewAngles(Bone *end, glm::vec3 target, int iterations) {
    bool found = false;
    glm::vec3 final;
    float thresh = 0.01;
    printf("Find new angles started\n");

    while(!found && iterations--) {
        Bone * currentBone = end;
        while(currentBone->parent != NULL) {
            // compute vector to target and vector to last link
            glm::vec4 endPosition = currentBone->getEndPosition();
            glm::vec4 startPosition = currentBone->parent->getEndPosition();
            std::cout << "Link begin and end" << glm::to_string(startPosition)
            << " " << glm::to_string(endPosition) << std::endl;
            glm::vec3 toTarget = normalize(glm::vec3(target.x-startPosition.x, target.y-startPosition.y, target.z-startPosition.z));
            glm::vec3 toEnd = normalize(glm::vec3(endPosition.x-startPosition.x, endPosition.y-startPosition.y, endPosition.z-startPosition.z));
            std::cout << "Totarget and toend" << glm::to_string(toTarget)
                      << " " << glm::to_string(toEnd) << std::endl;
            float cosine = dot(toEnd, toTarget);
            std::cout << "Angle" << ANGLE(cosine) <<std::endl;
            if(cosine < 0.99) {
                glm::vec3 crossResult = cross(toEnd, toTarget);
                float angle = glm::angle(toTarget, toEnd);
                glm::quat rotation = angleAxis(angle, crossResult);
                rotation = normalize(rotation);
                glm::vec3 euler = glm::eulerAngles(rotation);
                glm::vec3 eulerDeg = glm::vec3(ANGLE(euler.x), ANGLE(euler.y), ANGLE(euler.z));
                currentBone->rotateMax(eulerDeg.x, eulerDeg.y, eulerDeg.z);
            }

            glm::vec3 temp = glm::vec3(end->getEndPosition());
            temp.x -= target.x;
            temp.y -= target.y;
            temp.z -= target.z;
            if(dot(temp, temp) < 0.001) {
                found = true;
                printf("target reached\n");
            }
            final = temp;
            currentBone = currentBone->parent;
        }
    }

    printf("Found new angles? %d\n", found);
    printf("Final target %f %f %f\n", final.x, final.y, final.z);

}