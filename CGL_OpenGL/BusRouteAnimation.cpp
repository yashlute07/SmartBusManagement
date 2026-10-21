#include <GL/freeglut.h>
#include <cstdio>
#include <cstdlib>

// ============================================================
// SMART BUS MANAGEMENT SYSTEM
// CGL CO2 - GEOMETRIC TRANSFORMATIONS
// TOP VIEW - SEAT MANAGEMENT
// ============================================================

const int TOTAL_SEATS = 20;

// false = vacant
// true  = booked
bool booked[TOTAL_SEATS] = {
    false, true, false, true, false,
    false, true, false, false, true,
    false, false, true, false, false,
    true, false, true, false, false
};

int selectedSeat = -1;

// 0 = All seats
// 1 = Booked seats
// 2 = Vacant seats
int displayMode = 0;


// ============================================================
// DRAW TEXT
// ============================================================

void drawText(float x, float y, const char* text)
{
    glRasterPos2f(x, y);

    for (int i = 0; text[i] != '\0'; i++)
    {
        glutBitmapCharacter(
            GLUT_BITMAP_HELVETICA_18,
            text[i]
        );
    }
}


// ============================================================
// DRAW RECTANGLE
// ============================================================

void drawRectangle(
    float x1,
    float y1,
    float x2,
    float y2
)
{
    glBegin(GL_QUADS);

        glVertex2f(x1, y1);
        glVertex2f(x2, y1);
        glVertex2f(x2, y2);
        glVertex2f(x1, y2);

    glEnd();
}


// ============================================================
// DRAW BUS - TOP VIEW
// ============================================================

void drawBus()
{
    // Outer bus body
    glColor3f(
        0.05f,
        0.25f,
        0.65f
    );

    drawRectangle(
        -0.85f,
        -0.65f,
         0.85f,
         0.65f
    );


    // Inner bus area
    glColor3f(
        0.18f,
        0.22f,
        0.27f
    );

    drawRectangle(
        -0.77f,
        -0.57f,
         0.77f,
         0.57f
    );


    // ========================================================
    // TOP WINDOWS
    // ========================================================

    glColor3f(
        0.30f,
        0.75f,
        0.95f
    );

    for (float x = -0.52f; x <= 0.50f; x += 0.26f)
    {
        drawRectangle(
            x,
            0.49f,
            x + 0.18f,
            0.55f
        );
    }


    // ========================================================
    // BOTTOM WINDOWS
    // ========================================================

    for (float x = -0.52f; x <= 0.50f; x += 0.26f)
    {
        drawRectangle(
            x,
            -0.55f,
            x + 0.18f,
            -0.49f
        );
    }


    // ========================================================
    // DRIVER AREA
    // ========================================================

    glColor3f(
        0.10f,
        0.13f,
        0.16f
    );

    drawRectangle(
        -0.76f,
        -0.48f,
        -0.58f,
         0.48f
    );


    // Driver seat
    glColor3f(
        0.30f,
        0.30f,
        0.32f
    );

    drawRectangle(
        -0.73f,
         0.18f,
        -0.61f,
         0.35f
    );


    // Steering wheel
    glColor3f(
        0.05f,
        0.05f,
        0.05f
    );

    glPointSize(18);

    glBegin(GL_POINTS);

        glVertex2f(
            -0.67f,
             0.08f
        );

    glEnd();


    // ========================================================
    // ENTRANCE
    // ========================================================

    glColor3f(
        0.65f,
        0.65f,
        0.65f
    );

    drawRectangle(
        -0.86f,
        -0.54f,
        -0.77f,
        -0.36f
    );


    glColor3f(
        0.95f,
        0.75f,
        0.20f
    );

    glBegin(GL_LINES);

        glVertex2f(
            -0.85f,
            -0.49f
        );

        glVertex2f(
            -0.78f,
            -0.49f
        );

        glVertex2f(
            -0.85f,
            -0.42f
        );

        glVertex2f(
            -0.78f,
            -0.42f
        );

    glEnd();


    // ========================================================
    // CENTRAL AISLE
    // ========================================================

    glColor3f(
        0.10f,
        0.12f,
        0.15f
    );

    drawRectangle(
        -0.52f,
        -0.40f,
         0.70f,
         0.40f
    );
}


// ============================================================
// DRAW ONE SEAT
// ============================================================

void drawSeat(
    int seatNumber,
    float x,
    float y
)
{
    bool isBooked =
        booked[seatNumber - 1];


    // --------------------------------------------------------
    // Display filtering
    // --------------------------------------------------------

    if (displayMode == 1 && !isBooked)
        return;

    if (displayMode == 2 && isBooked)
        return;


    // --------------------------------------------------------
    // GEOMETRIC TRANSFORMATION
    // --------------------------------------------------------

    glPushMatrix();

    // Translation
    glTranslatef(
        x,
        y,
        0.0f
    );

    // Scaling
    glScalef(
        1.0f,
        1.0f,
        1.0f
    );


    // --------------------------------------------------------
    // SEAT COLOR
    // --------------------------------------------------------

    if (selectedSeat == seatNumber)
    {
        // Selected
        glColor3f(
            0.10f,
            0.55f,
            1.0f
        );
    }
    else if (isBooked)
    {
        // Booked
        glColor3f(
            0.90f,
            0.08f,
            0.08f
        );
    }
    else
    {
        // Vacant
        glColor3f(
            0.05f,
            0.75f,
            0.25f
        );
    }


    // Seat body
    drawRectangle(
        -0.065f,
        -0.10f,
         0.065f,
         0.10f
    );


    // Seat border
    glColor3f(
        0.75f,
        0.75f,
        0.75f
    );

    glLineWidth(2.0f);

    glBegin(GL_LINE_LOOP);

        glVertex2f(
            -0.065f,
            -0.10f
        );

        glVertex2f(
             0.065f,
            -0.10f
        );

        glVertex2f(
             0.065f,
             0.10f
        );

        glVertex2f(
            -0.065f,
             0.10f
        );

    glEnd();


    // --------------------------------------------------------
    // SEAT NUMBER
    // --------------------------------------------------------

    char number[5];

    sprintf(
        number,
        "%d",
        seatNumber
    );

    glColor3f(
        1.0f,
        1.0f,
        1.0f
    );

    if (seatNumber < 10)
    {
        drawText(
            -0.018f,
            -0.01f,
            number
        );
    }
    else
    {
        drawText(
            -0.032f,
            -0.01f,
            number
        );
    }


    glPopMatrix();
}


// ============================================================
// DRAW ALL 20 SEATS
// ============================================================

void drawSeats()
{
    // Smaller spacing so all seats stay inside bus
    float startX = -0.42f;

    float gap = 0.12f;


    // ========================================================
    // TOP ROW
    // Seats 1 - 10
    // ========================================================

    for (int i = 0; i < 10; i++)
    {
        drawSeat(
            i + 1,
            startX + (i * gap),
            0.25f
        );
    }


    // ========================================================
    // BOTTOM ROW
    // Seats 11 - 20
    // ========================================================

    for (int i = 0; i < 10; i++)
    {
        drawSeat(
            i + 11,
            startX + (i * gap),
            -0.25f
        );
    }
}


// ============================================================
// COUNT BOOKED SEATS
// ============================================================

int getBookedCount()
{
    int count = 0;

    for (int i = 0; i < TOTAL_SEATS; i++)
    {
        if (booked[i])
        {
            count++;
        }
    }

    return count;
}


// ============================================================
// COUNT VACANT SEATS
// ============================================================

int getVacantCount()
{
    return TOTAL_SEATS - getBookedCount();
}


// ============================================================
// INFORMATION PANEL
// ============================================================

void drawInfoPanel()
{
    int bookedCount =
        getBookedCount();

    int vacantCount =
        getVacantCount();


    glColor3f(
        0.08f,
        0.12f,
        0.17f
    );

    drawRectangle(
         0.48f,
        -0.92f,
         0.94f,
        -0.45f
    );


    glColor3f(
        1.0f,
        1.0f,
        1.0f
    );

    drawText(
        0.52f,
        -0.52f,
        "SMART BUS"
    );


    char text[50];


    sprintf(
        text,
        "Total : %d",
        TOTAL_SEATS
    );

    drawText(
        0.52f,
        -0.61f,
        text
    );


    sprintf(
        text,
        "Booked : %d",
        bookedCount
    );

    drawText(
        0.52f,
        -0.69f,
        text
    );


    sprintf(
        text,
        "Vacant : %d",
        vacantCount
    );

    drawText(
        0.52f,
        -0.77f,
        text
    );


    if (displayMode == 0)
    {
        drawText(
            0.52f,
            -0.85f,
            "View: ALL"
        );
    }
    else if (displayMode == 1)
    {
        drawText(
            0.52f,
            -0.85f,
            "View: BOOKED"
        );
    }
    else
    {
        drawText(
            0.52f,
            -0.85f,
            "View: VACANT"
        );
    }
}


// ============================================================
// LEGEND
// ============================================================

void drawLegend()
{
    // Vacant
    glColor3f(
        0.05f,
        0.75f,
        0.25f
    );

    drawRectangle(
        -0.95f,
         0.86f,
        -0.89f,
         0.92f
    );

    glColor3f(
        1.0f,
        1.0f,
        1.0f
    );

    drawText(
        -0.85f,
         0.87f,
        "Vacant"
    );


    // Booked
    glColor3f(
        0.90f,
        0.08f,
        0.08f
    );

    drawRectangle(
        -0.95f,
         0.76f,
        -0.89f,
         0.82f
    );

    glColor3f(
        1.0f,
        1.0f,
        1.0f
    );

    drawText(
        -0.85f,
         0.77f,
        "Booked"
    );


    // Selected
    glColor3f(
        0.10f,
        0.55f,
        1.0f
    );

    drawRectangle(
        -0.95f,
         0.66f,
        -0.89f,
         0.72f
    );

    glColor3f(
        1.0f,
        1.0f,
        1.0f
    );

    drawText(
        -0.85f,
         0.67f,
        "Selected"
    );
}


// ============================================================
// CONTROLS
// ============================================================

void drawControls()
{
    glColor3f(
        1.0f,
        1.0f,
        1.0f
    );

    drawText(
        -0.95f,
        -0.82f,
        "A: All  B: Booked  V: Vacant"
    );

    drawText(
        -0.95f,
        -0.90f,
        "R: Reset  Click: Book/Unbook"
    );
}


// ============================================================
// DISPLAY
// ============================================================

void display()
{
    glClear(
        GL_COLOR_BUFFER_BIT
    );

    glMatrixMode(
        GL_MODELVIEW
    );

    glLoadIdentity();


    // ========================================================
    // TITLE
    // ========================================================

    glColor3f(
        0.05f,
        0.10f,
        0.18f
    );

    drawText(
        -0.30f,
         0.95f,
        "SMART BUS - SEAT MANAGEMENT"
    );


    // ========================================================
    // BUS
    // ========================================================

    drawBus();


    // ========================================================
    // SEATS
    // ========================================================

    drawSeats();


    // ========================================================
    // INFORMATION
    // ========================================================

    drawInfoPanel();


    // ========================================================
    // LEGEND
    // ========================================================

    drawLegend();


    // ========================================================
    // CONTROLS
    // ========================================================

    drawControls();


    glutSwapBuffers();
}


// ============================================================
// MOUSE CLICK
// ============================================================

void mouse(
    int button,
    int state,
    int mouseX,
    int mouseY
)
{
    if (
        button != GLUT_LEFT_BUTTON ||
        state != GLUT_DOWN
    )
    {
        return;
    }


    // Convert mouse coordinates
    float x =
        (2.0f * mouseX / 900.0f) - 1.0f;

    float y =
        1.0f -
        (2.0f * mouseY / 700.0f);


    float startX = -0.42f;

    float gap = 0.12f;


    // ========================================================
    // TOP ROW
    // ========================================================

    for (int i = 0; i < 10; i++)
    {
        float seatX =
            startX + (i * gap);


        if (
            x >= seatX - 0.075f &&
            x <= seatX + 0.075f &&
            y >= 0.13f &&
            y <= 0.37f
        )
        {
            booked[i] =
                !booked[i];

            selectedSeat =
                i + 1;

            glutPostRedisplay();

            return;
        }
    }


    // ========================================================
    // BOTTOM ROW
    // ========================================================

    for (int i = 0; i < 10; i++)
    {
        float seatX =
            startX + (i * gap);


        if (
            x >= seatX - 0.075f &&
            x <= seatX + 0.075f &&
            y >= -0.37f &&
            y <= -0.13f
        )
        {
            booked[i + 10] =
                !booked[i + 10];

            selectedSeat =
                i + 11;

            glutPostRedisplay();

            return;
        }
    }
}


// ============================================================
// KEYBOARD
// ============================================================

void keyboard(
    unsigned char key,
    int x,
    int y
)
{
    switch (key)
    {
        // ----------------------------------------------------
        // ALL SEATS
        // ----------------------------------------------------

        case 'a':
        case 'A':

            displayMode = 0;

            break;


        // ----------------------------------------------------
        // BOOKED SEATS
        // ----------------------------------------------------

        case 'b':
        case 'B':

            displayMode = 1;

            break;


        // ----------------------------------------------------
        // VACANT SEATS
        // ----------------------------------------------------

        case 'v':
        case 'V':

            displayMode = 2;

            break;


        // ----------------------------------------------------
        // RESET
        // ----------------------------------------------------

        case 'r':
        case 'R':

            for (
                int i = 0;
                i < TOTAL_SEATS;
                i++
            )
            {
                booked[i] = false;
            }

            selectedSeat = -1;

            displayMode = 0;

            break;


        // ----------------------------------------------------
        // ESC
        // ----------------------------------------------------

        case 27:

            exit(0);

            break;
    }


    glutPostRedisplay();
}


// ============================================================
// INITIALIZATION
// ============================================================

void init()
{
    glClearColor(
        0.65f,
        0.70f,
        0.76f,
        1.0f
    );


    glMatrixMode(
        GL_PROJECTION
    );

    glLoadIdentity();


    gluOrtho2D(
        -1.0,
         1.0,
        -1.0,
         1.0
    );


    glMatrixMode(
        GL_MODELVIEW
    );

    glLoadIdentity();
}


// ============================================================
// MAIN
// ============================================================

int main(
    int argc,
    char** argv
)
{
    glutInit(
        &argc,
        argv
    );


    glutInitDisplayMode(
        GLUT_DOUBLE |
        GLUT_RGB
    );


    glutInitWindowSize(
        900,
        700
    );


    glutCreateWindow(
        "Smart Bus - Seat Management (Top View)"
    );


    init();


    glutDisplayFunc(
        display
    );


    glutKeyboardFunc(
        keyboard
    );


    glutMouseFunc(
        mouse
    );


    glutMainLoop();


    return 0;
}