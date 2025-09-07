#include <windows.h>
#include <GL/glut.h>
#include <cmath>

// Car body coordinates
GLfloat carBody[5][2] = {
    {-50, 0},    // bottom left
    {-50, 50},   // top left
    {60, 50},    // top right
    {90, 30},    // front slant
    {90, 0}      // bottom right
};

// Car wheels (center x, center y, radius)
GLfloat wheel1[3] = {-30, -20, 20};
GLfloat wheel2[3] = {50, -20, 20};

void drawCar() {
    // Draw car body with gradient
    glBegin(GL_POLYGON);
        glColor3f(0, 0, 1);    glVertex2fv(carBody[0]); // blue
        glColor3f(0, 1, 0);    glVertex2fv(carBody[1]); // green
        glColor3f(1, 0, 0);    glVertex2fv(carBody[2]); // red
        glColor3f(1, 0, 0);    glVertex2fv(carBody[3]); // red
        glColor3f(1, 0, 0);    glVertex2fv(carBody[4]); // red
    glEnd();

    // Draw wheels (bright green)
    glColor3f(0, 1, 0);
    for (int w = 0; w < 2; ++w) {
        GLfloat* wheel = (w == 0) ? wheel1 : wheel2;
        glBegin(GL_POLYGON);
        for (int i = 0; i < 100; ++i) {
            float theta = 2.0f * M_PI * i / 100.0f;
            glVertex2f(wheel[0] + wheel[2] * cos(theta), wheel[1] + wheel[2] * sin(theta));
        }
        glEnd();
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // --- Draw Original Car ---
    drawCar();

    // --- Draw Transformed Car ---
    glPushMatrix();
        glTranslatef(3, 4, 0);      // Translation
        glRotatef(175, 0, 0, 1);    // Rotation
        glScalef(3, 4, 1);          // Scaling
        drawCar();
    glPopMatrix();

    glutSwapBuffers();
}

void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);     // Black background
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-200, 200, -300, 300);         // Custom coordinate system
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 900);
    glutCreateWindow("Car Transformation");

    init();

    glutDisplayFunc(display);

    glutMainLoop();
    return 0;
}
