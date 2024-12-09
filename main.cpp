#include <windows.h>
#include <GL/glut.h>
#include <math.h>

// GLOBAL VARIABLES
GLfloat canopyHeight = 1.0f;       // Length of Canopy
GLfloat canopyOpenRatio = 0.5f;    // Canopy opening ratio (affects ribs' extension)
GLfloat canopyOpenedRadius = 1.06f; // Radius of the canopy when fully opened
GLfloat runnerPosition = 0.0f;     // Position of the runner on the shaft

bool isMovingUp = false;           // Indicates whether the ribs are moving up
bool isMovingDown = false;         // Indicates whether the ribs are moving down
GLfloat ribYOffset = 0.0f;         // Y-offset for gradual movement
GLfloat ribTargetOffset = 0.4f;    // Target offset for the ribs' final position
GLfloat movementSpeed = 0.01f;     // Speed of movement

// Function to draw the runner (the central part connecting the ribs to the shaft)
void drawRunner() {
    glColor3f(0.4f, 0.4f, 0.4f); // Set runner color
    glPushMatrix();
    runnerPosition = 0.5f * canopyHeight * canopyOpenRatio; // Dynamic position
    glTranslatef(0.009f, runnerPosition - 0.1f, 0.0f);        // Position the runner on the shaft
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);                     // Rotate to align with the shaft
    GLUquadric* quad = gluNewQuadric();
    gluCylinder(quad, 0.04f, 0.04f, 0.5f, 20, 5);           // Small cylindrical runner
    gluDeleteQuadric(quad);
    glPopMatrix();
}

// Function to draw the ribs
void drawRibs() {
    glColor3f(0.1f, 0.1f, 0.1f); // Set rib color (dark gray)
    glLineWidth(3.0f);           // Set rib thickness

    GLfloat angles[4] = {M_PI / 4, 3*M_PI / 4, 5*M_PI / 4, 7*M_PI / 4}; // Angles in radians for 45°, 135°, 225°, 315°
    GLfloat radius = 0.5f;

    for (int i = 0; i < 4; ++i) {
        GLfloat angle = angles[i];
        GLfloat yOffset = runnerPosition - 0.1f + ribYOffset;

        glBegin(GL_QUADS);
        glVertex3f(0.0f, -0.38f, 0.0f);
        glVertex3f(0.05f, -0.38f, 0.0f);
        glVertex3f(radius * cos(angle) + 0.1f, yOffset, radius * sin(angle)); // Start
        glVertex3f(radius * cos(angle) - 0.1f, yOffset, radius * sin(angle)); // End
        glEnd();
    }

    glLineWidth(1.0f); // Reset line thickness
}

// Timer function
void timerFunction(int value) {
    if (isMovingUp) {
        ribYOffset += movementSpeed;
        if (ribYOffset >= ribTargetOffset) {
            ribYOffset = ribTargetOffset;
            isMovingUp = false; // Stop upward movement
        }
    } else if (isMovingDown) {
        ribYOffset -= movementSpeed;
        if (ribYOffset <= 0.0f) {
            ribYOffset = 0.0f;
            isMovingDown = false; // Stop downward movement
        }
    }

    glutPostRedisplay();                // Redraw the scene
    if (isMovingUp || isMovingDown) {   // Continue the animation if movement is still ongoing
        glutTimerFunc(16, timerFunction, 0); // Call again in ~16 ms (for ~60 FPS)
    }
}

// Key press handler
void handleKeyPress(unsigned char key, int x, int y) {
    if (key == 'P' || key == 'p') {
        if (!isMovingUp && !isMovingDown) {
            if (ribYOffset == 0.0f) {
                isMovingUp = true; // Start moving up
                glutTimerFunc(16, timerFunction, 0);
            } else if (ribYOffset == ribTargetOffset) {
                isMovingDown = true; // Start moving down
                glutTimerFunc(16, timerFunction, 0);
            }
        }
    }
}

// DISPLAY FUNCTION
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(0.0f, 0.0f, -5.0f);  // Move into the screen
    glRotatef(25.0f, 1.0f, 0.0f, 0.0f); // Rotate to view the ribs better

    drawRunner();
    drawRibs();

    glutSwapBuffers();
}

// RESHAPE FUNCTION
void reshape(GLsizei width, GLsizei height) {
    if (height == 0) height = 1; // Prevent division by zero
    float aspect = (float)width / (float)height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, aspect, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

// MAIN FUNCTION
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Umbrella Ribs and Runner");
    glEnable(GL_DEPTH_TEST);

    // Set the background color to white
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // RGB = (1.0, 1.0, 1.0) corresponds to white

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(handleKeyPress); // Register keyboard handler

    glutMainLoop();
    return 0;
}
