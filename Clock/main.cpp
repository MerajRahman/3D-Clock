#include <GL/freeglut.h>
#include <cmath>
#include <ctime>
#include <iostream>

// Camera & Animation variables
float camYaw = 0.0f;
float camPitch = 0.1f;
float camRadius = 18.0f;
bool isDragging = false;
int lastMouseX = 0, lastMouseY = 0;
float animFrame = 0.0f;

// =====================================================================
//  Materials & Colors (Laser-Cut Wood Theme)
// =====================================================================
void setBirchWood() {
    // Light, pale wood color (the face and main body)
    GLfloat mat_ambient[] = { 0.45f, 0.40f, 0.32f, 1.0f };
    GLfloat mat_diffuse[] = { 0.88f, 0.80f, 0.68f, 1.0f };
    GLfloat mat_specular[] = { 0.05f, 0.05f, 0.05f, 1.0f }; // Very matte
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMateriali(GL_FRONT, GL_SHININESS, 4);
}

void setBurntWood() {
    // Dark brown color (simulating the laser-cut burnt edges)
    GLfloat mat_ambient[] = { 0.2f, 0.12f, 0.08f, 1.0f };
    GLfloat mat_diffuse[] = { 0.35f, 0.22f, 0.14f, 1.0f };
    GLfloat mat_specular[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMateriali(GL_FRONT, GL_SHININESS, 8);
}

void setBlackMetal() {
    GLfloat mat_ambient[] = { 0.05f, 0.05f, 0.05f, 1.0f };
    GLfloat mat_diffuse[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    GLfloat mat_specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMateriali(GL_FRONT, GL_SHININESS, 32);
}

void setRedMetal() {
    GLfloat mat_ambient[] = { 0.3f, 0.0f, 0.0f, 1.0f };
    GLfloat mat_diffuse[] = { 0.8f, 0.1f, 0.1f, 1.0f };
    GLfloat mat_specular[] = { 0.4f, 0.2f, 0.2f, 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMateriali(GL_FRONT, GL_SHININESS, 16);
}

// =====================================================================
//  3D Helper: Flat Circular Disk
// =====================================================================
void drawCircle(float radius, int segments) {
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 0.0f, 1.0f);
    for (int i = 0; i < segments; i++) {
        float theta = 2.0f * 3.1415926f * float(i) / float(segments);
        glVertex3f(radius * cosf(theta), radius * sinf(theta), 0.0f);
    }
    glEnd();
}

// =====================================================================
//  3D Helper: Ornate Hand Shape
// =====================================================================
void drawOrnateHand(float length, float width) {
    glPushMatrix();
    // Base line
    glPushMatrix();
    glTranslatef(0.0f, length * 0.4f, 0.0f);
    glScalef(width * 0.3f, length * 0.8f, 0.05f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Decorative diamond shape in the middle of the hand
    glPushMatrix();
    glTranslatef(0.0f, length * 0.5f, 0.0f);
    glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
    glScalef(width * 1.5f, width * 1.5f, 0.06f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Pointer tip
    glPushMatrix();
    glTranslatef(0.0f, length * 0.9f, 0.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    glutSolidCone(width, length * 0.2f, 8, 1);
    glPopMatrix();
    glPopMatrix();
}

// =====================================================================
//  3D Helper: Construct the Intricate Wooden Stand
// =====================================================================
void drawFiligreeStructure() {
    // --- ANIMATED PENDULUM (Drawn first so it sits inside the wood) ---
    glPushMatrix();
    glTranslatef(0.0f, -1.0f, -0.6f); // Pivot point inside the clock
    float swing = sin(animFrame * 0.05f) * 15.0f; // 15-degree swing
    glRotatef(swing, 0.0f, 0.0f, 1.0f);

    setBirchWood();
    // Pendulum Arm
    glPushMatrix();
    glTranslatef(0.0f, -2.5f, 0.0f);
    glScalef(0.15f, 5.0f, 0.05f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Pendulum Bob (Detailed)
    setBurntWood();
    glPushMatrix();
    glTranslatef(0.0f, -5.0f, 0.05f);
    glutSolidTorus(0.15f, 0.8f, 16, 32);
    setBirchWood();
    drawCircle(0.8f, 32);
    glPopMatrix();
    glPopMatrix();
    // ----------------------------------------------------------------

    // --- FRONT FILIGREE SHIELD (Simulating carved wood) ---
    // We use a matrix of overlapping rings to create a lace/scrollwork effect
    glPushMatrix();
    glTranslatef(0.0f, -3.0f, 0.0f);

    for (int i = 0; i < 4; i++) {
        float yOffset = i * 1.5f;

        // Left side swirls
        setBirchWood();
        glPushMatrix();
        glTranslatef(-2.0f + (i * 0.2f), yOffset, 0.0f);
        glScalef(1.0f, 1.2f, 0.1f);
        glutSolidTorus(0.2f, 1.0f, 16, 32);
        glPopMatrix();

        // Right side swirls
        glPushMatrix();
        glTranslatef(2.0f - (i * 0.2f), yOffset, 0.0f);
        glScalef(1.0f, 1.2f, 0.1f);
        glutSolidTorus(0.2f, 1.0f, 16, 32);
        glPopMatrix();

        // Center burnt wood joints
        setBurntWood();
        glPushMatrix();
        glTranslatef(0.0f, yOffset + 0.75f, 0.05f);
        glutSolidTorus(0.1f, 0.4f, 16, 16);
        glPopMatrix();
    }

    // Wide elaborate base
    setBirchWood();
    for (float x = -3.5f; x <= 3.5f; x += 1.75f) {
        glPushMatrix();
        glTranslatef(x, -1.0f, 0.0f);
        glScalef(1.0f, 0.8f, 0.1f);
        glutSolidTorus(0.25f, 0.8f, 16, 32);
        glPopMatrix();
    }

    // Solid bottom plate
    setBurntWood();
    glPushMatrix();
    glTranslatef(0.0f, -2.0f, 0.0f);
    glScalef(9.0f, 0.3f, 0.5f);
    glutSolidCube(1.0f);
    glPopMatrix();

    glPopMatrix();

    // Solid backplate to house the clock mechanism
    setBurntWood();
    glPushMatrix();
    glTranslatef(0.0f, 3.0f, -1.0f);
    glScalef(5.5f, 5.5f, 0.2f);
    glutSolidCube(1.0f);
    glPopMatrix();
}

// =====================================================================
//  3D Helper: Draw the Clock Face, Ticks, and Hands
// =====================================================================
void drawClockFace() {
    // Get Current Real Time safely
    time_t now = time(0);
    tm ltm;
    localtime_s(&ltm, &now);

    int hours = ltm.tm_hour;
    int minutes = ltm.tm_min;
    int seconds = ltm.tm_sec;

    float secAngle = -(seconds * 6.0f);
    float minAngle = -(minutes * 6.0f) - (seconds * 0.1f);
    float hourAngle = -((hours % 12) * 30.0f) - (minutes * 0.5f);

    glPushMatrix();
    glTranslatef(0.0f, 4.0f, 0.0f); // Center of the clock face

    // 1. Outer Burnt Wood Ring (Frame)
    setBurntWood();
    glPushMatrix();
    glutSolidTorus(0.3f, 3.0f, 16, 64);
    glPopMatrix();

    // 2. Inner Birch Dial Layer
    setBirchWood();
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -0.1f);
    drawCircle(3.0f, 64);
    glPopMatrix();

    // 3. Inner Decorative Ring
    setBurntWood();
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.05f);
    glutSolidTorus(0.05f, 1.8f, 8, 32);
    glPopMatrix();

    // 4. Tick Marks (Simulating the Roman Numeral Blocks)
    setBurntWood();
    for (int i = 0; i < 12; i++) {
        glPushMatrix();
        glRotatef(i * 30.0f, 0.0f, 0.0f, 1.0f);
        glTranslatef(0.0f, 2.5f, 0.05f);

        if (i % 3 == 0) {
            // Main quarters (Thicker blocks to represent XII, III, VI, IX)
            glScalef(0.3f, 0.6f, 0.1f);
        }
        else {
            // Standard hours
            glScalef(0.1f, 0.4f, 0.1f);
        }
        glutSolidCube(1.0f);
        glPopMatrix();
    }

    // 5. Hour Hand (Ornate Black Metal)
    setBlackMetal();
    glPushMatrix();
    glRotatef(hourAngle, 0.0f, 0.0f, 1.0f);
    glTranslatef(0.0f, 0.0f, 0.2f);
    drawOrnateHand(1.5f, 0.2f);
    glPopMatrix();

    // 6. Minute Hand (Ornate Black Metal)
    glPushMatrix();
    glRotatef(minAngle, 0.0f, 0.0f, 1.0f);
    glTranslatef(0.0f, 0.0f, 0.3f);
    drawOrnateHand(2.3f, 0.15f);
    glPopMatrix();

    // 7. Second Hand (Thin Red Metal)
    setRedMetal();
    glPushMatrix();
    glRotatef(secAngle, 0.0f, 0.0f, 1.0f);
    glTranslatef(0.0f, 0.0f, 0.4f);

    glPushMatrix();
    glTranslatef(0.0f, 0.8f, 0.0f); // Offset to hang backwards
    glScalef(0.04f, 3.5f, 0.04f);
    glutSolidCube(1.0f);
    glPopMatrix();
    glPopMatrix();

    // Center Brass/Gold Pin
    GLfloat gold_diffuse[] = { 0.8f, 0.6f, 0.2f, 1.0f };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, gold_diffuse);
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.5f);
    glutSolidSphere(0.2f, 32, 32);
    glPopMatrix();

    glPopMatrix(); // End Clock Face Center
}

// =====================================================================
//  OpenGL Lighting Setup (Soft Studio Lighting for Wood)
// =====================================================================
void initLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);

    // Soft light from above and slightly to the right
    GLfloat light_pos[] = { 5.0f, 15.0f, 20.0f, 0.0f };
    GLfloat ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f }; // High ambient for bright wood
    GLfloat diffuse[] = { 0.7f, 0.7f, 0.7f, 1.0f };
    GLfloat specular[] = { 0.3f, 0.3f, 0.3f, 1.0f }; // Low specular so wood doesn't look like plastic

    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
}

// =====================================================================
//  Main Render Function
// =====================================================================
void display() {
    // PURE WHITE BACKGROUND (Matches the reference image)
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Camera Math 
    float camX = camRadius * cos(camPitch) * sin(camYaw);
    float camY = camRadius * sin(camPitch) + 1.0f;
    float camZ = camRadius * cos(camPitch) * cos(camYaw);
    gluLookAt(camX, camY, camZ, 0.0f, 1.5f, 0.0f, 0.0f, 1.0f, 0.0f);

    // Draw the clock
    drawFiligreeStructure();
    drawClockFace();

    glutSwapBuffers();
}

// =====================================================================
//  Mouse Input & Animation Loop
// =====================================================================
void mouseButton(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            isDragging = true;
            lastMouseX = x;
            lastMouseY = y;
        }
        else {
            isDragging = false;
        }
    }
    // Scroll wheel zoom
    if (button == 3 && state == GLUT_DOWN) camRadius -= 1.0f;
    if (button == 4 && state == GLUT_DOWN) camRadius += 1.0f;
    if (camRadius < 5.0f) camRadius = 5.0f;
    if (camRadius > 35.0f) camRadius = 35.0f;
}

void mouseMotion(int x, int y) {
    if (isDragging) {
        camYaw -= (x - lastMouseX) * 0.01f;
        camPitch += (y - lastMouseY) * 0.01f;
        if (camPitch > 1.5f) camPitch = 1.5f;
        if (camPitch < -0.5f) camPitch = -0.5f;
        lastMouseX = x;
        lastMouseY = y;
    }
}

void update(int value) {
    animFrame += 1.0f; // Advance pendulum animation
    glutPostRedisplay();
    glutTimerFunc(16, update, 0); // ~60 FPS
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)w / h, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(900, 800);
    glutCreateWindow("3D Laser-Cut Wooden Clock");

    glEnable(GL_DEPTH_TEST);
    initLighting();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMotion);

    // Start update loop
    glutTimerFunc(16, update, 0);

    glutMainLoop();
    return 0;
}