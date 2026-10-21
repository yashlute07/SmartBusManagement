#include <GL/freeglut.h>
#include <cmath>

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Bus body
    glColor3f(0.0f, 0.5f, 1.0f);
    glBegin(GL_QUADS);
        glVertex2f(-0.7f, -0.3f);
        glVertex2f(0.7f, -0.3f);
        glVertex2f(0.7f, 0.3f);
        glVertex2f(-0.7f, 0.3f);
    glEnd();

    // Windows
    glColor3f(0.8f, 0.9f, 1.0f);

    glBegin(GL_QUADS);

        glVertex2f(-0.55f, 0.0f);
        glVertex2f(-0.25f, 0.0f);
        glVertex2f(-0.25f, 0.2f);
        glVertex2f(-0.55f, 0.2f);

        glVertex2f(-0.15f, 0.0f);
        glVertex2f(0.15f, 0.0f);
        glVertex2f(0.15f, 0.2f);
        glVertex2f(-0.15f, 0.2f);

        glVertex2f(0.25f, 0.0f);
        glVertex2f(0.55f, 0.0f);
        glVertex2f(0.55f, 0.2f);
        glVertex2f(0.25f, 0.2f);

    glEnd();

    // Wheels
    glColor3f(0.1f, 0.1f, 0.1f);

    glBegin(GL_POLYGON);
        for (int i = 0; i < 360; i++) {
            float angle = i * 3.14159f / 180.0f;
            glVertex2f(-0.45f + 0.12f * cos(angle),
                       -0.35f + 0.12f * sin(angle));
        }
    glEnd();

    glBegin(GL_POLYGON);
        for (int i = 0; i < 360; i++) {
            float angle = i * 3.14159f / 180.0f;
            glVertex2f(0.45f + 0.12f * cos(angle),
                       -0.35f + 0.12f * sin(angle));
        }
    glEnd();

    glFlush();
}

void init() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
}

int main(int argc, char** argv) {

    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);

    glutCreateWindow("Smart Bus Visualization");

    init();

    glutDisplayFunc(display);

    glutMainLoop();

    return 0;
}