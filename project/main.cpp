#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include "particle.h"  // Include the Particle class header

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
// const float GRAVITY = -9.8f; // Gravity force
const float radius = 0.05f; // Radius of cirlce

// Global Particle instance
Particle particle(1.0f, glm::vec2(0.0f, 0.5f), glm::vec2(-0.05f, -0.20f));

// OpenGL Render function
void render() {
	glClear(GL_COLOR_BUFFER_BIT);

    // Set the circle's color
    glColor3f(1.0f, 1.0f, 1.0f); // White

    // Begin drawing the circle
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(particle.getPosition().x, particle.getPosition().y); // Center of circle
    for (int i = 0; i <= 100; i++) {
        float angle = i * 2.0f * M_PI / 100;
        float x = particle.getPosition().x + radius * cos(angle);
        float y = particle.getPosition().y + radius * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();

    glfwSwapBuffers(glfwGetCurrentContext());
}

// Update and render simulation
void updateAndRender() {
    float deltaTime = 0.016f;  // Assuming 60fps, so 1/60 = 0.016s per frame

	// Giving constraint:
	/*

	if (particle.getPosition().y - radius > -1.0f){
		// Apply gravity force to the particle (pointing downwards)
		// particle.ApplyForce(glm::vec2(0.0f, GRAVITY * particle.mass));

		// Update particle's state
		particle.update(deltaTime);
	} else {
		particle.constrain_to_bound();
	}
	*/
	if(particle.getPosition().y - radius <= -1.0 || particle.getPosition().y + radius >= 1.0){
		particle.hitBottomTop();
		particle.update(deltaTime);
	}
	else if(particle.getPosition().x - radius <= -1.0 || particle.getPosition().x + radius >= 1.0){
		particle.hitLeftRight();
		particle.update(deltaTime);
	}
	else{
		particle.update(deltaTime);
	}

    // Render the updated particle
    render();
}

int main() {
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
        updateAndRender();

        // Poll for events (e.g., window close)
        glfwPollEvents();
    }

    // Clean up and terminate
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

