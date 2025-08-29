#include <windows.h>
#include <GL/glut.h>
#include <math.h>

// Draw a filled pentagon
void drawPentagon(float cx, float cy, float r, float red, float green, float blue) {
    glColor3f(red, green, blue);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 5; ++i) {
        float angle = 2.0f * M_PI * i / 5.0f + M_PI/2.0f;
        glVertex2f(cx + r * cos(angle), cy + r * sin(angle));
    }
    glEnd();
}

// Draw a filled circle (approximated as a polygon)
void drawCircle(float cx, float cy, float r, float red, float green, float blue) {
    glColor3f(red, green, blue);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; ++i) {
        float angle = 2.0f * M_PI * i / 100.0f;
        glVertex2f(cx + r * cos(angle), cy + r * sin(angle));
    }
    glEnd();
}

void display() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw axes
    glLineWidth(6.0f);
    glColor3f(0, 0, 0);
    glBegin(GL_LINES);
        // X axis
        glVertex2f(-1, 0);
        glVertex2f(1, 0);
        // Y axis
        glVertex2f(0, -1);
        glVertex2f(0, 1);
    glEnd();

    // Draw yellow filled circle at center (origin)
    drawCircle(0.0f, 0.0f, 0.05f, 1.0f, 1.0f, 0.0f);

    // Draw red rectangle (upper left quadrant)
    glColor3f(1, 0, 0);
    glBegin(GL_QUADS);
        glVertex2f(-0.85f, 0.75f);
        glVertex2f(-0.55f, 0.75f);
        glVertex2f(-0.55f, 0.55f);
        glVertex2f(-0.85f, 0.55f);
    glEnd();

    // Draw sky blue rectangle (upper left quadrant, left of Y axis)
    glColor3f(0.6f, 0.85f, 1.0f); // light blue/cyan
    glBegin(GL_QUADS);
        glVertex2f(-0.1f, 0.4f);
        glVertex2f(-0.6f, 0.4f);
        glVertex2f(-0.6f, 0.12f);
        glVertex2f(-0.1f, 0.12f);
    glEnd();

    // Draw blue filled circle (upper right quadrant)
    drawCircle(0.5f, 0.5f, 0.04f, 0.0f, 0.6f, 1.0f);

    // Draw green triangle (bottom left quadrant)
    glColor3f(0, 0.8f, 0.2f);
    glBegin(GL_TRIANGLES);
        glVertex2f(-0.6f, -0.2f);
        glVertex2f(-0.6f, -0.5f);
        glVertex2f(-0.3f, -0.5f);
    glEnd();

    // Draw magenta pentagon (bottom right quadrant)
    drawPentagon(0.5f, -0.5f, 0.22f, 1.0f, 0.0f, 1.0f);

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutCreateWindow("Lab 1: Basic Primitives - Fixed");
    glutInitWindowSize(600, 600);
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
