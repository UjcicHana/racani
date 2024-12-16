//
// Created by HANA on 21.10.2024..
//

#ifndef INC_3DMA_BSPLINE_H
#define INC_3DMA_BSPLINE_H

#include "ObjectLoader.h"
#include <vec3.hpp>
#include <vec4.hpp>
#include <geometric.hpp>
#include <mat4x4.hpp>
#include <gtx/string_cast.hpp>
#include <gtx/transform.hpp> // For glm::rotate
#include <gtx/quaternion.hpp> // For quaternion-based rotation
#include <cmath>


class BSpline {
public:
    vector<glm::vec4> controlPoints;
    vector<glm::vec3> splinePoints;
    vector<glm::vec3> tangents;

    BSpline() {}

    void load(const string& filename, int w, int h);
    void draw();
};


#endif //INC_3DMA_BSPLINE_H
