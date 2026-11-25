
#include <GL/freeglut.h>
#include <cmath>
#include <cstdio>

const int ROWS = 10;
const int COLS = 2;
const int TOTAL = ROWS * COLS;

bool booked[TOTAL] = {false};
int selectedSeat = 0;
GLuint floorTexture;

float seatX(int i) {
    return (i % COLS == 0) ? -1.25f : 1.25f;
}

float seatZ(int i) {
    return -7.5f + (i / COLS) * 1.65f;
}

void createFloorTexture() {
    const int size = 64;
    unsigned char pixels[size][size][3];

    // Generate a checker-style floor texture.
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            bool tile = ((x / 8) + (y / 8)) % 2 == 0;

            if (tile) {
                pixels[y][x][0] = 155;
                pixels[y][x][1] = 165;
                pixels[y][x][2] = 170;
            } else {
                pixels[y][x][0] = 95;
                pixels[y][x][1] = 110;
                pixels[y][x][2] = 120;
            }
        }
    }

    glGenTextures(1, &floorTexture);
    glBindTexture(GL_TEXTURE_2D, floorTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGB,
        size, size, 0,
        GL_RGB, GL_UNSIGNED_BYTE, pixels
    );
}

void drawBox(float x, float y, float z,
             float sx, float sy, float sz,
             float r, float g, float b) {
    glPushMatrix();

    glTranslatef(x, y, z);
    glScalef(sx, sy, sz);

    glColor3f(r, g, b);
    glutSolidCube(1.0);

    glPopMatrix();
}

void drawFloor() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, floorTexture);
    glColor3f(1.0f, 1.0f, 1.0f);

    glNormal3f(0.0f, 1.0f, 0.0f);

    glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
        glVertex3f(-3.0f, 0.0f, -9.0f);

        glTexCoord2f(0, 5);
        glVertex3f(-3.0f, 0.0f, 9.0f);

        glTexCoord2f(5, 5);
        glVertex3f(3.0f, 0.0f, 9.0f);

        glTexCoord2f(5, 0);
        glVertex3f(3.0f, 0.0f, -9.0f);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void drawBus() {
    // Floor and bus body.
    drawFloor();

    drawBox(-3.1f, 0.7f, 0, 0.25f, 1.4f, 18,
            0.15f, 0.35f, 0.55f);

    drawBox(3.1f, 0.7f, 0, 0.25f, 1.4f, 18,
            0.15f, 0.35f, 0.55f);

    drawBox(0, 0.7f, -8.8f, 6.2f, 1.4f, 0.3f,
            0.2f, 0.4f, 0.6f);

    drawBox(0, 0.7f, 8.8f, 6.2f, 1.4f, 0.3f,
            0.2f, 0.4f, 0.6f);

    // Central aisle.
    drawBox(0, 0.04f, 0, 0.65f, 0.06f, 17,
            0.85f, 0.78f, 0.58f);

    // Driver area.
    drawBox(0, 0.35f, -8.1f, 1.5f, 0.7f, 0.9f,
            0.25f, 0.25f, 0.3f);

    // Seats: green = vacant, red = booked, blue = selected.
    for (int i = 0; i < TOTAL; i++) {
        float x = seatX(i);
        float z = seatZ(i);

        float r = 0.1f, g = 0.65f, b = 0.2f;

        if (booked[i]) {
            r = 0.85f;
            g = 0.12f;
            b = 0.1f;
        }

        if (i == selectedSeat) {
            r = 0.1f;
            g = 0.3f;
            b = 0.95f;
        }

        // Seat base.
        drawBox(x, 0.2f, z, 1.0f, 0.35f, 1.0f,
                r, g, b);

        // Seat backrest.
        drawBox(x, 0.65f, z + 0.35f, 1.0f, 0.8f, 0.2f,
                r, g, b);

        // Seat armrests.
        drawBox(x - 0.48f, 0.42f, z, 0.08f, 0.3f, 0.8f,
                0.25f, 0.25f, 0.28f);

        drawBox(x + 0.48f, 0.42f, z, 0.08f, 0.3f, 0.8f,
                0.25f, 0.25f, 0.28f);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Top-down 3D camera.
    gluLookAt(
        0.0, 20.0, 0.0,
        0.0, 0.0, 0.0,
        0.0, 0.0, -1.0
    );

    GLfloat lightPosition[] = { -4.0f, 12.0f, -3.0f, 1.0f };
    GLfloat lightAmbient[]  = { 0.35f, 0.35f, 0.35f, 1.0f };
    GLfloat lightDiffuse[]  = { 0.9f, 0.9f, 0.9f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);

    drawBus();

    glutSwapBuffers();
}

void reshape(int w, int h) {
    if (h == 0) h = 1;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspect = (float)w / h;

    if (aspect >= 1.0f) {
        glOrtho(-11 * aspect / 1.1, 11 * aspect / 1.1,
                -11, 11, 1, 40);
    } else {
        glOrtho(-11, 11, -11 / aspect, 11 / aspect, 1, 40);
    }

    glMatrixMode(GL_MODELVIEW);
}

void updateTitle() {
    int count = 0;

    for (int i = 0; i < TOTAL; i++) {
        if (booked[i]) count++;
    }

    char title[150];

    std::snprintf(
        title, sizeof(title),
        "Smart Bus | Seat %d | Booked: %d | Vacant: %d",
        selectedSeat + 1, count, TOTAL - count
    );

    glutSetWindowTitle(title);
}

void keyboard(unsigned char key, int, int) {
    switch (key) {
        case 'b':
        case 'B':
            booked[selectedSeat] = true;
            break;

        case 'v':
        case 'V':
            booked[selectedSeat] = false;
            break;

        case 'r':
        case 'R':
            for (int i = 0; i < TOTAL; i++) {
                booked[i] = false;
            }
            selectedSeat = 0;
            break;

        case 27:
            std::exit(0);
    }

    updateTitle();
    glutPostRedisplay();
}

void specialKeys(int key, int, int) {
    if (key == GLUT_KEY_RIGHT || key == GLUT_KEY_DOWN) {
        selectedSeat = (selectedSeat + 1) % TOTAL;
    }

    if (key == GLUT_KEY_LEFT || key == GLUT_KEY_UP) {
        selectedSeat = (selectedSeat - 1 + TOTAL) % TOTAL;
    }

    updateTitle();
    glutPostRedisplay();
}

void init() {
    glClearColor(0.08f, 0.1f, 0.14f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    glShadeModel(GL_SMOOTH);

    GLfloat specular[] = { 0.6f, 0.6f, 0.6f, 1.0f };
    GLfloat shininess[] = { 40.0f };

    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

    createFloorTexture();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(900, 700);
    glutCreateWindow("Smart Bus - Lighting and Texture Mapping");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);

    updateTitle();

    glutMainLoop();
    return 0;
}
