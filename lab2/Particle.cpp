//
// Created by HANA on 14.12.2024..
//

#include "Particle.h"

ParticleGenerator::ParticleGenerator(int N) : N(N), randomDist(0.0f, 1.0f) {
    particles = new Particle[N];
    source = glm::vec3(0.0f, 0.0f, 0.0f);
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
            p.color.r = 1 - p.age / p.oldest;
            p.color.b = p.age / p.oldest;
        }
    }
}

// placeholder, not needed
void ParticleGenerator::Render() {
    glPointSize(10.0);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_POINTS);

    glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < N; ++i) {
        const Particle& p = particles[i];
        glPointSize(10.0f);
        glColor4f(p.color.r, p.color.g, p.color.b, p.color.a);
        glVertex3f(p.position.x, p.position.y, p.position.z);
        //if (i == 0) std::cout << "Particle: " << p.age << "  " << glm::to_string(p.color) << std::endl;
    }

    glEnd();
}

void ParticleGenerator::RespawnParticle(Particle& particle) {
    particle.oldest = (0.5f + randomDist(randomEngine))*3.0f;
    particle.age = 0.0f;
    particle.position = source;
    particle.speed = glm::vec3(randomDist(randomEngine) * 2.0f - 1.0f, randomDist(randomEngine) * 2.0f - 1.0f, randomDist(randomEngine) * 2.0f - 1.0f);
    particle.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

    //std::cout << "Particle: " << particle.age << "  " << glm::to_string(particle.position) << std::endl;

}

