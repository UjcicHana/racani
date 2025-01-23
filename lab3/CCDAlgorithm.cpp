//
// Created by HANA on 20.1.2025..
//

#include "CCDAlgorithm.h"
#include <cstdio>
#include <iostream>

void ccd::findNewAngles(Bone *end, glm::vec3 target, int iterations) {
    bool found = false;
    glm::vec3 final;
    float threshold = 0.01f;
    //printf("Find new angles started\n");

    while (!found && iterations--) {
        Bone* currentBone = end;

        while (currentBone->parent != NULL) {
            glm::vec4 endPosition = end->getEndPosition();
            glm::vec4 startPosition = currentBone->parent->getEndPosition();

            glm::vec3 toTarget = glm::normalize(glm::vec3(target - glm::vec3(startPosition)));
            glm::vec3 toEnd = glm::normalize(glm::vec3(glm::vec3(endPosition) - glm::vec3(startPosition)));

            /*std::cout << "Bone start: " << glm::to_string(startPosition)
                      << " end: " << glm::to_string(endPosition) << std::endl;
            std::cout << "To target: " << glm::to_string(toTarget)
                      << " To end: " << glm::to_string(toEnd) << std::endl;*/

            float cosine = glm::dot(toEnd, toTarget);

            if (cosine < 0.999f) {
                glm::vec3 rotationAxis = glm::cross(toEnd, toTarget);

                if (glm::length(rotationAxis) > 0.0001f) {
                    rotationAxis = glm::normalize(rotationAxis);
                    float angle = acos(glm::clamp(cosine, -1.0f, 1.0f));

                    glm::quat rotation = glm::angleAxis(angle, rotationAxis);
                    glm::vec3 eulerAngles = glm::degrees(glm::eulerAngles(rotation));

                    currentBone->rotateMax(eulerAngles.x, eulerAngles.y, eulerAngles.z);
                }
            }

            glm::vec3 endEffectorPos = glm::vec3(end->getEndPosition());
            glm::vec3 diff = endEffectorPos - target;

            if (glm::dot(diff, diff) < threshold) {
                found = true;
                printf("Target reached at: (%f, %f, %f)\n", endEffectorPos.x, endEffectorPos.y, endEffectorPos.z);
                break;
            }
            currentBone = currentBone->parent;
        }
    }

    // Debug final position
    glm::vec3 endEffectorPos = glm::vec3(end->getEndPosition());
    printf("Final end effector position: (%f, %f, %f)\n", endEffectorPos.x, endEffectorPos.y, endEffectorPos.z);
    printf("Target position: (%f, %f, %f)\n", target.x, target.y, target.z);

}