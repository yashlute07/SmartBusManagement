
#include <GL/freeglut.h>
#include <cmath>
#include <cstdlib>

float busX = -15.0f;
float speed = 0.08f;
float wheelAngle = 0.0f;
float roadOffset = 0.0f;
bool paused = false;

const float PI = 3.14159265f;

// Draw a cuboid
void drawCube(float x, float y, float z,
              float sx, float sy, float sz,
              float r, float g, float b) {

    glPushMatrix();

    glTranslatef(x, y, z);
    glScalef(sx, sy, sz);

    glColor3f(r, g, b);
    glutSolidCube(1.0);

    glPopMatrix();
}

// Draw a cylinder
void drawWheel(float x, float y, float z) {

    glPushMatrix();

    glTranslatef(x, y, z);
    glRotatef(90, 0, 1, 0);
    glRotatef(wheelAngle, 0, 0, 1);

    glColor3f(0.08f, 0.08f, 0.08f);

    GLUquadric* wheel = gluNewQuadric();

    gluCylinder(wheel, 0.55, 0.55, 0.25, 32, 32);

    gluDisk(wheel, 0, 0.55, 32, 1);

    glTranslatef(0, 0, 0.25);
    gluDisk(wheel, 0, 0.55, 32, 1);

    gluDeleteQuadric(wheel);

    glPopMatrix();
}

// Draw bus
void drawBus() {

    glPushMatrix();

    glTranslatef(busX, 0, 0);

    // Main bus body
    drawCube(0, 2.0, 0, 6.0, 2.5, 2.5,
             0.05f, 0.35f, 0.75f);

    // Bus roof
    drawCube(0, 3.3, 0, 5.8, 0.25, 2.4,
             0.9f, 0.9f, 0.9f);

    // Bus front
    drawCube(3.05, 2.0, 0, 0.15, 1.8, 2.3,
             0.02f, 0.2f, 0.5f);

    // Windows
    for (int i = 0; i < 5; i++) {

        float x = -2.1f + i * 1.05f;

        drawCube(x, 2.45, 1.27,
                 0.85, 0.8, 0.08,
                 0.4f, 0.8f, 1.0f);
    }

    // Headlights
    drawCube(3.15, 1.7, -0.8,
             0.1, 0.35, 0.4,
             1.0f, 1.0f, 0.2f);

    drawCube(3.15, 1.7, 0.8,
             0.1, 0.35, 0.4,
             1.0f, 1.0f, 0.2f);

    // Wheels
    drawWheel(-2.0, 0.7, -1.25);
    drawWheel(-2.0, 0.7, 1.25);
    drawWheel(2.0, 0.7, -1.25);
    drawWheel(2.0, 0.7, 1.25);

    glPopMatrix();
}

// Draw road
void drawRoad() {

    // Road surface
    glColor3f(0.18f, 0.18f, 0.18f);

    glBegin(GL_QUADS);

    glVertex3f(-100, 0, -5);
    glVertex3f(100, 0, -5);
    glVertex3f(100, 0, 5);
    glVertex3f(-100, 0, 5);

    glEnd();

    // Road lane markings
    glColor3f(1, 1, 1);

    for (int i = 0; i < 30; i++) {

        float x = -90 + i * 6 - roadOffset;

        glBegin(GL_QUADS);

        glVertex3f(x, 0.03, -0.08);
        glVertex3f(x + 2, 0.03, -0.08);
        glVertex3f(x + 2, 0.03, 0.08);
        glVertex3f(x, 0.03, 0.08);

        glEnd();
    }
}

// Draw tree
void drawTree(float x, float z) {

    glPushMatrix();

    glTranslatef(x, 0, z);

    // Tree trunk
    glColor3f(0.45f, 0.22f, 0.08f);

    glPushMatrix();
    glTranslatef(0, 1, 0);
    glScalef(0.3, 2, 0.3);
    glutSolidCube(1);
    glPopMatrix();

    // Tree leaves
    glColor3f(0.05f, 0.5f, 0.12f);

    glPushMatrix();
    glTranslatef(0, 2.5, 0);
    glutSolidSphere(1.0, 20, 20);
    glPopMatrix();

    glPopMatrix();
}

// Draw roadside trees
void drawTrees() {

    for (int i = 0; i < 20; i++) {

        float x = -80 + i * 9 - roadOffset;

        if (x < -100)
            x += 180;

        drawTree(x, -8);
        drawTree(x, 8);
    }
}

// Draw ground
void drawGround() {

    glColor3f(0.25f, 0.55f, 0.2f);

    glBegin(GL_QUADS);

    glVertex3f(-100, -0.1, -30);
    glVertex3f(100, -0.1, -30);
    glVertex3f(100, -0.1, 30);
    glVertex3f(-100, -0.1, 30);

    glEnd();
}

// Display function
void display() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(
        12, 9, 18,
        0, 1, 0,
        0, 1, 0
    );

    drawGround();
    drawRoad();
    drawTrees();
    drawBus();

    glutSwapBuffers();
}

// Animation update
void update(int value) {

    if (!paused) {

        busX += speed;

        wheelAngle -= 5.0f * (speed / 0.08f);

        roadOffset += speed;

        if (roadOffset >= 6.0f)
            roadOffset = 0;

        if (busX > 15.0f)
            busX = -15.0f;
    }

    glutPostRedisplay();

    glutTimerFunc(16, update, 0);
}

// Keyboard controls
void keyboard(unsigned char key, int x, int y) {

    switch (key) {

        case ' ':
            paused = !paused;
            break;

        case '+':
            speed += 0.02f;
            break;

        case '-':
            speed -= 0.02f;

            if (speed < 0.02f)
                speed = 0.02f;

            break;

        case 'r':
        case 'R':
            busX = -15.0f;
            roadOffset = 0;
            wheelAngle = 0;
            break;

        case 27:
            exit(0);
            break;
    }

    glutPostRedisplay();
}

// Window resize
void reshape(int w, int h) {

    if (h == 0)
        h = 1;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(
        45.0,
        (float)w / h,
        0.1,
        500.0
    );

    glMatrixMode(GL_MODELVIEW);
}

// Initialize OpenGL
void init() {

    glClearColor(0.6f, 0.82f, 0.95f, 1.0f);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat ambient[] = {0.5f, 0.5f, 0.5f, 1.0f};
    GLfloat diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat position[] = {5.0f, 10.0f, 5.0f, 1.0f};

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    glEnable(GL_COLOR_MATERIAL);

    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}

// Main function
int main(int argc, char** argv) {

    glutInit(&argc, argv);

    glutInitDisplayMode(
        GLUT_DOUBLE |
        GLUT_RGB |
        GLUT_DEPTH
    );

    glutInitWindowSize(1000, 650);
    glutInitWindowPosition(100, 100);

    glutCreateWindow("Smart Bus - 3D OpenGL Animation");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    glutTimerFunc(16, update, 0);

    glutMainLoop();

    return 0;
}
