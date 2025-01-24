//
// Created by HANA on 20.1.2025..
//

#ifndef RACANI_CCDALGORITHM_H
#define RACANI_CCDALGORITHM_H

#include "Bone.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/vector_angle.hpp>

namespace ccd {
    void findNewAngles(Bone *end, glm::vec3 target, int iterations=1000);
}


#endif //RACANI_CCDALGORITHM_H
