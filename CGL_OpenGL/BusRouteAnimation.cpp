// SMART BUS MANAGEMENT SYSTEM
// CGL CO3: Procedural Texture Mapping, Lighting, and 3D-Style Seat Management
// Controls: Left-click a seat to toggle booking | A=All, B=Booked, V=Vacant, R=Reset

#include <GL/freeglut.h>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <string>
#include <sstream>

const int TOTAL_SEATS = 20;
bool booked[TOTAL_SEATS] = {false};
int selectedSeat = -1;
char displayMode = 'A';
int windowWidth = 900;
int windowHeight = 700;
GLuint floorTexture = 0;
GLuint seatTexture = 0;
GLuint busTexture = 0;

struct SeatPosition {
    float x, y;
};
SeatPosition seatPositions[TOTAL_SEATS];

void drawText(float x, float y, const char* text, void* font = GLUT_BITMAP_HELVETICA_18) {
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.08f, 0.12f, 0.18f);
    glRasterPos2f(x, y);
    for (const char* p = text; *p; ++p) glutBitmapCharacter(font, *p);
}

void makeTexture(GLuint& texture, int type) {
    const int size = 64;
    GLubyte pixels[size][size][3];
    for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
            int checker = ((x / 8) + (y / 8)) % 2;
            if (type == 0) { // speckled dark bus floor
                int grain = (x * 17 + y * 29 + (x * y) % 31) % 20;
                pixels[y][x][0] = 55 + grain + checker * 4;
                pixels[y][x][1] = 67 + grain + checker * 4;
                pixels[y][x][2] = 77 + grain + checker * 4;
            } else if (type == 1) { // woven seat fabric
                int weave = ((x % 4 == 0) || (y % 4 == 0)) ? 22 : 0;
                pixels[y][x][0] = 35 + weave + checker * 7;
                pixels[y][x][1] = 93 + weave + checker * 8;
                pixels[y][x][2] = 145 + weave + checker * 9;
            } else { // metallic body finish
                int stripe = (y % 12 < 2) ? 25 : 0;
                pixels[y][x][0] = 155 + stripe;
                pixels[y][x][1] = 174 + stripe;
                pixels[y][x][2] = 190 + stripe;
            }
        }
    }
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
}

void setMaterial(float r, float g, float b, float shine = 25.0f) {
    GLfloat diffuse[] = {r, g, b, 1.0f};
    GLfloat specular[] = {0.35f, 0.35f, 0.35f, 1.0f};
    GLfloat shininess[] = {shine};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
}

void drawTexturedRect(float x1, float y1, float x2, float y2, float z,
                      GLuint texture, float repeatX = 1.0f, float repeatY = 1.0f) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glColor3f(1, 1, 1);
    glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);
        glTexCoord2f(0, 0); glVertex3f(x1, y1, z);
        glTexCoord2f(repeatX, 0); glVertex3f(x2, y1, z);
        glTexCoord2f(repeatX, repeatY); glVertex3f(x2, y2, z);
        glTexCoord2f(0, repeatY); glVertex3f(x1, y2, z);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void drawBox(float x, float y, float z, float w, float h, float d,
             float r, float g, float b, float shine = 25.0f) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(w, h, d);
    setMaterial(r, g, b, shine);
    glutSolidCube(1.0);
    glPopMatrix();
}

void drawBus() {
    // Main bus chassis with metallic procedural texture.
    glDisable(GL_LIGHTING);
    glColor3f(0.15f, 0.19f, 0.24f);
    glBegin(GL_QUADS);
        glVertex3f(115, 70, -5); glVertex3f(785, 70, -5);
        glVertex3f(785, 625, -5); glVertex3f(115, 625, -5);
    glEnd();
    glEnable(GL_LIGHTING);

    // Body top surface and metallic outer rails.
    drawTexturedRect(120, 75, 780, 620, -1, busTexture, 5, 5);
    drawBox(450, 75, 3, 670, 10, 8, 0.25f, 0.30f, 0.36f, 60);
    drawBox(450, 620, 3, 670, 10, 8, 0.25f, 0.30f, 0.36f, 60);
    drawBox(120, 347, 3, 10, 550, 8, 0.25f, 0.30f, 0.36f, 60);
    drawBox(780, 347, 3, 10, 550, 8, 0.25f, 0.30f, 0.36f, 60);

    // Textured floor, with aisle.
    drawTexturedRect(135, 90, 765, 605, 1, floorTexture, 12, 10);
    glDisable(GL_LIGHTING);
    glColor3f(0.86f, 0.88f, 0.90f);
    glBegin(GL_QUADS);
        glVertex3f(424, 95, 2); glVertex3f(476, 95, 2);
        glVertex3f(476, 600, 2); glVertex3f(424, 600, 2);
    glEnd();
    glColor3f(0.96f, 0.72f, 0.18f);
    glBegin(GL_LINES);
        for (int y = 110; y < 590; y += 35) {
            glVertex3f(450, (float)y, 3);
            glVertex3f(450, (float)y + 18, 3);
        }
    glEnd();
    glEnable(GL_LIGHTING);

    // Front/driver area and entrance details.
    drawBox(450, 590, 5, 95, 30, 10, 0.18f, 0.22f, 0.28f, 40);
    drawBox(450, 565, 5, 120, 5, 7, 0.90f, 0.68f, 0.16f, 45);
    drawBox(735, 120, 4, 45, 55, 7, 0.18f, 0.24f, 0.29f, 35);
    glDisable(GL_LIGHTING);
    drawText(700, 122, "ENTRY", GLUT_BITMAP_HELVETICA_12);
    drawText(390, 592, "DRIVER", GLUT_BITMAP_HELVETICA_12);
    glEnable(GL_LIGHTING);

    // Side windows, rendered as glossy panels.
    for (int x = 160; x <= 700; x += 110) {
        drawBox((float)x, 612, 4, 82, 5, 4, 0.18f, 0.62f, 0.82f, 90);
        drawBox((float)x, 83, 4, 82, 5, 4, 0.18f, 0.62f, 0.82f, 90);
    }
}

void drawSeat(int index) {
    float x = seatPositions[index].x;
    float y = seatPositions[index].y;
    if (displayMode == 'B' && !booked[index]) return;
    if (displayMode == 'V' && booked[index]) return;

    // Seat cushion: a raised, lit 3D block.
    glPushMatrix();
    glTranslatef(x, y, 9);
    if (index == selectedSeat) {
        setMaterial(1.0f, 0.70f, 0.18f, 80);
        glColor3f(1.0f, 0.70f, 0.18f);
    } else if (booked[index]) {
        setMaterial(0.78f, 0.22f, 0.20f, 55);
        glColor3f(0.78f, 0.22f, 0.20f);
    } else {
        setMaterial(0.20f, 0.48f, 0.72f, 55);
        glColor3f(0.20f, 0.48f, 0.72f);
    }
    glPushMatrix();
    glScalef(62, 48, 9);
    glutSolidCube(1.0);
    glPopMatrix();

    // Fabric-textured seat back, slightly raised.
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, seatTexture);
    if (index == selectedSeat) glColor3f(1.0f, 0.85f, 0.45f);
    else if (booked[index]) glColor3f(1.0f, 0.55f, 0.50f);
    else glColor3f(0.78f, 0.90f, 1.0f);
    glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);
        glTexCoord2f(0, 0); glVertex3f(-27, 5, 5);
        glTexCoord2f(1, 0); glVertex3f(27, 5, 5);
        glTexCoord2f(1, 1); glVertex3f(27, 23, 5);
        glTexCoord2f(0, 1); glVertex3f(-27, 23, 5);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    // Seat number and status are overlaid without lighting.
    char label[12];
    std::snprintf(label, sizeof(label), "%02d", index + 1);
    drawText(x - 9, y - 4, label, GLUT_BITMAP_HELVETICA_12);
}

void setupSeatPositions() {
    // Four seats per row, two on each side of the aisle; five rows.
    int index = 0;
    const float rows[] = {150, 235, 320, 405, 490};
    for (int row = 0; row < 5; ++row) {
        seatPositions[index++] = {205, rows[row]};
        seatPositions[index++] = {285, rows[row]};
        seatPositions[index++] = {615, rows[row]};
        seatPositions[index++] = {695, rows[row]};
    }
}

void drawPanel() {
    int bookedCount = 0;
    for (int i = 0; i < TOTAL_SEATS; ++i) if (booked[i]) ++bookedCount;
    int vacantCount = TOTAL_SEATS - bookedCount;

    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.95f, 0.97f, 0.99f);
    glBegin(GL_QUADS);
        glVertex2f(18, 15); glVertex2f(882, 15);
        glVertex2f(882, 58); glVertex2f(18, 58);
    glEnd();

    char info[160];
    std::snprintf(info, sizeof(info), "Seats: %d    Booked: %d    Vacant: %d",
                  TOTAL_SEATS, bookedCount, vacantCount);
    drawText(30, 38, info);
    drawText(30, 675, "SMART BUS - SEAT MANAGEMENT | Textures + Lighting (CGL CO3)");
    drawText(30, 650, "Click a seat to book/unbook | A: All  B: Booked  V: Vacant  R: Reset",
             GLUT_BITMAP_HELVETICA_12);

    // Legend
    glColor3f(0.20f, 0.48f, 0.72f);
    glBegin(GL_QUADS);
        glVertex2f(565, 25); glVertex2f(582, 25);
        glVertex2f(582, 42); glVertex2f(565, 42);
    glEnd();
    drawText(588, 29, "Vacant", GLUT_BITMAP_HELVETICA_12);
    glColor3f(0.78f, 0.22f, 0.20f);
    glBegin(GL_QUADS);
        glVertex2f(655, 25); glVertex2f(672, 25);
        glVertex2f(672, 42); glVertex2f(655, 42);
    glEnd();
    drawText(678, 29, "Booked", GLUT_BITMAP_HELVETICA_12);
    glColor3f(1.0f, 0.70f, 0.18f);
    glBegin(GL_QUADS);
        glVertex2f(755, 25); glVertex2f(772, 25);
        glVertex2f(772, 42); glVertex2f(755, 42);
    glEnd();
    drawText(778, 29, "Selected", GLUT_BITMAP_HELVETICA_12);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Overhead scene lights illuminate raised seat geometry and metallic trim.
    GLfloat lightPosition[] = {450.0f, 350.0f, 500.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    drawBus();
    for (int i = 0; i < TOTAL_SEATS; ++i) drawSeat(i);
    drawPanel();
    glutSwapBuffers();
}

void reshape(int w, int h) {
    windowWidth = w;
    windowHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 900, 0, 700);
    glMatrixMode(GL_MODELVIEW);
}

void mouse(int button, int state, int mx, int my) {
    if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN) return;
    float x = mx * 900.0f / (windowWidth ? windowWidth : 900);
    float y = 700.0f - my * 700.0f / (windowHeight ? windowHeight : 700);

    for (int i = 0; i < TOTAL_SEATS; ++i) {
        if (displayMode == 'B' && !booked[i]) continue;
        if (displayMode == 'V' && booked[i]) continue;
        float dx = x - seatPositions[i].x;
        float dy = y - seatPositions[i].y;
        if (std::fabs(dx) <= 34 && std::fabs(dy) <= 29) {
            booked[i] = !booked[i];
            selectedSeat = i;
            glutPostRedisplay();
            return;
        }
    }
}

void keyboard(unsigned char key, int, int) {
    if (key == 'a' || key == 'A') displayMode = 'A';
    else if (key == 'b' || key == 'B') displayMode = 'B';
    else if (key == 'v' || key == 'V') displayMode = 'V';
    else if (key == 'r' || key == 'R') {
        for (int i = 0; i < TOTAL_SEATS; ++i) booked[i] = false;
        selectedSeat = -1;
        displayMode = 'A';
    } else if (key == 27) {
        std::exit(0);
    }
    glutPostRedisplay();
}

void init() {
    glClearColor(0.82f, 0.87f, 0.91f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);

    // Legacy OpenGL lighting with ambient, diffuse and specular components.
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat ambient[] = {0.42f, 0.42f, 0.42f, 1.0f};
    GLfloat diffuse[] = {0.90f, 0.90f, 0.90f, 1.0f};
    GLfloat specular[] = {0.75f, 0.75f, 0.75f, 1.0f};
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_NORMALIZE);

    makeTexture(floorTexture, 0);
    makeTexture(seatTexture, 1);
    makeTexture(busTexture, 2);
    setupSeatPositions();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(100, 40);
    glutCreateWindow("Smart Bus Management - CGL CO3");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
