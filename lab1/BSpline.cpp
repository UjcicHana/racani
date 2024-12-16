//
// Created by HANA on 21.10.2024..
//

#include "BSpline.h"

void BSpline::load(const string& filename, int w, int h) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file" << endl;
        exit(1);
    }

    string line;

    while (getline(file, line)) {
        if (line.substr(0, 2) == "v ") {
            istringstream s(line.substr(2));
            float x, y, z;
            s >> x >> y >> z;

            controlPoints.push_back(glm::vec4(x, y, z, 1));
        }
    }

    file.close();

    int numSegments = controlPoints.size() - 3;

    for (int i = 0; i < numSegments; i++) {
        glm::vec4 v0 = controlPoints[i];
        glm::vec4 v1 = controlPoints[i + 1];
        glm::vec4 v2 = controlPoints[i + 2];
        glm::vec4 v3 = controlPoints[i + 3];

        std::cout << to_string(v3) << std::endl;

        for (int t = 0; t < 100; t++) {

            double j = t / 100.0;
            float f1 = (-pow(j, 3.0) + 3 * pow(j, 2.0) - 3 * j + 1) / 6.0;
            float f2 = (3 * pow(j, 3.0) - 6 * pow(j, 2.0) + 4) / 6.0;
            float f3 = (-3 * pow(j, 3.0) + 3 * pow(j, 2.0) + 3 * j + 1) / 6.0;
            float f4 = pow(j, 3.0) / 6.0;

            float x0 = f1 * v0.x + f2 * v1.x + f3 * v2.x + f4 * v3.x;
            float y0 = f1 * v0.y + f2 * v1.y + f3 * v2.y + f4 * v3.y;
            float z0 = f1 * v0.z + f2 * v1.z + f3 * v2.z + f4 * v3.z;

            glm::vec3 p = {x0, y0, z0};

            splinePoints.push_back(p);

            float t1 = 0.5 * (-pow(j, 2.0) + 2 * j - 1);
            float t2 = 0.5 * (3 * pow(j, 2.0) - 4 * j);
            float t3 = 0.5 * (-3 * pow(j, 2.0) + 2 * j + 1);
            float t4 = 0.5 * (pow(j, 2.0));


            float x1 = t1 * v0.x + t2 * v1.x + t3 * v2.x + t4 * v3.x;
            float y1 = t1 * v0.y + t2 * v1.y + t3 * v2.y + t4 * v3.y;
            float z1 = t1 * v0.z + t2 * v1.z + t3 * v2.z + t4 * v3.z;

            glm::vec3 tangent = {x1, y1, z1};

            tangents.push_back(tangent);

        }
    }
}

void BSpline::draw() {

/*    glColor3f(1.0f, 0.0f, 0.0f);  // Red
    glPointSize(5.0f);
    glBegin(GL_POINTS);
    for (const auto& v : controlPoints) {
        glVertex3f(v.x, v.y, v.z);
    }
    glEnd(); */

    glColor3f(0.0f, 0.0f, 1.0f);  // Red
    glLineWidth(3.0f);
    glBegin(GL_LINE_STRIP);
    for (const auto& v : splinePoints) {
        glVertex3f(v.x, v.y, v.z);
    }
    glEnd();

    glColor3f(0.0f, 1.0f, 0.0f);  // Red
    glLineWidth(1.0f);
    glBegin(GL_LINES);
    for (int i = 0; i < splinePoints.size(); i = i + 10) {
        glVertex3f(splinePoints[i].x, splinePoints[i].y, splinePoints[i].z);
        glVertex3f(splinePoints[i].x + tangents[i].x,
                   splinePoints[i].y + tangents[i].y,
                   splinePoints[i].z + tangents[i].z);
    }
    glEnd();

}