// simple_car_with_birds_and_clouds.cpp
#include <windows.h>
#include <GL/glut.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include <cstdio>

using namespace std;

// --- Window constants ---
const int WIN_W = 480;
const int WIN_H = 640;
const float ROAD_W = 200.0f;
const int LANES = 3;
const float LANE_W = ROAD_W / LANES;

// --- Car constants ---
const float CAR_W = 40.0f;
const float CAR_H = 70.0f;

// --- Structures ---
struct Cloud {
    float x, y, speed;
};

struct Bird {
    float x, y, speed;
};

// --- Game state ---
float roadX = (WIN_W - ROAD_W) / 2.0f;
float playerX;
float playerY = 100.0f;

vector<Cloud> clouds;
vector<Bird> birds;

unsigned long ticks = 0;

// --- Helpers for remove_if (no lambdas) ---
bool cloudOff(const Cloud &c) { return c.x < -80.0f; }
bool birdOff(const Bird &b)  { return b.x < -40.0f; }

// --- Utility functions ---
void drawRect(float cx, float cy, float w, float h) {
    float x0 = cx - w/2.0f;
    float y0 = cy - h/2.0f;
    float x1 = cx + w/2.0f;
    float y1 = cy + h/2.0f;
    glBegin(GL_QUADS);
        glVertex2f(x0,y0); glVertex2f(x1,y0);
        glVertex2f(x1,y1); glVertex2f(x0,y1);
    glEnd();
}

float laneCenterX(int lane) {
    return roadX + lane * LANE_W + LANE_W/2.0f;
}

void spawnCloud() {
    Cloud c;
    c.x = WIN_W + 60.0f;
    c.y = WIN_H - (rand() % 200 + 40);
    c.speed = 0.2f + (rand()%10)/50.0f; // slow
    clouds.push_back(c);
}

void spawnBird() {
    Bird b;
    b.x = WIN_W + 50.0f;
    b.y = WIN_H - (rand() % 250 + 90);
    b.speed = 0.8f + (rand()%10)/20.0f; // faster than clouds
    birds.push_back(b);
}

// --- Draw helpers ---
void drawCloud(float cx, float cy) {
    glColor3f(1.0f,1.0f,1.0f);
    drawRect(cx, cy, 60, 30);
    drawRect(cx-22, cy + 4, 40, 22);
    drawRect(cx+22, cy + 4, 40, 22);
}

void drawBird(float cx, float cy) {
    glColor3f(0.0f,0.0f,0.0f);
    glBegin(GL_LINES);
        glVertex2f(cx-10.0f, cy);
        glVertex2f(cx, cy+8.0f);
        glVertex2f(cx, cy+8.0f);
        glVertex2f(cx+10.0f, cy);
    glEnd();
}

void drawCar(float cx, float cy) {
    glColor3f(0.2f,0.7f,1.0f);
    drawRect(cx, cy, CAR_W, CAR_H);
}

// --- Display ---
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Background (grass)
    glColor3f(0.2f,0.6f,0.2f);
    drawRect(WIN_W/2.0f, WIN_H/2.0f, (float)WIN_W, (float)WIN_H);

    // Draw clouds
    for (size_t i = 0; i < clouds.size(); ++i) {
        drawCloud(clouds[i].x, clouds[i].y);
    }

    // Draw birds
    for (size_t i = 0; i < birds.size(); ++i) {
        drawBird(birds[i].x, birds[i].y);
    }

    // Road
    glColor3f(0.12f,0.12f,0.12f);
    drawRect(WIN_W/2.0f, WIN_H/2.0f, ROAD_W, WIN_H);

    // Lane markers (simple scrolling)
    glColor3f(1.0f,1.0f,0.6f);
    float dash = 30.0f;
    float gap  = 30.0f;
    float offset = (ticks % (int)(dash + gap));
    for (int lane = 1; lane < LANES; ++lane) {
        float x = roadX + lane * LANE_W;
        for (float y = -dash; y < WIN_H + dash; y += dash + gap) {
            drawRect(x, y + offset, 6.0f, dash);
        }
    }

    // Player car
    drawCar(playerX, playerY);

    glutSwapBuffers();
}

// --- Update (called by idle) ---
void update() {
    ticks++;

    // spawn occasionally
    if (ticks % 300 == 0) spawnCloud();
    if (ticks % 500 == 0) spawnBird();

    // update clouds
    for (size_t i = 0; i < clouds.size(); ++i) {
        clouds[i].x -= clouds[i].speed;
    }
    // remove off-screen clouds
    vector<Cloud>::iterator newc = remove_if(clouds.begin(), clouds.end(), cloudOff);
    clouds.erase(newc, clouds.end());

    // update birds
    for (size_t i = 0; i < birds.size(); ++i) {
        birds[i].x -= birds[i].speed;
    }
    // remove off-screen birds
    vector<Bird>::iterator newb = remove_if(birds.begin(), birds.end(), birdOff);
    birds.erase(newb, birds.end());

    glutPostRedisplay();
}

// --- Input ---
void specialKeys(int key, int, int) {
    int lane = (int)((playerX - roadX) / LANE_W + 0.5f);
    if (key == GLUT_KEY_LEFT) {
        lane = lane - 1;
        if (lane < 0) lane = 0;
    } else if (key == GLUT_KEY_RIGHT) {
        lane = lane + 1;
        if (lane >= LANES) lane = LANES - 1;
    }
    playerX = laneCenterX(lane);
}

// --- Setup ---
void initGL() {
    glClearColor(0.1f,0.1f,0.12f,1.0f);
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WIN_W, 0, WIN_H);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// --- Main ---
int main(int argc, char** argv) {
    srand((unsigned)time(NULL));
    playerX = laneCenterX(1);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WIN_W, WIN_H);
    glutCreateWindow("Simple Car Racing - Birds & Clouds (easy)");

    initGL();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(specialKeys);
    glutIdleFunc(update);

    glutMainLoop();
    return 0;
}
