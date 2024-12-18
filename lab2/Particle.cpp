//
// Created by HANA on 14.12.2024..
//

#include "Particle.h"

ParticleGenerator::ParticleGenerator(int N, const glm::vec3& source, const glm::vec3& normal)
        : N(N), source(source), normal(glm::normalize(normal)), randomDist(0.0f, 1.0f) {
    particles = new Particle[N];
    randomEngine.seed(static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()));
    for (int i = 0; i < N; ++i) {
        RespawnParticle(particles[i]);
    }
}

ParticleGenerator::~ParticleGenerator() {
    delete[] particles;
}

void ParticleGenerator::Update(float deltaTime) {
    //source.x += 0.0001f;
    for (int i = 0; i < N; ++i) {
        Particle& p = particles[i];
        p.age += deltaTime;
        if (p.oldest - p.age < 0.0f) {
            RespawnParticle(p);
        } else {
            p.position += p.speed * deltaTime;
            p.color.a = (1 - p.age / p.oldest) * visibility;
            //p.color.r = 1 - p.age / p.oldest;
            //p.color.b = p.age / p.oldest;
        }
    }
}

void ParticleGenerator::Render() {
    glPointSize(pointSize);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_POINTS);

    glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < N; ++i) {
        const Particle& p = particles[i];
        glColor4f(p.color.r, p.color.g, p.color.b, p.color.a);
        glVertex3f(p.position.x, p.position.y, p.position.z);
        //if (i == 0) std::cout << "Particle: " << p.age << "  " << glm::to_string(p.color) << std::endl;
    }

    glEnd();
}

void ParticleGenerator::RespawnParticle(Particle& particle) {
    particle.oldest = (0.5f + randomDist(randomEngine))*3.0f;
    particle.age = 0.0f;
    glm::vec3 randomDir(
            randomDist(randomEngine) * 2.0f - 1.0f,
            randomDist(randomEngine) * 2.0f - 1.0f,
            randomDist(randomEngine) * 2.0f - 1.0f
    );

    glm::vec3 outwardDir = glm::normalize(randomDir);
    particle.speed = glm::normalize(outwardDir + normal * 0.5f) * randomDist(randomEngine) * 2.0f;
    particle.position = source + outwardDir * 0.01f;
    particle.color = glm::vec4(0.0f, 0.0f, 0.0f, visibility);

    //std::cout << "Particle: " << particle.age << "  " << glm::to_string(particle.position) << std::endl;

}

int ParticleGenerator::getParticleCount() {
    return N;
}

Particle& ParticleGenerator::getParticle(int i) {
    return particles[i];
}

void ParticleGenerator::setSource(glm::vec3 source) {
    this->source = source;
}

