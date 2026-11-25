
#include <GL/freeglut.h>
#include <iostream>
using namespace std;

const int LEFT = 1, RIGHT = 2, BOTTOM = 4, TOP = 8;

float xmin = -0.5f, ymin = -0.4f;
float xmax =  0.5f, ymax =  0.4f;

int computeCode(float x, float y) {
    int code = 0;

    if (x < xmin) code |= LEFT;
    else if (x > xmax) code |= RIGHT;

    if (y < ymin) code |= BOTTOM;
    else if (y > ymax) code |= TOP;

    return code;
}

void drawLine(float x1, float y1, float x2, float y2, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
}

void cohenSutherland(float x1, float y1, float x2, float y2) {
    int code1 = computeCode(x1, y1);
    int code2 = computeCode(x2, y2);

    bool accept = false;

    while (true) {
        if (!(code1 | code2)) {
            accept = true;
            break;
        }
        else if (code1 & code2) {
            break;
        }
        else {
            int codeOut = code1 ? code1 : code2;
            float x, y;

            if (codeOut & TOP) {
                x = x1 + (x2 - x1) * (ymax - y1) / (y2 - y1);
                y = ymax;
            }
            else if (codeOut & BOTTOM) {
                x = x1 + (x2 - x1) * (ymin - y1) / (y2 - y1);
                y = ymin;
            }
            else if (codeOut & RIGHT) {
                y = y1 + (y2 - y1) * (xmax - x1) / (x2 - x1);
                x = xmax;
            }
            else {
                y = y1 + (y2 - y1) * (xmin - x1) / (x2 - x1);
                x = xmin;
            }

            if (codeOut == code1) {
                x1 = x;
                y1 = y;
                code1 = computeCode(x1, y1);
            }
            else {
                x2 = x;
                y2 = y;
                code2 = computeCode(x2, y2);
            }
        }
    }

    if (accept) {
        drawLine(x1, y1, x2, y2, 0.0f, 0.7f, 0.0f);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Clipping Window
    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(xmin, ymin);
    glVertex2f(xmax, ymin);
    glVertex2f(xmax, ymax);
    glVertex2f(xmin, ymax);
    glEnd();

    // Original Lines
    drawLine(-0.8f, 0.7f, 0.8f, -0.7f, 1.0f, 0.0f, 0.0f);
    drawLine(-0.8f, 0.0f, 0.8f, 0.0f, 1.0f, 0.0f, 0.0f);
    drawLine(-0.8f, -0.7f, 0.8f, 0.7f, 1.0f, 0.0f, 0.0f);

    // Clipped Lines
    cohenSutherland(-0.8f, 0.7f, 0.8f, -0.7f);
    cohenSutherland(-0.8f, 0.0f, 0.8f, 0.0f);
    cohenSutherland(-0.8f, -0.7f, 0.8f, 0.7f);

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
    glutCreateWindow("CO5 - Cohen Sutherland Line Clipping");

    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
