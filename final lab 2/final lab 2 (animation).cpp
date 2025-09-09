#include <windows.h> // for MS Windows
#include <GL/glut.h> // GLUT, include glu.h and gl.h
#ifdef _WIN32
#endif
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include <string>
#include <cmath>
#include <cstdio>

// --- Game constants ---
const int WIN_W = 480;
const int WIN_H = 640;
const float ROAD_W = 280.0f;
const int LANES = 3;
const float LANE_W = ROAD_W / LANES;
const float CAR_W = 40.0f;
const float CAR_H = 70.0f;
const float ENEMY_W = 40.0f;
const float ENEMY_H = 70.0f;

struct Enemy {
    int lane;
    float y;
    float speed;
    bool passed;
};

// --- Game state ---
float roadX = (WIN_W - ROAD_W) / 2.0f;
float playerX = roadX + LANE_W * 1 + LANE_W/2.0f;
float playerY = 100.0f;

std::vector<Enemy> enemies;
bool paused = false;
bool gameOver = false;
int score = 0;
int bestScore = 0;
float baseSpeed = 4.0f;
unsigned long ticks = 0;

// --- Utility ---
float laneCenterX(int lane) {
    return roadX + lane * LANE_W + LANE_W/2.0f;
}

void drawRect(float cx, float cy, float w, float h) {
    float x0 = cx - w/2.0f;
    float y0 = cy - h/2.0f;
    float x1 = cx + w/2.0f;
    float y1 = cy + h/2.0f;
    glBegin(GL_QUADS);
      glVertex2f(x0,y0); glVertex2f(x1,y0); glVertex2f(x1,y1); glVertex2f(x0,y1);
    glEnd();
}

void drawCar(float cx, float cy, float w, float h) {
    drawRect(cx, cy, w, h);
}

bool aabbOverlap(float ax, float ay, float aw, float ah, float bx, float by, float bw, float bh) {
    return (ax - aw/2.0f < bx + bw/2.0f && ax + aw/2.0f > bx - bw/2.0f &&
            ay - ah/2.0f < by + bh/2.0f && ay + ah/2.0f > by - bh/2.0f);
}

void spawnEnemy() {
    Enemy e;
    e.lane = rand() % LANES;
    e.y = WIN_H + ENEMY_H;
    float difficulty = ticks / 3000.0f;
    if (difficulty > 2.0f) difficulty = 2.0f;
    float diff = 1.0f + difficulty;
    e.speed = baseSpeed * diff + (rand()%100)/120.0f;
    e.passed = false;
    enemies.push_back(e);
}

void resetGame() {
    enemies.clear();
    playerX = laneCenterX(1);
    playerY = 100.0f;
    score = 0;
    paused = false;
    gameOver = false;
    ticks = 0;
    baseSpeed = 4.0f;
}

void drawHUD() {
    glColor3f(1.0f, 1.0f, 1.0f);
    char buf[128];
    std::sprintf(buf, "Score: %d   Best: %d", score, bestScore);
    glRasterPos2f(10.0f, WIN_H - 20.0f);
    const char *p = buf;
    while (*p) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *p++);
    }

    if (paused) {
        const char *ps = "PAUSED - press 'p' to resume";
        glRasterPos2f(WIN_W/2.0f - 90.0f, WIN_H/2.0f + 10.0f);
        p = ps;
        while (*p) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *p++);
    }

    if (gameOver) {
        const char *gs = "GAME OVER - press 'r' to restart";
        glRasterPos2f(WIN_W/2.0f - 110.0f, WIN_H/2.0f - 10.0f);
        p = gs;
        while (*p) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *p++);
    }
}

// --- Rendering & update ---
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Background (grass)
    glColor3f(0.2f,0.6f,0.2f);
    drawRect(WIN_W/2.0f, WIN_H/2.0f, (float)WIN_W, (float)WIN_H);

    // Road
    glColor3f(0.08f,0.08f,0.08f);
    drawRect(WIN_W/2.0f, WIN_H/2.0f, ROAD_W, WIN_H);

    // Lane markers (simulate movement)
    glColor3f(1.0f,1.0f,0.6f);
    float dashH = 30.0f;
    float gap = 30.0f;
    float offset = std::fmod((float)ticks * 5.0f, dashH + gap);
    int lane;
    for (lane = 1; lane < LANES; ++lane) {
        float x = roadX + lane * LANE_W;
        float y;
        for (y = -dashH; y < WIN_H + dashH; y += dashH + gap) {
            drawRect(x, y + offset, 6.0f, dashH);
        }
    }

    // Player car
    glColor3f(0.2f,0.7f,1.0f);
    drawCar(playerX, playerY, CAR_W, CAR_H);

    // Enemies
    glColor3f(1.0f,0.3f,0.3f);
    size_t i;
    for (i = 0; i < enemies.size(); ++i) {
        drawCar(laneCenterX(enemies[i].lane), enemies[i].y, ENEMY_W, ENEMY_H);
    }

    drawHUD();
    glutSwapBuffers();
}

void updateGameState() {
    if (paused || gameOver) return;

    ++ticks;

    if (ticks % 40 == 0) {
        spawnEnemy();
    }

    // Update enemies and scoring
    size_t i;
    for (i = 0; i < enemies.size(); ++i) {
        enemies[i].y -= enemies[i].speed;
        if (!enemies[i].passed && enemies[i].y + ENEMY_H/2.0f < playerY - CAR_H/2.0f) {
            enemies[i].passed = true;
            score += 1;
            if (score > bestScore) bestScore = score;
        }
    }

    // Remove off-screen enemies (build a new vector)
    std::vector<Enemy> kept;
    kept.reserve(enemies.size());
    for (i = 0; i < enemies.size(); ++i) {
        if (enemies[i].y >= -ENEMY_H) kept.push_back(enemies[i]);
    }
    enemies.swap(kept);

    // Collision detection
    for (i = 0; i < enemies.size(); ++i) {
        if (aabbOverlap(playerX, playerY, CAR_W * 0.9f, CAR_H * 0.9f,
                        laneCenterX(enemies[i].lane), enemies[i].y, ENEMY_W * 0.9f, ENEMY_H * 0.9f)) {
            gameOver = true;
            break;
        }
    }

    // Gradually increase difficulty
    if (ticks % 300 == 0) baseSpeed += 0.2f;
}

void timerFunc(int value) {
    updateGameState();
    glutTimerFunc(16, timerFunc, 0); // ~60 FPS update tick
}

void idle() {
    // Keep display updating smoothly
    glutPostRedisplay();
}

// --- Input ---
void specialKeys(int key, int x, int y) {
    if (gameOver || paused) return;
    int currentLane = (int)((playerX - roadX) / LANE_W + 0.001f);
    if (currentLane < 0) currentLane = 0;
    if (currentLane >= LANES) currentLane = LANES-1;

    if (key == GLUT_KEY_LEFT) {
        currentLane = currentLane - 1;
        if (currentLane < 0) currentLane = 0;
        playerX = laneCenterX(currentLane);
    } else if (key == GLUT_KEY_RIGHT) {
        currentLane = currentLane + 1;
        if (currentLane >= LANES) currentLane = LANES-1;
        playerX = laneCenterX(currentLane);
    }
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 27: std::exit(0); break; // ESC
        case 'p': case 'P': paused = !paused; break;
        case 'r': case 'R': resetGame(); break;
    }
}

// --- Window & GL setup ---
void initGL() {
    glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
}

void reshape(GLsizei width, GLsizei height) {
    if (height == 0) height = 1;
    GLfloat aspect = (GLfloat)width / (GLfloat)height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Keep fixed world coords similar to original code
    gluOrtho2D(0.0, WIN_W, 0.0, WIN_H);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// --- Main ---
int main(int argc, char** argv) {
    srand((unsigned)time(NULL));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WIN_W, WIN_H);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Simple Car Racing - GLUT (C++98)");

    initGL();
    resetGame();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutIdleFunc(idle);
    glutTimerFunc(0, timerFunc, 0);

    glutMainLoop();
    return 0;
}
