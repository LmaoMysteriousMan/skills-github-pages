#include <windows.h>
#include <GL/glut.h>
#include <math.h>

char title[] = "All-In-One Umbrella";// Window title
int refreshMills = 10;// Refresh interval in milliseconds

// GLOBAL VARIABLES
GLfloat angleUmbrella = 0.0f;// Rotational angle for the umbrella
// Umbrella Canopy
GLfloat canopyOpenedRadius = 1.06f;// Initial radius of Canopy
GLfloat canopyHeight = 1.0f;// Length of Canopy
GLfloat canopySpeed = 0.02f;// Speed of Canopy movement
GLfloat canopyOpenRatio = 0.2f;// Speed of Canopy Opening/Closing movement
bool isUmbrellaOpen = false;// Flag for Canopy state
// Solar Panel
const int numPanels = 4;                // Number of panels
const GLfloat cylinderRadius = 0.05f;   // Radius of the hollow cylinder
const GLfloat panelHeight = 0.12f;      // Height of each panel
const GLfloat panelThickness = 0.01f;   // Thickness of each panel
const GLfloat segmentAngle = 360.0f / numPanels; // Angle covered by each segment
const int numSolars = 4;                // Number of panels
const GLfloat solarsRadius = 0.04f;   // Radius of the hollow cylinder
const GLfloat solarsHeight = 0.118f;      // Height of each panel
const GLfloat solarsThickness = 0.01f;   // Thickness of each panel
const GLfloat solarsAngle = 360.0f / numSolars; // Angle covered by each segment
GLfloat solarPanelRotation = 0.0f; // Current rotation angle
const GLfloat maxSolarPanelRotation = 90.0f; // Max rotation angle
const GLfloat solarPanelSpeed = 0.5f; // Speed of rotation
bool isSolarPanelActive = false; // Flag for toggling solar panel behavior
// Umbrella Canopy Shield
GLfloat shieldHeight = 0.0f;// Height of the shield
GLfloat shieldSpeed = 0.01f;// Speed of the shield movement
GLfloat maxShieldHeight = 0.4f;// Maximum height of the shield (66% of shaft length)
bool isShieldActive = false;// Flag for the shield state
// Wind Blowers Leaves
GLfloat leafAngle = 45.0f;
bool rotateUp = true; // Flag for the leaves state
// Umbrella Shaft Extension
GLfloat shaftLength = 1.3f;// Length of the shaft
GLfloat shaftExtendSpeed = 0.02f;// Speed of extension
GLfloat shaftRadius = 0.04f;// Radius of shaft
bool isShaftExtended = false;// Flag for shaft extension state
// Wind Blower
GLfloat blowerHeight = 0.1f;     // Height of the cylinder
GLfloat blowerRadius = 0.0345f;  // The radius of the cylinder
GLfloat blowerOffset = 0.45f;    // The offset of the cylinder (the height from the top of the umbrella)
GLfloat meshRadius = blowerRadius + 0.01f; // The radius of the grid to ensure overlap with the umbrella
GLfloat offsetX = 0.0f;
GLfloat offsetY = -0.01f;
GLfloat offsetZ = -0.01f;
GLfloat angleStep = 2 * M_PI / 30;
GLfloat heightStep = blowerHeight / 10;
// Paper (Proof got Wind)
const float PAPER_WIDTH = 0.2f;
const float PAPER_HEIGHT = 0.3f;
const int PAPER_RES_X = 50;
const int PAPER_RES_Y = 25;
const float WAVE_AMPLITUDE = 0.025f;
const float WAVE_SPEED = 0.1f;
float waveTime = 0.0f;
bool isPaper = false; // Flag for paper state
// Handle(Bottle)
GLfloat handleBaseY = -0.5f;// Base position of the handle
GLfloat handlePositionY = -0.5f;;// Initial Y position of the handle
GLfloat handleMoveSpeed = 0.0001f;// Speed of downward/upward movement
bool isHandleRotate = false;// Flag for handle state
// Mouthpiece
GLfloat mouthpieceWidth = 0.04f;// Width of the mouthpiece
GLfloat mouthpieceHeight = 0.1f;// Height of the mouthpiece
GLfloat mouthpieceDepth = 0.02f;// Depth of the mouthpiece
GLfloat mouthpieceAngle = 0.0f;// Rotation angle for the mouthpiece
GLfloat mouthpieceSpeed = 1.0f;// Speed of mouthpiece
bool isMouthpieceRotate = false;// Flag for mouthpiece state
// Rainfall
GLfloat skyDarkColor[3] = {0.1f, 0.2f, 0.35f}; // Initial dark blue color
GLfloat skyBrightColor[3] = {0.53f, 0.81f, 0.98f}; // Initial light blue color
const int numRaindrops = 5000; // Number of raindrops
struct Raindrop {
    GLfloat x = 0, y = 1, z = 2; // Position
    GLfloat speed = 4.0f;   // Fall speed
};
Raindrop raindrops[numRaindrops];
bool isRaining = false; // Toggle rain state
// Zooming
GLfloat zoomFactor = 1.0f;// Initial zoom distance
const GLfloat zoomSpeed = 0.02f; // Speed of zoom transition
bool isZooming = false;// Flag for zoom state
// Light
static GLfloat groundPlane[4] = {0.0f, 1.0f, 0.0f, 1.5f};// Ground
static GLfloat lightPos[4] = {4.0f, 4.0f, 0.0f, 1.0f};// Light position
static GLfloat lightAmb[4] = {0.2f, 0.2f, 0.2f, 1.0f};// Light ambient
bool isLightingEnabled = true; // Flag for light state

// OPENGL INITIALIZATION
void initGL(){
    glClearColor(0.5f, 0.5f, 1.0f, 1.0f);// Set background color
    glEnable(GL_DEPTH_TEST);// Enable depth testing for z-culling
    glDepthFunc(GL_LEQUAL);// Set the type of depth-test
    glClearDepth(1.0f);// Set background depth to farthest
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glShadeModel(GL_SMOOTH);// Enable smooth shading
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);// Nice perspective corrections
    // Lighting setup
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
    glEnable(GL_NORMALIZE);// Normalize normals for consistent lighting
}

// INITIALIZE RAINFALL
void initRaindrops(){
    for(int i = 0; i < numRaindrops; ++i) {
        raindrops[i].x = (rand()% 200-100)/10.0f; // Random X in [-10, 10]
        raindrops[i].y = (rand()% 100)/10.0f+5.0f; // Random Y in [5, 15]
        raindrops[i].z = (rand()% 200-100)/10.0f; // Random Z in [-10, 10]
        raindrops[i].speed = (rand()%50)/1000.0f+0.05f; // Random speed
    }
}

// MATRIXES FOR SHADOW
void myShadowMatrix(float ground[4], float light[4]){
    float dot = ground[0]*light[0]+ground[1]*light[1]+ground[2]*light[2]+ground[3]*light[3];
    float shadowMat[4][4];
    shadowMat[0][0] = dot-light[0]*ground[0];
    shadowMat[1][0] = 0.0-light[0]*ground[1];
    shadowMat[2][0] = 0.0-light[0]*ground[2];
    shadowMat[3][0] = 0.0-light[0]*ground[3];
    shadowMat[0][1] = 0.0-light[1]*ground[0];
    shadowMat[1][1] = dot-light[1]*ground[1];
    shadowMat[2][1] = 0.0-light[1]*ground[2];
    shadowMat[3][1] = 0.0-light[1]*ground[3];
    shadowMat[0][2] = 0.0-light[2]*ground[0];
    shadowMat[1][2] = 0.0-light[2]*ground[1];
    shadowMat[2][2] = dot-light[2]*ground[2];
    shadowMat[3][2] = 0.0-light[2]*ground[3];
    shadowMat[0][3] = 0.0-light[3]*ground[0];
    shadowMat[1][3] = 0.0-light[3]*ground[1];
    shadowMat[2][3] = 0.0-light[3]*ground[2];
    shadowMat[3][3] = dot-light[3]*ground[3];
    glMultMatrixf((const GLfloat*)shadowMat);
}

void drawCanopy(){
    if(!isLightingEnabled){
        glColor3f(0.3f, 0.1f, 0.1f);// Use glColor3f when lighting is disabled
    }else{
        GLfloat umbrellaColor[4] = {0.8f, 0.4f, 0.4f, 1.0f};// Use material properties when lighting is enabled
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, umbrellaColor);
    }
    for(int i = 0; i < 8; ++i){
        GLfloat angle1 = i*2.0f*M_PI/8;
        GLfloat angle2 = (i+1)*2.0f * M_PI/8;
        for(int j = 0; j < 8; ++j){
            GLfloat height1 = j*canopyHeight/8;
            GLfloat height2 = (j+1)*canopyHeight/8;
            GLfloat radius1 = canopyOpenRatio*canopyOpenedRadius*(1-pow(height1/canopyHeight,2));
            GLfloat radius2 = canopyOpenRatio*canopyOpenedRadius*(1-pow(height2/canopyHeight,2));
            glBegin(GL_TRIANGLES);
            glNormal3f(radius1 * cos(angle1), height1, radius1 * sin(angle1));
            glVertex3f(radius1 * cos(angle1), height1, radius1 * sin(angle1));
            glVertex3f(radius2 * cos(angle1), height2, radius2 * sin(angle1));
            glVertex3f(radius1 * cos(angle2), height1, radius1 * sin(angle2));
            glNormal3f(radius1 * cos(angle1), height1, radius1 * sin(angle1));
            glVertex3f(radius1 * cos(angle2), height1, radius1 * sin(angle2));
            glVertex3f(radius2 * cos(angle1), height2, radius2 * sin(angle1));
            glVertex3f(radius2 * cos(angle2), height2, radius2 * sin(angle2));
            glEnd();
        }
    }
}

void drawEndTop(){
    if(!isLightingEnabled){
        glColor3f(0.2f, 0.2f, 0.2f);// Use glColor3f when lighting is disabled
    }else{
        GLfloat endTopColor[4] = {0.4f, 0.4f, 0.4f, 1.0f};// Use material properties when lighting is enabled
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, endTopColor);
    }
    GLUquadric* quad = gluNewQuadric();
    glPushMatrix();
    glTranslatef(0.0f, canopyHeight+0.1f, 0.0f);// Position at the tip of the Umbrella Canopy
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);// Rotate the End Top vertically
    gluCylinder(quad, 0.04f, 0.038f, 0.18f, 80, 80);// Draw cylinder with its dimensions
    glPopMatrix();
    gluDeleteQuadric(quad);// Clean up
}

void drawSolar() {
    if(!isLightingEnabled){
        glColor3f(0.6f, 0.4f, 0.0f);// Use glColor3f when lighting is disabled
    }else{
        GLfloat umbrellaColor[4] = {0.6f, 0.6f, 0.0f, 1.0f};// Use material properties when lighting is enabled
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, umbrellaColor);
    }
    for (int i = 0; i < numSolars; ++i) {
        GLfloat baseAngle = i * solarsAngle; // Base rotation angle for this panel
        glPushMatrix();
        // Position the panel to the top of the canopy and align it
        glTranslatef(0.0f, canopyHeight+0.03f, 0.0f); // Align with the top of the end
        glRotatef(baseAngle, 0.0f, 1.0f, 0.0f); // Rotate to the appropriate segment
        // Rotate outward if activated
        GLfloat currentRotation = isSolarPanelActive ? solarPanelRotation : 0.0f;
        // Clamp the rotation to not exceed 90 degrees
        if (currentRotation > 90.0f) {
            currentRotation = 90.0f;
        }
        glTranslatef(0.0f, -solarsHeight / 2, solarsRadius-0.01f); // Pivot at the bottom of the panel
        glRotatef(currentRotation, 1.0f, 0.0f, 0.0f);         // Rotate outward
        glTranslatef(0.0f, solarsHeight / 2, -solarsRadius); // Restore after pivot
        // Draw the panel as a curved segment of the cylinder
        glBegin(GL_QUADS);
        for (int j = 0; j < 10; ++j) { // Divide the arc into smaller sections for smoothness
            GLfloat theta1 = (j / 10.0f) * (solarsAngle * M_PI / 180.0f);
            GLfloat theta2 = ((j + 1) / 10.0f) * (solarsAngle * M_PI / 180.0f);
            GLfloat xOuter1 = solarsRadius * cos(theta1)-0.01f;
            GLfloat zOuter1 = solarsRadius * sin(theta1);
            GLfloat xOuter2 = solarsRadius * cos(theta2)-0.01f;
            GLfloat zOuter2 = solarsRadius * sin(theta2);
            GLfloat xInner1 = (solarsRadius - solarsThickness) * cos(theta1);
            GLfloat zInner1 = (solarsRadius - solarsThickness) * sin(theta1);
            GLfloat xInner2 = (solarsRadius - solarsThickness) * cos(theta2);
            GLfloat zInner2 = (solarsRadius - solarsThickness) * sin(theta2);
            // Draw bottom face of the panel
            glNormal3f(0.0f, -1.0f, 0.0f);
            glVertex3f(xOuter1, -solarsHeight / 2, zOuter1);
            glVertex3f(xOuter2, -solarsHeight / 2, zOuter2);
            glVertex3f(xInner2, -solarsHeight / 2, zInner2);
            glVertex3f(xInner1, -solarsHeight / 2, zInner1);
            // Draw top face of the panel
            glNormal3f(0.0f, 1.0f, 0.0f);
            glVertex3f(xOuter1, solarsHeight / 2, zOuter1);
            glVertex3f(xOuter2, solarsHeight / 2, zOuter2);
            glVertex3f(xInner2, solarsHeight / 2, zInner2);
            glVertex3f(xInner1, solarsHeight / 2, zInner1);
            // Draw side faces (outer edges)
            glNormal3f(cos(theta1), 0.0f, sin(theta1));
            glVertex3f(xOuter1, -solarsHeight / 2, zOuter1);
            glVertex3f(xOuter1, solarsHeight / 2, zOuter1);
            glNormal3f(cos(theta2), 0.0f, sin(theta2));
            glVertex3f(xOuter2, solarsHeight / 2, zOuter2);
            glVertex3f(xOuter2, -solarsHeight / 2, zOuter2);
            // Draw side faces (inner edges)
            glNormal3f(-cos(theta1), 0.0f, -sin(theta1));
            glVertex3f(xInner1, -solarsHeight / 2, zInner1);
            glVertex3f(xInner1, solarsHeight / 2, zInner1);
            glNormal3f(-cos(theta2), 0.0f, -sin(theta2));
            glVertex3f(xInner2, solarsHeight / 2, zInner2);
            glVertex3f(xInner2, -solarsHeight / 2, zInner2);
        }
        glEnd();
        glPopMatrix();
    }
}

// Function to draw the solar panel (cylinder shell)
void drawSolarPanel() {
    if(!isLightingEnabled){
        glColor3f(0.2f, 0.2f, 0.2f);// Use glColor3f when lighting is disabled
    }else{
        GLfloat umbrellaColor[4] = {0.4f, 0.4f, 0.4f, 1.0f};// Use material properties when lighting is enabled
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, umbrellaColor);
    }
    for (int i = 0; i < numPanels; ++i) {
        GLfloat baseAngle = i * segmentAngle; // Base rotation angle for this panel
        glPushMatrix();
        // Position the panel to the top of the canopy and align it
        glTranslatef(0.0f, canopyHeight+0.03f, 0.0f); // Align with the top of the end
        glRotatef(baseAngle, 0.0f, 1.0f, 0.0f); // Rotate to the appropriate segment
        // Rotate outward if activated
        GLfloat currentRotation = isSolarPanelActive ? solarPanelRotation : 0.0f;
        // Clamp the rotation to not exceed 90 degrees
        if (currentRotation > 90.0f) {
            currentRotation = 90.0f;
        }
        glTranslatef(0.0f, -panelHeight / 2, cylinderRadius-0.01f); // Pivot at the bottom of the panel
        glRotatef(currentRotation, 1.0f, 0.0f, 0.0f);         // Rotate outward
        glTranslatef(0.0f, panelHeight / 2, -cylinderRadius); // Restore after pivot
        // Draw the panel as a curved segment of the cylinder
        glBegin(GL_QUADS);
        for (int j = 0; j < 10; ++j) { // Divide the arc into smaller sections for smoothness
            GLfloat theta1 = (j / 10.0f) * (segmentAngle * M_PI / 180.0f);
            GLfloat theta2 = ((j + 1) / 10.0f) * (segmentAngle * M_PI / 180.0f);
            GLfloat xOuter1 = cylinderRadius * cos(theta1)-0.01f;
            GLfloat zOuter1 = cylinderRadius * sin(theta1);
            GLfloat xOuter2 = cylinderRadius * cos(theta2)-0.01f;
            GLfloat zOuter2 = cylinderRadius * sin(theta2);
            GLfloat xInner1 = (cylinderRadius - panelThickness) * cos(theta1);
            GLfloat zInner1 = (cylinderRadius - panelThickness) * sin(theta1);
            GLfloat xInner2 = (cylinderRadius - panelThickness) * cos(theta2);
            GLfloat zInner2 = (cylinderRadius - panelThickness) * sin(theta2);
            // Draw bottom face of the panel
            glNormal3f(0.0f, -1.0f, 0.0f);
            glVertex3f(xOuter1, -panelHeight / 2, zOuter1);
            glVertex3f(xOuter2, -panelHeight / 2, zOuter2);
            glVertex3f(xInner2, -panelHeight / 2, zInner2);
            glVertex3f(xInner1, -panelHeight / 2, zInner1);
            // Draw top face of the panel
            glNormal3f(0.0f, 1.0f, 0.0f);
            glVertex3f(xOuter1, panelHeight / 2, zOuter1);
            glVertex3f(xOuter2, panelHeight / 2, zOuter2);
            glVertex3f(xInner2, panelHeight / 2, zInner2);
            glVertex3f(xInner1, panelHeight / 2, zInner1);
            // Draw side faces (outer edges)
            glNormal3f(cos(theta1), 0.0f, sin(theta1));
            glVertex3f(xOuter1, -panelHeight / 2, zOuter1);
            glVertex3f(xOuter1, panelHeight / 2, zOuter1);
            glNormal3f(cos(theta2), 0.0f, sin(theta2));
            glVertex3f(xOuter2, panelHeight / 2, zOuter2);
            glVertex3f(xOuter2, -panelHeight / 2, zOuter2);
            // Draw side faces (inner edges)
            glNormal3f(-cos(theta1), 0.0f, -sin(theta1));
            glVertex3f(xInner1, -panelHeight / 2, zInner1);
            glVertex3f(xInner1, panelHeight / 2, zInner1);
            glNormal3f(-cos(theta2), 0.0f, -sin(theta2));
            glVertex3f(xInner2, panelHeight / 2, zInner2);
            glVertex3f(xInner2, -panelHeight / 2, zInner2);
        }
        glEnd();
        glPopMatrix();
    }
    drawSolar();
}

void drawShaft(){
    if(!isLightingEnabled){
        glColor3f(0.0f, 0.0f, 0.0f);// Use glColor3f when lighting is disabled
    }else{
        GLfloat umbrellaColor[4] = {0.1f, 0.1f, 0.1f, 1.0f};// Use material properties when lighting is enabled
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, umbrellaColor);
    }
    GLUquadric* quad2 = gluNewQuadric();
    glPushMatrix();
    glTranslatef(0.0f, 0.8f, -0.01f);// Position the Umbrella Shaft to the bottom of the Umbrella Canopy
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);// Rotate to align it vertically
    gluCylinder(quad2, 0.031, 0.04, shaftLength, 20, 8);// Draw the Umbrella Shaft
    glPopMatrix();
    gluDeleteQuadric(quad2);
}

void drawGrid(){
    if(!isLightingEnabled){
        glColor4f(0.7f, 0.7f, 0.7f, 1.0f); // No light, gray
    }else{
        GLfloat blowerColor[4] = {0.9f, 0.9f, 0.9f, 1.0f}; // light, other gray
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blowerColor);
    }
    // Grid
    glPushMatrix();
    glTranslatef(offsetX, offsetY, offsetZ);
    for(int j = 0; j <= 10; ++j){
        float h = blowerOffset+j*heightStep;
        glBegin(GL_LINE_LOOP);
        for(int i = 0; i <= 30; ++i){
            float angle = i*angleStep;
            glVertex3f(meshRadius*cos(angle), h, meshRadius*sin(angle));
        }
        glEnd();
    }
    for(int i = 0; i < 30; ++i){
        float angle = i*angleStep;
        glBegin(GL_LINE_STRIP);
        for(int j = 0; j <= 10; ++j){
            float h = blowerOffset+j*heightStep;
            glVertex3f(meshRadius*cos(angle), h, meshRadius*sin(angle));
        }
        glEnd();
    }
    glPopMatrix();
}

void drawWindBlowers(){
    GLUquadric* quad = gluNewQuadric();
    if(!isLightingEnabled){
        glColor3f(0.2f, 0.2f, 0.2f);// No light, gray
    }else{
        GLfloat blowerColor[4] = {0.4f, 0.4f, 0.4f, 1.0f};// light, other gray
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blowerColor);
    }
    glPushMatrix();
    glTranslatef(0.0f, canopyHeight - blowerOffset, -0.01f);// position
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    gluCylinder(quad, blowerRadius, blowerRadius, blowerHeight, 20, 8);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(offsetX, offsetY, offsetZ);// Offset
    glBegin(GL_QUADS);
    for (int i = 0; i < 30; ++i) {
        float angle1 = i*angleStep;
        float angle2 = (i+1)*angleStep;
        for (int j = 0; j < 10; ++j) {
            float h1 = blowerOffset+j*heightStep;
            float h2 = blowerOffset+(j+1)*heightStep;
            glVertex3f(meshRadius*cos(angle1), h1, meshRadius*sin(angle1));
            glVertex3f(meshRadius*cos(angle2), h1, meshRadius*sin(angle2));
            glVertex3f(meshRadius*cos(angle2), h2, meshRadius*sin(angle2));
            glVertex3f(meshRadius*cos(angle1), h2, meshRadius*sin(angle1));
        }
    }
    glEnd();
    glPopMatrix();
    drawGrid();
    gluDeleteQuadric(quad);
}

void drawHandle(){
    if(!isLightingEnabled){
        glColor3f(0.2f, 0.2f, 0.2f);// Use glColor3f when lighting is disabled
    }else{
        GLfloat umbrellaColor[4] = {0.4f, 0.4f, 0.4f, 1.0f};// Use material properties when lighting is enabled
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, umbrellaColor);
    }
    GLUquadric* quad3 = gluNewQuadric();
    glPushMatrix();
    glColor3f(0.5f, 0.5f, 0.5f);// Handle color
    glTranslatef(0.0f, handlePositionY, -0.01f);// Position the Umbrella Shaft to the bottom of the Umbrella Canopy
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);// Rotate it vertically
    gluCylinder(quad3, 0.04, 0.04, 0.238f, 80, 80);// Create a torus
    glPopMatrix();
    // Draw Top Screw On Handle
    glPushMatrix();
    glColor3f(0.4f, 0.4f, 0.4f);// Screw color
    glTranslatef(0.0f, handlePositionY+0.06f, -0.01f);// Position at the top of the handle
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);// Rotate vertically
    gluCylinder(quad3, 0.02, 0.02, 0.066f, 30, 30);// Create a tiny cylinder to act as the screw
    glPopMatrix();
    // Draw Screw Threads (Helical Pattern)
    float screwRadius = 0.02f;// Matches the radius of the main screw cylinder
    int threadSegments = 200;// Smoothness of the helical thread
    float screwHeight = 0.066f;// Height of the screw
    int threadTurns = 6;// Number of complete spiral turns
    glPushMatrix();
    glColor3f(0.3f, 0.3f, 0.3f);// Thread color
    glLineWidth(1.2f);// Adjust this value as needed
    glBegin(GL_LINE_STRIP);
    for(int i = 0; i < threadSegments; i++){
        // Calculate helical path
        float t = (float)i/threadSegments;// Normalized position along the helix
        float angle = t*threadTurns*360.0f;// Spiral angle
        float y = handlePositionY+0.06f-(t*screwHeight);// Position along the height of the screw
        float x = screwRadius*cos(angle*M_PI/180.0f);// X coordinate (circular path)
        float z = screwRadius*sin(angle*M_PI/180.0f)-0.01f;// Z coordinate (circular path)
        // Translate to thread position
        glVertex3f(x, y, z);
    }
    glEnd();
    glPopMatrix();
    // Draw Handle Details
    float d = handlePositionY-0.03f;
    for(int i = 0; i <= 4; i++){
        glPushMatrix();
        glColor3f(0.5f, 0.5f, 0.5f);// Umbrella Handle color
        glTranslatef(0.0f, d, 0.0f);// Move to the end of the expected area
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);// Rotate torus to align with the Z-axis
        glutSolidTorus(0.02, 0.025, 20, 20);// Create a torus
        glPopMatrix();
        d -= 0.05f;
    }
    gluDeleteQuadric(quad3);
}

void drawMouthpiece(){
    if(!isLightingEnabled){
        glColor3f(0.0f, 0.0f, 0.0f);// Use glColor3f when lighting is disabled
    }else{
        GLfloat umbrellaColor[4] = {0.1f, 0.1f, 0.1f, 1.0f};// Use material properties when lighting is enabled
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, umbrellaColor);
    }
    glPushMatrix();
    glTranslatef(0.0f, handlePositionY-0.18f, -0.04f);// Position it at the bottom of the handle
    glTranslatef(0.0f, -mouthpieceHeight/2.0f, 0.0f);// Translate down by half the height of the mouthpiece
    glRotatef(mouthpieceAngle, 1.0f, 0.0f, 0.0f);// Rotate the mouthpiece
    glTranslatef(0.0f, mouthpieceHeight/2.0f, 0.0f);// Translate back up to position
    glScalef(mouthpieceWidth, mouthpieceHeight, mouthpieceDepth);// Scale to the desired dimensions
    glBegin(GL_QUADS);// Draw The Rectangular Mouthpiece
        // Front face
        glVertex3f(-0.5f, -0.5f, 0.5f);
        glVertex3f(0.5f, -0.5f, 0.5f);
        glVertex3f(0.5f, 0.5f, 0.5f);
        glVertex3f(-0.5f, 0.5f, 0.5f);
        // Back face
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f(-0.5f, 0.5f, -0.5f);
        glVertex3f(0.5f, 0.5f, -0.5f);
        glVertex3f(0.5f, -0.5f, -0.5f);
        // Left face
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f(-0.5f, -0.5f, 0.5f);
        glVertex3f(-0.5f, 0.5f, 0.5f);
        glVertex3f(-0.5f, 0.5f, -0.5f);
        // Right face
        glVertex3f(0.5f, -0.5f, 0.5f);
        glVertex3f(0.5f, -0.5f, -0.5f);
        glVertex3f(0.5f, 0.5f, -0.5f);
        glVertex3f(0.5f, 0.5f, 0.5f);
        // Top face
        glVertex3f(-0.5f, 0.5f, 0.5f);
        glVertex3f(0.5f, 0.5f, 0.5f);
        glVertex3f(0.5f, 0.5f, -0.5f);
        glVertex3f(-0.5f, 0.5f, -0.5f);
        // Bottom face
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f(0.5f, -0.5f, -0.5f);
        glVertex3f(0.5f, -0.5f, 0.5f);
        glVertex3f(-0.5f, -0.5f, 0.5f);
    glEnd();
    glPopMatrix();
}

void drawUSB(){
    if(!isLightingEnabled){
        glColor3f(0.2f, 0.2f, 0.2f);// Use glColor3f when lighting is disabled
    }else{
        GLfloat umbrellaColor[4] = {0.4f, 0.4f, 0.4f, 1.0f};// Use material properties when lighting is enabled
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, umbrellaColor);
    }
    glPushMatrix();
    GLfloat widthUSB = 0.02f;// Width of the USB port
    GLfloat heightUSB = 0.04f;// Height of the USB port
    GLfloat depthInsetUSB = 0.005f;// Radius of the offset of Umbrella Shaft
    GLfloat shaftRadiusUSB = 0.04f;// Radius of the Umbrella Shaft
    GLfloat angleStepUSB = M_PI/20;// Angle step for smooth curvature (more segments for smoother curves)
    glTranslatef(0.0f, -shaftLength+0.9f, 0.0348f);// Adjust the position vertically
    glBegin(GL_QUAD_STRIP);// Draw the curved rectangle
    for(GLfloat angle = -widthUSB/(2*shaftRadiusUSB); angle <= widthUSB/(2*shaftRadiusUSB); angle += angleStepUSB){
        GLfloat x = shaftRadiusUSB*sin(angle);// Curve around the shaft
        GLfloat z = shaftRadiusUSB*cos(angle)-shaftRadiusUSB-depthInsetUSB;
        glVertex3f(x, heightUSB/2.0f, z);// Top edge
        glVertex3f(x, -heightUSB/2.0f, z);// Bottom edge
    }
    glEnd();
    glPopMatrix();
}

GLfloat runnerPosition = 0.0f;
void drawRunner(){
    if(!isLightingEnabled){
        glColor3f(0.2f, 0.2f, 0.2f);// Use glColor3f when lighting is disabled
    }else{
        GLfloat umbrellaColor[4] = {0.4f, 0.4f, 0.4f, 1.0f};// Use material properties when lighting is enabled
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, umbrellaColor);
    }
    GLUquadric* quad4 = gluNewQuadric();
    glPushMatrix();
    runnerPosition = 0.5f*canopyHeight*canopyOpenRatio;// Dynamic position
    glTranslatef(0.0f, runnerPosition-0.1f, -0.01f);// Position the runner on shaft
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);// Rotate to align with the shaft
    gluCylinder(quad4, 0.04f, 0.04f, 0.1f, 20, 5);// Small cylindrical runner
    glPopMatrix();
    gluDeleteQuadric(quad4);// Clean up
}

void drawRibs(){
    if(!isLightingEnabled){
        glColor3f(0.0f, 0.0f, 0.0f);// Use glColor3f when lighting is disabled
    }else{
        GLfloat umbrellaColor[4] = {0.1f, 0.1f, 0.1f, 1.0f};// Use material properties when lighting is enabled
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, umbrellaColor);
    }
    int ribSegments = 20;
    GLfloat step1 = canopyHeight/ribSegments;
    GLfloat angleStep = 2.0f*M_PI/8;
    // Short Ribs
    for(int i = 0; i < 8; ++i){
        GLfloat angle = i*angleStep;
        GLfloat radius = 0.5f*canopyOpenRatio*canopyOpenedRadius;
        GLfloat ribLength = 0.8f*canopyHeight-runnerPosition;
        glBegin(GL_LINES);
        glVertex3f(0.0f, runnerPosition-0.2f, 0.0f);// Start at the shaft
        glVertex3f(radius*cos(angle), runnerPosition+ribLength-0.1f, radius*sin(angle));// End at canopy edge
        glEnd();
    }
    // Long Ribs
    for(int i = 0; i < 8; ++i){
        GLfloat angle = i * angleStep;
        glBegin(GL_LINE_STRIP);
        for(int j = 0; j <= ribSegments; ++j){
            GLfloat y = j*step1;
            GLfloat radius = canopyOpenRatio*canopyOpenedRadius*(1-pow(y/canopyHeight, 2))-0.008f;
            GLfloat x = radius*cos(angle);
            GLfloat z = radius*sin(angle);
            glVertex3f(x, y, z);// Create the rib
        }
        glEnd();
    }
}

void drawWavingPaper(){
    if(!isLightingEnabled){
        glColor3f(0.7f, 0.7f, 0.7f);// No light, gray
    }else{
        GLfloat blowerColor[4] = {0.9f, 0.9f, 0.9f, 1.0f};// light, other gray
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blowerColor);
    }
    glPushMatrix();
    glTranslatef(-0.1f, 1.0f, -10.0f);// Center position of umbrella
    glRotatef(angleUmbrella, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.25f, 0.3f, 0.0f);
    glRotatef(100.0f, 1.0f, 0.0f, 0.0f);// Adjust the angle
    glRotatef(-45.0f, 0.0f, 1.0f, 0.0f);
    glRotatef(80.0f, 0.0f, 0.0f, 1.0f);
    for(int y = 0; y < PAPER_RES_Y - 1; ++y){
        glBegin(GL_TRIANGLE_STRIP);
        for(int x = 0; x < PAPER_RES_X; ++x){
            float px = (x/(float)(PAPER_RES_X-1))*PAPER_WIDTH-PAPER_WIDTH/2.0f;
            float py1 = (y/(float)(PAPER_RES_Y-1))*PAPER_HEIGHT-PAPER_HEIGHT/2.0f;
            float py2 = ((y+1)/(float)(PAPER_RES_Y-1))*PAPER_HEIGHT-PAPER_HEIGHT/2.0f;
            float pz1 = WAVE_AMPLITUDE*sin(px*2.0f*M_PI+waveTime+y*0.2f);
            float pz2 = WAVE_AMPLITUDE*sin(px*2.0f*M_PI+waveTime+(y + 1)*0.2f);
            glVertex3f(px, py1, pz1);
            glVertex3f(px, py2, pz2);
        }
        glEnd();
    }
    glPopMatrix();
}

void updatePaper(){
    if(isPaper){
        drawWavingPaper();
    }
}

void updatePaper(int value){
    if(isPaper){
        waveTime += WAVE_SPEED;
        if(waveTime > 2.0f * M_PI){
            waveTime -= 2.0f * M_PI;
        }
        glutPostRedisplay();
        glutTimerFunc(16, updatePaper, 0); // Schedule the next update only if isPaper is true
    }
}

void drawShield(){
    if(!isLightingEnabled){
        glColor4f(1.0f, 1.0f, 1.0f, 0.2f); // Set color with transparency
    }else{
        GLfloat umbrellaColor[4] = {1.0f, 1.0f, 1.0f, 0.4f};// Use material properties when lighting is enabled
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, umbrellaColor);
    }
    if(shieldHeight > 0.0f){
        GLfloat shieldRadius = canopyOpenedRadius*canopyOpenRatio;// Match the shield radius to the opened canopy radius
        glEnable(GL_BLEND);// Enable blending for transparency
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask(GL_FALSE);// Disable depth writing for the shield
        glPushMatrix();
        glTranslatef(0.0f, -canopyHeight*(1-canopyOpenRatio)-0.01f, 0.0f);// Position the shield at the bottom of the opened canopy
        // Draw the octagonal shield (front and back faces)
        glBegin(GL_QUADS);
        for(int face = 0; face < 2; ++face){// Loop for both front and back faces
            for (int i = 0; i < 8; i++) {
                GLfloat angle1 = i*M_PI/4;// 45 degrees for octagon
                GLfloat angle2 = (i+1)*M_PI/4;
                // Calculate the outer vertices of the octagon
                GLfloat x1 = shieldRadius*cos(angle1);
                GLfloat z1 = shieldRadius*sin(angle1);
                GLfloat x2 = shieldRadius*cos(angle2);
                GLfloat z2 = shieldRadius*sin(angle2);
                // Draw the quad
                glVertex3f(x1, 0.0f, z1);// First vertex of the quad (top)
                glVertex3f(x2, 0.0f, z2);// Second vertex of the quad (top)
                glVertex3f(x2, -shieldHeight, z2);// Third vertex of the quad (bottom)
                glVertex3f(x1, -shieldHeight, z1);// Fourth vertex of the quad (bottom)
            }
            glEnd();
            glFrontFace(face == 0 ? GL_CW : GL_CCW);// Adjust the winding order for the back face
        }
        glPopMatrix();
        glDepthMask(GL_TRUE);// Enable depth writing again
        glDisable(GL_BLEND);// Disable blending after drawing the shield
    }
}

void drawRaindrops() {
    if(!isRaining) return;// Skip drawing if rain is off
    glDisable(GL_LIGHTING);// Disable lighting for simplicity
    glPointSize(2.2f);
    glColor3f(0.8f, 1.8f, 3.0f);// Light blue raindrops
    glBegin(GL_POINTS);
    for(int i = 0; i < numRaindrops; ++i){
        glVertex3f(raindrops[i].x, raindrops[i].y, raindrops[i].z);
    }
    glEnd();
    glEnable(GL_LIGHTING);// Re-enable lighting
}

// FUNCTION TO DRAW THE UMBRELLA
void drawUmbrella(){
    glPushMatrix();
    glTranslatef(0.0f, 1.0f, -10.0f);// Move into the screen, translate to position the Umbrella
    glScalef(zoomFactor, zoomFactor, zoomFactor);
    glRotatef(angleUmbrella, 0.0f, 1.6f, 0.0f);// Rotate the Umbrella
    glRotatef(25.0f, 1.0f, 0.0f, 0.0f);// So the Umbrella face the viewers
    drawCanopy();
    drawEndTop();
    drawSolarPanel();
    drawShaft();
    drawWindBlowers();
    drawHandle();
    drawMouthpiece();
    drawUSB();
    drawRunner();
    drawRibs();
    drawShield();
    glPopMatrix();
}

// FUNCTION TO DRAW THE SHADOW
void drawShadowUmbrella(){
    glPushMatrix();
    glTranslatef(0.0f, 1.0f, -10.0f);// Move into the screen, translate to position the Umbrella
    glScalef(zoomFactor, zoomFactor, zoomFactor);
    glRotatef(angleUmbrella, 0.0f, 1.6f, 0.0f);// Rotate the Umbrella
    glRotatef(25.0f, 1.0f, 0.0f, 0.0f);// So the Umbrella face the viewers
    // Draw Canopy
    for (int i = 0; i < 8; ++i) {
        GLfloat angle1 = i*2.0f*M_PI/8;
        GLfloat angle2 = (i+1)*2.0f*M_PI/8;
        for (int j = 0; j < 8; ++j) {
            GLfloat height1 = j*canopyHeight/8;
            GLfloat height2 = (j+1)*canopyHeight/8;
            GLfloat radius1 = canopyOpenRatio*canopyOpenedRadius*(1-pow(height1/canopyHeight, 2));
            GLfloat radius2 = canopyOpenRatio*canopyOpenedRadius*(1-pow(height2/canopyHeight, 2));
            glBegin(GL_TRIANGLES);
            glNormal3f(radius1*cos(angle1), height1, radius1*sin(angle1));
            glVertex3f(radius1*cos(angle1), height1, radius1*sin(angle1));
            glVertex3f(radius2*cos(angle1), height2, radius2*sin(angle1));
            glVertex3f(radius1*cos(angle2), height1, radius1*sin(angle2));
            glNormal3f(radius1*cos(angle1), height1, radius1*sin(angle1));
            glVertex3f(radius1*cos(angle2), height1, radius1*sin(angle2));
            glVertex3f(radius2*cos(angle1), height2, radius2*sin(angle1));
            glVertex3f(radius2*cos(angle2), height2, radius2*sin(angle2));
            glEnd();
        }
    }
    // Draw End Top
    GLUquadric* quad = gluNewQuadric();
    glPushMatrix();
    glTranslatef(0.0f, canopyHeight+0.1f, 0.0f);// Position at the tip of the Umbrella Canopy
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);// Rotate the End Top vertically
    gluCylinder(quad, 0.04f, 0.038f, 0.18f, 80, 80);// Cylinder dimensions
    glPopMatrix();
    gluDeleteQuadric(quad);// Clean up
    // Draw Panels
    for (int i = 0; i < numPanels; ++i) {
        GLfloat baseAngle = i * segmentAngle; // Base rotation angle for this panel
        glPushMatrix();
        // Position the panel to the top of the canopy and align it
        glTranslatef(0.0f, canopyHeight+0.03f, 0.0f); // Align with the top of the end
        glRotatef(baseAngle, 0.0f, 1.0f, 0.0f); // Rotate to the appropriate segment
        // Rotate outward if activated
        GLfloat currentRotation = isSolarPanelActive ? solarPanelRotation : 0.0f;
        // Clamp the rotation to not exceed 90 degrees
        if (currentRotation > 90.0f) {
            currentRotation = 90.0f;
        }
        glTranslatef(0.0f, -panelHeight / 2, cylinderRadius-0.01f); // Pivot at the bottom of the panel
        glRotatef(currentRotation, 1.0f, 0.0f, 0.0f);         // Rotate outward
        glTranslatef(0.0f, panelHeight / 2, -cylinderRadius); // Restore after pivot
        // Draw the panel as a curved segment of the cylinder
        glBegin(GL_QUADS);
        for (int j = 0; j < 10; ++j) { // Divide the arc into smaller sections for smoothness
            GLfloat theta1 = (j / 10.0f) * (segmentAngle * M_PI / 180.0f);
            GLfloat theta2 = ((j + 1) / 10.0f) * (segmentAngle * M_PI / 180.0f);
            GLfloat xOuter1 = cylinderRadius * cos(theta1)-0.01f;
            GLfloat zOuter1 = cylinderRadius * sin(theta1);
            GLfloat xOuter2 = cylinderRadius * cos(theta2)-0.01f;
            GLfloat zOuter2 = cylinderRadius * sin(theta2);
            GLfloat xInner1 = (cylinderRadius - panelThickness) * cos(theta1);
            GLfloat zInner1 = (cylinderRadius - panelThickness) * sin(theta1);
            GLfloat xInner2 = (cylinderRadius - panelThickness) * cos(theta2);
            GLfloat zInner2 = (cylinderRadius - panelThickness) * sin(theta2);
            // Draw bottom face of the panel
            glNormal3f(0.0f, -1.0f, 0.0f);
            glVertex3f(xOuter1, -panelHeight / 2, zOuter1);
            glVertex3f(xOuter2, -panelHeight / 2, zOuter2);
            glVertex3f(xInner2, -panelHeight / 2, zInner2);
            glVertex3f(xInner1, -panelHeight / 2, zInner1);
            // Draw top face of the panel
            glNormal3f(0.0f, 1.0f, 0.0f);
            glVertex3f(xOuter1, panelHeight / 2, zOuter1);
            glVertex3f(xOuter2, panelHeight / 2, zOuter2);
            glVertex3f(xInner2, panelHeight / 2, zInner2);
            glVertex3f(xInner1, panelHeight / 2, zInner1);
            // Draw side faces (outer edges)
            glNormal3f(cos(theta1), 0.0f, sin(theta1));
            glVertex3f(xOuter1, -panelHeight / 2, zOuter1);
            glVertex3f(xOuter1, panelHeight / 2, zOuter1);
            glNormal3f(cos(theta2), 0.0f, sin(theta2));
            glVertex3f(xOuter2, panelHeight / 2, zOuter2);
            glVertex3f(xOuter2, -panelHeight / 2, zOuter2);
            // Draw side faces (inner edges)
            glNormal3f(-cos(theta1), 0.0f, -sin(theta1));
            glVertex3f(xInner1, -panelHeight / 2, zInner1);
            glVertex3f(xInner1, panelHeight / 2, zInner1);
            glNormal3f(-cos(theta2), 0.0f, -sin(theta2));
            glVertex3f(xInner2, panelHeight / 2, zInner2);
            glVertex3f(xInner2, -panelHeight / 2, zInner2);
        }
        glEnd();
        glPopMatrix();
    }
    // Draw Shaft
    GLUquadric* quad2 = gluNewQuadric();
    glPushMatrix();
    glTranslatef(0.0f, 0.8f, -0.01f); // Position the Umbrella Shaft to the bottom of the Umbrella Canopy
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f); // Rotate to align it vertically
    gluCylinder(quad2, 0.031, 0.04, shaftLength, 20, 8); // Draw the Umbrella Shaft
    glPopMatrix();
    gluDeleteQuadric(quad2);
    GLUquadric* quad3 = gluNewQuadric();
    glPushMatrix();
    glTranslatef(0.0f, handlePositionY, -0.01f);// Position the Umbrella Shaft to the bottom of the Umbrella Canopy
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);// Rotate it vertically
    gluCylinder(quad3, 0.04, 0.04, 0.238f, 80, 80);// Create a torus
    glPopMatrix();
    // Draw Top Screw On Handle
    glPushMatrix();
    glTranslatef(0.0f, handlePositionY+0.06f, -0.01f);// Position at the top of the handle
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);// Rotate vertically
    gluCylinder(quad3, 0.02, 0.02, 0.066f, 30, 30);// Create a tiny cylinder to act as the screw
    glPopMatrix();
    // Draw Screw Threads (Helical Pattern)
    float screwRadius = 0.02f;// Matches the radius of the main screw cylinder
    int threadSegments = 200;// Smoothness of the helical thread
    float screwHeight = 0.066f;// Height of the screw
    int threadTurns = 6;// Number of complete spiral turns
    glPushMatrix();
    glLineWidth(1.2f);// Adjust this value as needed
    glBegin(GL_LINE_STRIP);
    for(int i = 0; i < threadSegments; i++){
        // Calculate helical path
        float t = (float)i/threadSegments;// Normalized position along the helix
        float angle = t*threadTurns*360.0f;// Spiral angle
        float y = handlePositionY+0.06f-(t*screwHeight);// Position along the height of the screw
        float x = screwRadius*cos(angle*M_PI/180.0f);// X coordinate (circular path)
        float z = screwRadius*sin(angle*M_PI/180.0f)-0.01f;// Z coordinate (circular path)
        // Translate to thread position
        glVertex3f(x, y, z);
    }
    glEnd();
    glPopMatrix();
    // Draw Handle Details
    float d = handlePositionY-0.03f;
    for(int i = 0; i <= 4; i++){
        glPushMatrix();
        glTranslatef(0.0f, d, 0.0f);// Move to the end of the expected area
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);// Rotate torus to align with the Z-axis
        glutSolidTorus(0.02, 0.025, 20, 20);// Create a torus
        glPopMatrix();
        d -= 0.05f;
    }
    gluDeleteQuadric(quad3);
    // Draw Mouthpiece
    glPushMatrix();
    glTranslatef(0.0f, handlePositionY-0.18f, -0.04f);// Position it at the bottom of the handle
    glTranslatef(0.0f, -mouthpieceHeight/2.0f, 0.0f);// Translate down by half the height of the mouthpiece
    glRotatef(mouthpieceAngle, 1.0f, 0.0f, 0.0f);// Rotate the mouthpiece
    glTranslatef(0.0f, mouthpieceHeight / 2.0f, 0.0f);// Translate back up to position
    glScalef(mouthpieceWidth, mouthpieceHeight, mouthpieceDepth);// Scale to the desired dimensions
    // Draw The Rectangular Mouthpiece
    glBegin(GL_QUADS);
        // Front face
        glVertex3f(-0.5f, -0.5f, 0.5f);
        glVertex3f(0.5f, -0.5f, 0.5f);
        glVertex3f(0.5f, 0.5f, 0.5f);
        glVertex3f(-0.5f, 0.5f, 0.5f);
        // Back face
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f(-0.5f, 0.5f, -0.5f);
        glVertex3f(0.5f, 0.5f, -0.5f);
        glVertex3f(0.5f, -0.5f, -0.5f);
        // Left face
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f(-0.5f, -0.5f, 0.5f);
        glVertex3f(-0.5f, 0.5f, 0.5f);
        glVertex3f(-0.5f, 0.5f, -0.5f);
        // Right face
        glVertex3f(0.5f, -0.5f, 0.5f);
        glVertex3f(0.5f, -0.5f, -0.5f);
        glVertex3f(0.5f, 0.5f, -0.5f);
        glVertex3f(0.5f, 0.5f, 0.5f);
        // Top face
        glVertex3f(-0.5f, 0.5f, 0.5f);
        glVertex3f(0.5f, 0.5f, 0.5f);
        glVertex3f(0.5f, 0.5f, -0.5f);
        glVertex3f(-0.5f, 0.5f, -0.5f);
        // Bottom face
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f(0.5f, -0.5f, -0.5f);
        glVertex3f(0.5f, -0.5f, 0.5f);
        glVertex3f(-0.5f, -0.5f, 0.5f);
    glEnd();
    glPopMatrix();
    // Draw Canopy Shield
    glColor4f(0.01f, 0.01f, 0.01f, 0.02f); // Set color with transparency
    if(shieldHeight > 0.0f){
        GLfloat shieldRadius = canopyOpenedRadius*canopyOpenRatio;// Match the shield radius to the opened canopy radius
        glEnable(GL_BLEND);// Enable blending for transparency
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask(GL_FALSE);// Disable depth writing for the shield
        glPushMatrix();
        glTranslatef(0.0f, -canopyHeight*(1-canopyOpenRatio)-0.01f, 0.0f);// Position the shield at the bottom of the opened canopy
        glBegin(GL_QUADS);// Draw the octagonal shield (front and back faces)
        for(int face = 0; face < 2; ++face){// Loop for both front and back faces
            for(int i = 0; i < 8; i++){
                GLfloat angle1 = i*M_PI/4;// 45 degrees for octagon
                GLfloat angle2 = (i+1)*M_PI/4;
                // Calculate the outer vertices of the octagon
                GLfloat x1 = shieldRadius*cos(angle1);
                GLfloat z1 = shieldRadius*sin(angle1);
                GLfloat x2 = shieldRadius*cos(angle2);
                GLfloat z2 = shieldRadius*sin(angle2);
                // Draw the quad
                glVertex3f(x1, 0.0f, z1);// First vertex of the quad (top)
                glVertex3f(x2, 0.0f, z2);// Second vertex of the quad (top)
                glVertex3f(x2, -shieldHeight, z2);// Third vertex of the quad (bottom)
                glVertex3f(x1, -shieldHeight, z1);// Fourth vertex of the quad (bottom)
            }
            glEnd();
            glFrontFace(face == 0 ? GL_CW : GL_CCW);// Adjust the winding order for the back face
        }
        glPopMatrix();
        glDepthMask(GL_TRUE);// Enable depth writing again
        glDisable(GL_BLEND);// Disable blending after drawing the shield
    }
    glPopMatrix();
}

// FUNCTION TO DRAW SKY
void drawSky() {
    glDepthMask(GL_FALSE);// Disable depth writes for the shadow
    glPushMatrix();
    glTranslatef(0.0f, -1.5f, -12.0f);
    if(!isLightingEnabled){
        glColor4f(skyDarkColor[0], skyDarkColor[1], skyDarkColor[2], 1.0f);// Use glColor3f when lighting is disabled
    }else{
        GLfloat skyMatAmbient[4] = {skyBrightColor[0], skyBrightColor[1], skyBrightColor[2], 1.0f};// Use material properties when lighting is enabled
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, skyMatAmbient);
    }
    glBegin(GL_QUADS);
    glVertex3f(-50.0f, 50.0f, 0.0f);  // Top-left
    glVertex3f(50.0f, 50.0f, 0.0f);   // Top-right
    glVertex3f(50.0f, -50.0f, 0.0f);  // Bottom-right
    glVertex3f(-50.0f, -50.0f, 0.0f); // Bottom-left
    glEnd();
    glPopMatrix();
    glDepthMask(GL_TRUE); // Re-enable depth writes
}

// FUNCTION TO DRAW GROUND
void drawGround() {
    glDepthMask(GL_FALSE);// Disable depth writes for the shadow
    glPushMatrix();
    glTranslatef(0.0f, -1.5f, -12.0f);
    if(!isLightingEnabled){
        glColor3f(0.1f, 0.3f, 0.1f);// Use glColor3f when lighting is disabled
    }else{
        GLfloat groundMatAmbient[4] = {0.0f, 0.3f, 0.0f, 0.6f};// Use material properties when lighting is enabled
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, groundMatAmbient);
    }
    glBegin(GL_QUADS);
    glVertex3f(-7.0f, 0.9f, -1.0f);
    glVertex3f(7.0f, 0.9f, -1.0f);
    glVertex3f(7.0f, -1.0f, 1.0f);
    glVertex3f(-7.0f, -1.0f, 1.0f);
    glEnd();
    glPopMatrix();
    glDepthMask(GL_TRUE); // Re-enable depth writes
}

// FUNCTION TO DRAW SHADOW
void drawShadow(){
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    if(!isLightingEnabled){
        glColor4f(0.0f, 0.0f, 0.0f, 0.6f);// Use glColor3f when lighting is disabled
    }else{
        GLfloat shadowMatAmbient[] = {0.0f, 0.0f, 0.0f, 0.7f};// Use material properties when lighting is enabled
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, shadowMatAmbient);
    }
    glPushMatrix();
    glTranslatef(0.0f, -1.5f, -7.5f); // Match ground position
    myShadowMatrix(groundPlane, lightPos);
    drawShadowUmbrella();
    glPopMatrix();
    glDisable(GL_BLEND);
}

// ANIMATION FUNCTION FOR SOLAR PANEL
void updateSolarPanel() {
    if (isSolarPanelActive) {
        // Rotate outward
        if (solarPanelRotation < maxSolarPanelRotation) {
            solarPanelRotation += solarPanelSpeed; // Increment rotation angle
        }
    } else {
        // Rotate back to original position
        if (solarPanelRotation > 0.0f) {
            solarPanelRotation -= solarPanelSpeed; // Decrement rotation angle
        }
        // Clamp to ensure it doesn't overshoot
        if (solarPanelRotation < 0.0f) {
            solarPanelRotation = 0.0f;
        }
    }
}

// ANIMATION FUNCTION FOR RAINING
void updateRaindrops(){
    for(int i = 0; i < numRaindrops; ++i){
        raindrops[i].y -= raindrops[i].speed;// Move down
        // Check collision with canopy
        GLfloat canopyY = 1.0f;// Y-position of the canopy
        GLfloat canopyRadius = canopyOpenRatio*canopyOpenedRadius;
        if(raindrops[i].y < canopyY && sqrt(raindrops[i].x*raindrops[i].x+raindrops[i].z*raindrops[i].z) < canopyRadius){
            // Reset raindrop to the top
            raindrops[i].y = (rand()% 100)/10.0f+5.0f;
            raindrops[i].x = (rand()% 200-100)/10.0f;
            raindrops[i].z = (rand()% 200-100)/10.0f;
            raindrops[i].speed = (rand()%50)/1000.0f+0.02f;
        }
        // Reset raindrops that fall below the ground
        if(raindrops[i].y < -1.0f){
            raindrops[i].y = (rand()%100)/10.0f +5.0f;
            raindrops[i].x = (rand()%200-100)/10.0f;
            raindrops[i].z = (rand()%200-100)/10.0f;
            raindrops[i].speed = (rand()%50)/1000.0f+0.02f;
        }
    }
}

// ANIMATION FUNCTION FOR ZOOMING
void updateZoom(){
    if(isZooming && zoomFactor < 1.5f){// Max scaling factor
        zoomFactor += zoomSpeed;// Gradually increase zoom factor
    }else if(!isZooming && zoomFactor > 1.0f){// Default size
        zoomFactor -= zoomSpeed;// Gradually reset zoom factor
    }
}

// Update light
void updateLight(){
    if(isLightingEnabled){
        glEnable(GL_LIGHTING);
    }else{
        glDisable(GL_LIGHTING);
    }
    drawSky();
    drawGround();
    drawUmbrella();
    drawShadow();
}

void renderTitleText(const char* text, float x, float y, float z){
    glPushMatrix();
    glTranslatef(x, y, z);// Move to the specified position
    glScalef(0.0008f, 0.0008f, 1.0f);// Scale down the text to make it fit well on the screen
    for(const char* c = text; *c != '\0'; c++){
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);// Render each character
    }
    glPopMatrix();
}

void drawTitleOnScreen(){
    glDisable(GL_LIGHTING);// Disable lighting for the text
    glColor3f(1.0f, 1.0f, 1.0f);// Set text color to white
    renderTitleText("The", -0.97f, 1.0f, -5.0f);
    renderTitleText("All-In-One", -1.1f, 0.9f, -5.0f);
    renderTitleText("Umbrella", -1.06f, 0.8f, -5.0f);
    renderTitleText("Operations Button:", 0.2f, -0.2f, -5.0f);
    glEnable(GL_LIGHTING); // Re-enable lighting
}

void renderOperationText(const char* text, float x, float y, float z){
    glPushMatrix();
    glTranslatef(x, y, z);// Move to the specified position
    glScalef(0.0004f, 0.0004f, 1.0f);// Scale down the text to make it fit well on the screen
    for(const char* c = text; *c != '\0'; c++){
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);// Render each character
    }
    glPopMatrix();
}

void drawOperationsOnScreen(){
    glDisable(GL_LIGHTING);// Disable lighting for the text
    glColor3f(0.0f, 0.0f, 0.0f);// Set text color to white
    renderOperationText("'o/O' - Canopy", 0.5f, -0.28f, -5.0f);
    renderOperationText("'e/E' - Shaft", 0.5f, -0.35f, -5.0f);
    renderOperationText("'u/U' - USB", 0.5f, -0.42f, -5.0f);
    renderOperationText("'p/P' - Solar Panels", 0.5f, -0.49f, -5.0f);
    renderOperationText("'b/B' - Bottle", 0.5f, -0.56f, -5.0f);
    renderOperationText("'d/D' - Drink", 0.5f, -0.63f, -5.0f);
    renderOperationText("'w/W' - Wind", 0.5f, -0.7f, -5.0f);
    renderOperationText("'s/S' - Shield", 0.5f, -0.77f, -5.0f);
    renderOperationText("'l/L' - Light(Sun)", 0.5f, -0.84f, -5.0f);
    renderOperationText("'r/R' - Rain", 0.5f, -0.91f, -5.0f);
    glEnable(GL_LIGHTING); // Re-enable lighting
}

// FUNCTION FOR DISPLAY
void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    updateSolarPanel();
    updateLight();
    drawShadow();
    if(isPaper){
        drawWavingPaper();
    }
    drawRaindrops();
    drawTitleOnScreen();
    drawOperationsOnScreen();
    glutSwapBuffers();// Swap buffers
    angleUmbrella += 0.2f;// Update rotation angle for animation
}

// ANIMATION FUNCTION FOR CANOPY
void updateCanopy(){
    if(isUmbrellaOpen && canopyOpenRatio < 1.0f){
        canopyOpenRatio += canopySpeed;// Open
    }else if(!isUmbrellaOpen && canopyOpenRatio > 0.2f){
        canopyOpenRatio -= canopySpeed;// Close
    }
}

// ANIMATION FUNCTION FOR SHIELD EXTENSION
void updateShield(){
    if(isShieldActive && isUmbrellaOpen){
        // Increase shield height until it reaches maxShieldHeight
        if(shieldHeight < maxShieldHeight){
            shieldHeight += shieldSpeed;// Increase shield height
        }
    }else{
        // Decrease shield height until it reaches 0
        if(shieldHeight > 0.0f){
            shieldHeight -= shieldSpeed;// Decrease shield height
        }
    }
}

// ANIMATION FUNCTION FOR SHAFT EXTENSION
void updateShaft(){
    if(isShaftExtended && shaftLength < 1.6f){
        shaftLength += shaftExtendSpeed;
    }else if(!isShaftExtended && shaftLength > 1.3f){
        shaftLength -= shaftExtendSpeed;
    }
    // Update handle position based on shaft length
    if(isHandleRotate){
        handlePositionY = handleBaseY-(shaftLength-1.3f)-0.5f;// Adjust handle position based on shaft length
    }else{
        handlePositionY = handleBaseY-(shaftLength-1.3f);// Keep it aligned when not rotating
    }
}

// ANIMATION FUNCTION FOR HANDLE MOVEMENT
void updateHandle(){
    // If the handle is attached to the shaft
    if(isHandleRotate){
        // Limit the downward movement
        if(handlePositionY > handleBaseY-0.4f){// Adjust this value to limit downward movement
            handlePositionY -= handleMoveSpeed;// Move the handle down
        }
    }else{
        // Return to the base position only if it's below the base
        if(handlePositionY < handleBaseY){
            handlePositionY += handleMoveSpeed;// Move the handle up
        }
    }
    if(handlePositionY < handleBaseY-0.4f){
        handlePositionY = handleBaseY-0.4f;// Clamp the value
    }
}

// ANIMATION FUNCTION FOR MOUTHPIECE
void updateMouthpiece(){
    if(isMouthpieceRotate && mouthpieceAngle > -90.0f){
        mouthpieceAngle -= mouthpieceSpeed;
        if(mouthpieceAngle < -90.0f){
            mouthpieceAngle = -90.0f;// Cap the angle at 90 degrees
        }
    }else if(!isMouthpieceRotate && mouthpieceAngle < 0.0f){
        mouthpieceAngle += mouthpieceSpeed;
        if(mouthpieceAngle > 0.0f){
            mouthpieceAngle = 0.0f;// Cap the angle at 0 degrees
        }
    }
}

// ANIMATION FUNCTION FOR SKY
void updateSkyColor(){
    if(isRaining){
        for(int i = 0; i < 3; ++i){
            skyBrightColor[i] -= 0.002f;// Gradual darkening
            if (skyBrightColor[0] < 0.1f){
                skyBrightColor[0] = 0.1f;
                skyBrightColor[1] = 0.2f;
                skyBrightColor[2] = 0.35f;
            }
        }
    }else{
        for(int i = 0; i < 3; ++i){
            skyBrightColor[i] += 0.003f;// Gradual brightening
            if (skyBrightColor[0] > 0.53f){
                skyBrightColor[0] = 0.53f;
                skyBrightColor[1] = 0.81f;
                skyBrightColor[2] = 0.98f;
            }
        }
    }
}

// TIMER FUNCTION TO REFRESH DISPLAY
void timer(int value){
    updateCanopy();// Update canopy height
    updateShield();
    updateShaft();// Update Shaft length
    updateHandle();// Update Handle(Bottle) rotation
    updateMouthpiece();
    updateRaindrops();
    updateSkyColor();
    updateZoom();// Update zooming
    glutPostRedisplay();// Post a re-paint request to activate display()
    angleUmbrella += 1.0f;// Increase the rotation angle
    if(angleUmbrella > 360.0f){
        angleUmbrella -= 360.0f;// Reset the angle if it exceeds 360 degrees
    }
    glutTimerFunc(refreshMills, timer, 0);// Call the timer function again
}

// KEY INTERACTION FUNCTION
void handleKey(unsigned char key, int x, int y){
    if(key == 'b' || key == 'B'){
        isHandleRotate = !isHandleRotate;
    }
    if(key == 'u' || key == 'U'){// Enlarge the charging spot view
        isZooming = !isZooming;
    }
    if(key == 'p' || key == 'P'){// Enlarge the charging spot view
        isSolarPanelActive = !isSolarPanelActive;
    }
    if(key == 'd' || key == 'D'){// Rotate mouthpiece
        isMouthpieceRotate = !isMouthpieceRotate;
    }
    if(key == 'e' || key == 'E'){// Extend the Shaft
        isShaftExtended = !isShaftExtended;
    }
    if(key == 'l' || key == 'L'){// Toggle lighting
        isLightingEnabled = !isLightingEnabled;
    }
    if(key == 'o' || key == 'O'){// Open/close the Umbrella
        isUmbrellaOpen = !isUmbrellaOpen;
    }
    if(key == 'r' || key == 'R'){
        isRaining = !isRaining;// Toggle rain
        if(isRaining) initRaindrops();// Reset raindrops if rain starts
        isLightingEnabled = !isLightingEnabled;
    }
    if(key == 's' || key == 'S'){// Toggle the canopy extension
        isShieldActive = !isShieldActive;// This will toggle the visibility
    }
    if(key == 'w' || key == 'W'){
        isPaper = !isPaper;
        if(isPaper){
            glutTimerFunc(0, updatePaper, 0);// Restart the paper animation
        }
    }
    glutPostRedisplay();
}

// RESHAPE FUNCTION
void reshape(GLsizei width, GLsizei height){
    if(height == 0) height = 1;  // Prevent division by zero
    float aspect = (float)width / (float)height;
    // Set the viewport to cover the whole window
    glViewport(0, 0, width, height);
    // Switch to the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Set up the perspective projection
    gluPerspective(25.0, aspect, 0.1, 100.0);
    // Switch back to the modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// MAIN FUNCTION
int main(int argc, char** argv){
    glutInit(&argc, argv);// Initialize GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);// Double buffered
    glutInitWindowSize(880, 1400);// Set the window's initial width & height
    glutInitWindowPosition(50, 50);// Position window's initial top-left corner
    glutCreateWindow(title);// Create window with the given title
    initGL();// OpenGL initialization
    glutDisplayFunc(display);// Register display callback handler
    glutReshapeFunc(reshape);// Register reshape callback handler
    glutKeyboardFunc(handleKey);// Register keyboard callback handler
    glutTimerFunc(0, timer, 0);// First timer call immediately
    glutMainLoop();// Enter event-processing loop
    return 0;
}

}
