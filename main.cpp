#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <glm/glm.hpp>
#include <algorithm>
#include <limits>
#include "particle.h"  // Include the Particle class header
#include "BVH.h"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
// const float GRAVITY = -9.8f; // Gravity force
const float radius = 0.05f; // Radius of cirlce

// Global Particle instance
// Position of x and y can not reach threshold of 0.94

// Defining Particles
std::vector<Particle> particles = {
        Particle(1.0f, glm::vec2(0.0f, 0.0f), glm::vec2(0.1f, 0.1f)),
        Particle(1.0f, glm::vec2(0.83f, -0.65f), glm::vec2(-0.1f, -0.1f)),
        Particle(1.0f, glm::vec2(-0.55f, 0.55f), glm::vec2(0.2f, 0.2f)),
        Particle(1.0f, glm::vec2(0.3f, 0.3f), glm::vec2(-0.2f, -0.2f))
};
// Get distance

float distance(float x1, float x2, float y1, float y2){
    float dis = ((x2-x1) * (x2-x1)) + ((y2-y1) * (y2-y1));
    return sqrt(dis);
}

// OpenGL Render function
void render(std::vector<Particle> par) {
    glClear(GL_COLOR_BUFFER_BIT);
    for(Particle& p : par){
        // glClear(GL_COLOR_BUFFER_BIT);

        // Set the circle's color
        glColor3f(1.0f, 1.0f, 1.0f); // White

        // Begin drawing the circle
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(p.getPosition().x, p.getPosition().y); // Center of circle
        for (int i = 0; i <= 100; i++) {
            float angle = i * 2.0f * M_PI / 100;
            float x = p.getPosition().x + radius * cos(angle);
            float y = p.getPosition().y + radius * sin(angle);
            glVertex2f(x, y);
        }
        glEnd();

        //glfwSwapBuffers(glfwGetCurrentContext());
    }
    glfwSwapBuffers(glfwGetCurrentContext());
}

// Update and render simulation
void updateAndRender(std::vector<Particle> par, BVH bvh) {
    float deltaTime = 0.016f;  // Assuming 60fps, so 1/60 = 0.016s per frame

	const float minX = -0.94f, maxX = 0.94f;
    const float minY = -0.94f, maxY = 0.94f;
    bvh.updateParticles(particles, deltaTime);

    // Handle boundary collisions
    for (auto& particle : particles) {
        // glm::vec2 pos = particle.getPosition();
        glm::vec2 pos = particle.getPosition();
        glm::vec2 vel = particle.getVelocity();

        // Check and resolve collisions with vertical boundaries
        if (pos.x < minX) {
            pos.x = minX;
            particle.hitLeftRight(); // Reflect velocity
        } else if (pos.x > maxX) {
            pos.x = maxX;
            particle.hitLeftRight();
        }

            // Check and resolve collisions with horizontal boundaries
        if (pos.y < minY) {
            pos.y = minY;
            particle.hitBottomTop();
        } else if (pos.y > maxY) {
            pos.y = maxY;
            particle.hitBottomTop();
        }
        particle.position = pos;
    }

        // Detect and resolve collisions between particles
    for (size_t i = 0; i < particles.size(); ++i) {
        AABB queryBounds(
            particles[i].getPosition().x - 0.1f, particles[i].getPosition().y - 0.1f,
            particles[i].getPosition().x + 0.1f, particles[i].getPosition().y + 0.1f
        );

        std::vector<int> results = bvh.query(queryBounds);

        for (int j : results) {
            if (i != j) { // Avoid self-collision
                glm::vec2 p1 = particles[i].getPosition();
                glm::vec2 p2 = particles[j].getPosition();

                float distance = glm::length(p1 - p2);
                float minDistance = 0.1f; // Diameter of the particle (2 * radius)

                if (distance < minDistance) {
                        // Resolve collision using Particle's collision response
                    particles[i].CollsionResponse(
                        particles[j].getMass(),
                        particles[j].getVelocity(),
                        particles[j].getPosition(),
                        deltaTime
                    );

                    particles[j].CollsionResponse(
                        particles[i].getMass(),
                        particles[i].getVelocity(),
                        particles[i].getPosition(),
                        deltaTime
                    );
                }
            }
        }
    }
    // Render the updated particle
    render(par);
    
}

int main() {
    BVH bvh;
    bvh.build(particles);
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Create a windowed OpenGL window
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Particle Simulation", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the OpenGL context current
    glfwMakeContextCurrent(window);

    // Set the OpenGL viewport and 2D projection
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0); // Set the coordinate system

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Update and render simulation
        updateAndRender(particles, bvh);
        // updateAndRender(particle2);

        // Poll for events (e.g., window close)
        glfwPollEvents();
    }

    // Clean up and terminate
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

