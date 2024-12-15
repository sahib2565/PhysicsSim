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
    glm::vec2 velocity;


    // Constructor
    Particle(float mass, const glm::vec2 position = glm::vec2(0.0f), const glm::vec2 velocity = glm::vec2(0.0f))
    : mass(mass), position(position), velocity(velocity), accelaration(0.0f), force(0.0f) {}

    // Apply force to Particle
    // Useful to simulate gravity

    void ApplyForce(const glm::vec2 forceToApply) {
        this->force += forceToApply;
    }

    // Update particle state

    void update(float deltaTime){
        if (mass > 0.0f){
            // compute accelaration
            // accelaration = force / mass;
            accelaration = glm::vec2(0.0f);
        } else {
            accelaration = glm::vec2(0.0f);
        }
    

        // update velocity v = v0 + a * dt
        velocity += accelaration * deltaTime;

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

    float getMass() const {
        return mass;
    }

    // Boundries function
    void constrain_to_bound(){
        position = glm::vec2(position.x,-0.95f);
        velocity = glm::vec2(0.0f,0.0f);
    }

    void hitBottomTop(){
        velocity = glm::vec2(velocity.x, -velocity.y);
    }
    void hitLeftRight() {
        velocity = glm::vec2(-velocity.x, velocity.y);
    }

    // Collision response

    void CollsionResponse(float m2, glm::vec2 v2, glm::vec2 p2, float deltaTime){
        glm::vec2 deltaV = velocity - v2;
        glm::vec2 deltaX = position - p2;

        // Dot product and squared norm

        float  dotProduct = glm::dot(deltaV,deltaX);
        float normSquared = glm::dot(deltaX, deltaX);

        // scalar multiplier

        float scalar = (2.0f * m2 / (m2 + mass)) * (dotProduct / normSquared);

        // resulting vector
        velocity = glm::vec2(velocity - scalar * deltaX);

        position += velocity * deltaTime;
    }
};

#endif