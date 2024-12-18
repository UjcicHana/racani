//
// Created by HANA on 14.12.2024..
//

#ifndef INC_RACANI_PARTICLE_H
#define INC_RACANI_PARTICLE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <GL/glut.h>
#include <vec3.hpp>
#include <geometric.hpp>
#include <mat4x4.hpp>
#include <gtx/string_cast.hpp>
#include <random>
#include <chrono>

#include "ObjectLoader.h"

struct Particle {
    float age, oldest;
    glm::vec3 position;
    glm::vec3 speed;
    glm::vec4 color;

    Particle() : age(0.0f), position(0.0f), speed(0.0f), color(1.0f, 1.0f, 1.0f, 1.0f) {}
};

class ParticleGenerator {
public:
    ParticleGenerator(int N, const glm::vec3& source, const glm::vec3& normal);
    ~ParticleGenerator();

    void Update(float deltaTime);
    void Render();
    void RespawnParticle(Particle& particle);
    int getParticleCount();
    Particle& getParticle(int i);
    void setSource(glm::vec3 source);

private:
    int N;
    Particle* particles;
    std::default_random_engine randomEngine;
    std::uniform_real_distribution<float> randomDist;
    glm::vec3 source;
    glm::vec3 normal;
    float visibility = 0.8f;
    float pointSize = 5.0f;
};

#endif //INC_RACANI_PARTICLE_H
