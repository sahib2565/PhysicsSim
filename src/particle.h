#ifndef PARTICLE_H
#define PARTICLE_H

#include <glm/glm.hpp>

class Particle {
    public:
    // Attributes of a particle
    float mass;
    glm::vec2 position;
    glm::vec2 accelaration;
    glm::vec2 force;
    glmn::vec2 velocity;


    // Constructor
    Particle(float mass, const glm::vec2 position = glm::vec2(0.0f), const glm::vec2 velocity = glm::vec2(0.0f))
    : mass(mass), position(position), velocity(velocity), accelaration(accelaration(0.0f)), force(force(0.0f)) {}

    // Apply force to Particle
    // Useful to simulate gravity

    void ApplyForce(const glm::vec2 forceToApply) {
        this->force += forceToApply;
    }

    // Update particle state

    void update(float deltaTime){
        if (mass > 0.0f){
            // compute accelaration
            accelaration = force / mass;
        } else {
            acceleration = glm::vec2(0.0f);
        }
    

        // update velocity v = v0 + a * dt

        velocity += acceleration * deltaTime;

        // update position: p = = p0 + v * dt

        position += velocity * deltaTime;

        // reset accumulated force for next frame

        force = glm::vec2(0.0f);
    }

    // Getter functions 

    glm::vec2 getPosition() const {
        return position;
    }

    glm::vec2 getVelocity() const {
        return velocity;
    }

    glm::vec2 getAcceleration() const {
        return accelaration;
    }
}

#endif