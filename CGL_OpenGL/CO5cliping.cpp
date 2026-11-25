
#include <GL/freeglut.h>
#include <vector>
#include <algorithm>

using namespace std;

struct Point {
    float x, y;
};

const int LEFT = 1;
const int RIGHT = 2;
const int BOTTOM = 4;
const int TOP = 8;

float xmin = 150, ymin = 100;
float xmax = 650, ymax = 450;

// Calculate region code for Cohen-Sutherland
int computeCode(float x, float y) {

    int code = 0;

    if (x < xmin)
        code |= LEFT;
    else if (x > xmax)
        code |= RIGHT;

    if (y < ymin)
        code |= BOTTOM;
    else if (y > ymax)
        code |= TOP;

    return code;
}

// Cohen-Sutherland Line Clipping Algorithm
bool cohenSutherland(
    float &x1, float &y1,
    float &x2, float &y2
) {

    int code1 = computeCode(x1, y1);
    int code2 = computeCode(x2, y2);

    while (true) {

        if ((code1 | code2) == 0) {
            return true;
        }

        else if (code1 & code2) {
            return false;
        }

        else {

            int codeOut;
            float x, y;

            if (code1 != 0)
                codeOut = code1;
            else
                codeOut = code2;

            if (codeOut & TOP) {
                x = x1 + (x2 - x1) *
                    (ymax - y1) / (y2 - y1);
                y = ymax;
            }

            else if (codeOut & BOTTOM) {
                x = x1 + (x2 - x1) *
                    (ymin - y1) / (y2 - y1);
                y = ymin;
            }

            else if (codeOut & RIGHT) {
                y = y1 + (y2 - y1) *
                    (xmax - x1) / (x2 - x1);
                x = xmax;
            }

            else {
                y = y1 + (y2 - y1) *
                    (xmin - x1) / (x2 - x1);
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
}

// Check whether a polygon point is inside a boundary
bool inside(Point p, int edge) {

    if (edge == 0)
        return p.x >= xmin;

    if (edge == 1)
        return p.x <= xmax;

    if (edge == 2)
        return p.y >= ymin;

    return p.y <= ymax;
}

// Calculate intersection point
Point intersection(Point s, Point e, int edge) {

    Point p;

    float dx = e.x - s.x;
    float dy = e.y - s.y;

    if (edge == 0 || edge == 1) {

        float x = (edge == 0) ? xmin : xmax;

        p.x = x;
        p.y = s.y + dy * (x - s.x) / dx;
    }

    else {

        float y = (edge == 2) ? ymin : ymax;

        p.y = y;
        p.x = s.x + dx * (y - s.y) / dy;
    }

    return p;
}

// Sutherland-Hodgman Polygon Clipping Algorithm
vector<Point> polygonClipping(vector<Point> polygon) {

    for (int edge = 0; edge < 4; edge++) {

        vector<Point> output;

        if (polygon.empty())
            return output;

        Point s = polygon.back();

        for (Point e : polygon) {

            bool sInside = inside(s, edge);
            bool eInside = inside(e, edge);

            if (sInside && eInside) {
                output.push_back(e);
            }

            else if (sInside && !eInside) {
                output.push_back(intersection(s, e, edge));
            }

            else if (!sInside && eInside) {
                output.push_back(intersection(s, e, edge));
                output.push_back(e);
            }

            s = e;
        }

        polygon = output;
    }

    return polygon;
}

// Draw clipping window
void drawClippingWindow() {

    glColor3f(0.0, 0.3, 1.0);
    glLineWidth(3);

    glBegin(GL_LINE_LOOP);

    glVertex2f(xmin, ymin);
    glVertex2f(xmax, ymin);
    glVertex2f(xmax, ymax);
    glVertex2f(xmin, ymax);

    glEnd();
}

// Draw original and clipped lines
void drawLines() {

    float lines[5][4] = {
        {50, 200, 750, 350},
        {200, 50, 500, 550},
        {250, 200, 550, 300},
        {50, 500, 100, 550},
        {400, 50, 400, 550}
    };

    // Original lines in red
    glColor3f(1.0, 0.0, 0.0);
    glLineWidth(2);

    glBegin(GL_LINES);

    for (int i = 0; i < 5; i++) {
        glVertex2f(lines[i][0], lines[i][1]);
        glVertex2f(lines[i][2], lines[i][3]);
    }

    glEnd();

    // Clipped lines in green
    glColor3f(0.0, 0.8, 0.0);
    glLineWidth(4);

    glBegin(GL_LINES);

    for (int i = 0; i < 5; i++) {

        float x1 = lines[i][0];
        float y1 = lines[i][1];
        float x2 = lines[i][2];
        float y2 = lines[i][3];

        if (cohenSutherland(x1, y1, x2, y2)) {

            glVertex2f(x1, y1);
            glVertex2f(x2, y2);
        }
    }

    glEnd();
}

// Draw original and clipped polygon
void drawPolygon() {

    vector<Point> original = {
        {80, 160},
        {380, 40},
        {720, 180},
        {600, 520},
        {250, 500}
    };

    // Original polygon in red
    glColor3f(1.0, 0.0, 0.0);
    glLineWidth(2);

    glBegin(GL_LINE_LOOP);

    for (Point p : original)
        glVertex2f(p.x, p.y);

    glEnd();

    // Apply Sutherland-Hodgman clipping
    vector<Point> clipped = polygonClipping(original);

    // Clipped polygon in green
    if (!clipped.empty()) {

        glColor3f(0.0, 0.8, 0.2);

        glBegin(GL_POLYGON);

        for (Point p : clipped)
            glVertex2f(p.x, p.y);

        glEnd();

        glColor3f(0.0, 0.3, 0.0);
        glLineWidth(3);

        glBegin(GL_LINE_LOOP);

        for (Point p : clipped)
            glVertex2f(p.x, p.y);

        glEnd();
    }
}

// Display function
void display() {

    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    drawClippingWindow();
    drawLines();
    drawPolygon();

    glFlush();
}

// Initialize OpenGL
void init() {

    glClearColor(1.0, 1.0, 1.0, 1.0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(0, 800, 0, 600);
}

// Main function
int main(int argc, char** argv) {

    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);

    glutCreateWindow("CO5 - Line and Polygon Clipping");

    init();

    glutDisplayFunc(display);

    glutMainLoop();

    return 0;
}
