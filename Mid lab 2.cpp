

#include <windows.h>
#include <GL/glut.h>
#include <math.h>

void display() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // White background
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0f, 0.0f, 0.0f);         // Black color

    // Body
    glBegin(GL_QUADS);
        glVertex2f(-0.4f, 0.0f);
        glVertex2f(0.4f, 0.0f);
        glVertex2f(0.4f, 0.3f);
        glVertex2f(-0.4f, 0.3f);
    glEnd();

    // Left leg
    glBegin(GL_QUADS);
        glVertex2f(-0.25f, -0.5f);
        glVertex2f(-0.15f, -0.5f);
        glVertex2f(-0.15f, 0.0f);
        glVertex2f(-0.25f, 0.0f);
    glEnd();

    // Right leg
    glBegin(GL_QUADS);
        glVertex2f(0.15f, -0.5f);
        glVertex2f(0.25f, -0.5f);
        glVertex2f(0.25f, 0.0f);
        glVertex2f(0.15f, 0.0f);
    glEnd();

    // Left ear (slanted manually)
    glBegin(GL_QUADS);
        glVertex2f(-0.6f, 0.45f);
        glVertex2f(-0.45f, 0.35f);
        glVertex2f(-0.50f, 0.25f);
        glVertex2f(-0.65f, 0.35f);
    glEnd();

    // Right ear (slanted manually)
    glBegin(GL_QUADS);
        glVertex2f(0.45f, 0.35f);
        glVertex2f(0.6f, 0.45f);
        glVertex2f(0.65f, 0.35f);
        glVertex2f(0.50f, 0.25f);
    glEnd();

    glFlush();
}

void init() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0); // 2D view
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(650, 650);
    glutInitWindowPosition(200, 200);
    glutCreateWindow("look like dog");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
