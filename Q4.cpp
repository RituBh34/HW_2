
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>

// QUESTION 4

// Constants
const double speed_of_sound = 343.0; // Speed of sound in air at 20°C (m/s)

// Function to calculate Mach number given speed and altitude
double calculateMachNumber(double speed, double altitude) {
    double speed_of_sound_at_altitude = speed_of_sound * sqrt(1.0 + 0.0000226 * altitude);
    return speed / speed_of_sound_at_altitude;
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "Mach Number vs Altitude", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 800, 0, 600, -1, 1);
    glMatrixMode(GL_MODELVIEW);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw X and Y axis
        glBegin(GL_LINES);
        glColor3f(1.0, 1.0, 1.0); // White color
        // X axis
        glVertex2f(100, 100);
        glVertex2f(700, 100);
        // Y axis
        glVertex2f(100, 100);
        glVertex2f(100, 500);
        glEnd();

        glBegin(GL_POINTS);
        glColor3f(0.0, 1.0, 0.0); // Red color

        // Plot Mach number vs altitude
        for (double altitude = 0; altitude <= 50000; altitude += 100) {
            double speed = 300.0; // Constant speed for simplicity (m/s)
            double mach = calculateMachNumber(speed, altitude);
            double x = 100 + altitude / 50;
            double y = 100 + 400 * mach;
            glVertex2f(x, y);
        }
        glEnd();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}