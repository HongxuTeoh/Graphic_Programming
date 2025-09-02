
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <math.h>

#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "glu32.lib")

#define WINDOW_TITLE "OpenGL Window"

float r = 0.5;

float /*tx = 0, tz = 0,*/ tspeed = 1;		//traslate z with tspeed for modelview
boolean isOrtho = true;			//is ortho view?
float ONear = -10, OFar = 10;	//Ortho near and far
float PNear = 1, PFar = 21;		//Perspective near and far
float s1radius = 3.0;			//sphere 1 radius
float ptx = 0, pty = 0, ptSpeed = 0.1;			//translate x and y for projection
float pry = 0, prSpeed = 1.0;	//rotate y for projection

float headRotation = 0.0f; // Start angle
bool rotateRight = false;       // Direction flag
bool rotate = false;

// Translation and rotation
float tx = 0.0, ty = 0.0, tz = -15.0;
float rotationX = 0.0, rotationY = 0.0;
float rotationSpeed = 2.0;

// Walking animation variables
float walkCycle = 0.0;
float walkSpeed = 0.0;
const float maxSpeed = 0.1f;
const float acceleration = 0.0002f;
const float deceleration = 0.0002f;
float M_PI = 3.14;

// Global variables for rotations
float leftShoulderAngle = 0.0f;   // Left Shoulder rotation
float leftElbowAngle = 0.0f;      // Left Elbow rotation
float leftWristAngle = 0.0f;      // Left Wrist rotation

float rightShoulderAngle = 0.0f;  // Right Shoulder rotation
float rightElbowAngle = 0.0f;     // Right Elbow rotation
float rightWristAngle = 0.0f;     // Right Wrist rotation

// Increment values for animation
bool shoulderUp = true;
bool shoulderMove = false;

// Global camera position and orientation
float cameraX = 0.0f, cameraY = 0.0f, cameraZ = 5.0f;  // Camera position
float lookAtX = 0.0f, lookAtY = 0.0f, lookAtZ = 0.0f; // Where the camera is looking
float upX = 0.0f, upY = 1.0f, upZ = 0.0f;            // "Up" direction for the camera
float cameraSpeed = 1.0f;                            // Speed of camera movement

bool swordEquiped = false;

LRESULT WINAPI WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			PostQuitMessage(0);
		else if (wParam == VK_UP)
			rotationX -= rotationSpeed;
		else if (wParam == VK_DOWN)
			rotationX += rotationSpeed;
		else if (wParam == VK_LEFT)
			rotationY -= rotationSpeed;
		else if (wParam == VK_RIGHT)
			rotationY += rotationSpeed;
		else if (wParam == VK_SPACE) {
			rotationY = 0;
			rotationX = 0;
			walkCycle = 0.0;
			walkSpeed = 0.0;
			rotate = false;
			headRotation = 0.0f;
			leftShoulderAngle = 0.0f;
			leftElbowAngle = 0.0f;
			leftWristAngle = 0.0f;
			rightShoulderAngle = 0.0f;
			rightElbowAngle = 0.0f;
			rightWristAngle = 0.0f;

		}
		else if (wParam == 'M')
			walkSpeed = acceleration, shoulderMove = true;
		else if (wParam == 'N')
			walkSpeed += acceleration;
		else if (wParam == 'B')
			walkSpeed -= deceleration;
		else if (wParam == 'V') {
			if (rotate == false) rotate = true;
			else if (rotate == true) rotate = false;
			else rotate = false;
		}

		else if (wParam == 'A' && leftShoulderAngle < 45.0f)
			leftShoulderAngle += 5.0f;
		else if (wParam == 'Q' && leftShoulderAngle > -45.0f)
			leftShoulderAngle -= 5.0f;
		else if (wParam == 'W' && leftElbowAngle > -90.0f)
			leftElbowAngle -= 5.0f;
		else if (wParam == 'S' && leftElbowAngle < 0.0f)
			leftElbowAngle += 5.0f;
		else if (wParam == 'E' && leftWristAngle < 30.0f)
			leftWristAngle += 5.0f;
		else if (wParam == 'D' && leftWristAngle > -30.0f)
			leftWristAngle -= 5.0f;
		else if (wParam == 'R' && rightShoulderAngle < 45.0f)
			rightShoulderAngle += 5.0f;
		else if (wParam == 'F' && rightShoulderAngle > -45.0f)
			rightShoulderAngle -= 5.0f;
		else if (wParam == 'T' && rightElbowAngle > -90.0f)
			rightElbowAngle -= 5.0f;
		else if (wParam == 'G' && rightElbowAngle < 0.0f)
			rightElbowAngle += 5.0f;
		else if (wParam == 'Y' && rightWristAngle < 30.0f)
			rightWristAngle += 5.0f;
		else if (wParam == 'H' && rightWristAngle > -30.0f)
			rightWristAngle -= 5.0f;
		// Camera movement
		else if (wParam == 'I') // Move camera forward
			cameraZ -= cameraSpeed;
		else if (wParam == 'K') // Move camera backward
			cameraZ += cameraSpeed;
		else if (wParam == 'J') // Move camera left
			cameraX -= cameraSpeed;
		else if (wParam == 'L') // Move camera right
			cameraX += cameraSpeed;
		else if (wParam == 'U') // Move camera up
			cameraY += cameraSpeed;
		else if (wParam == 'O') // Move camera down
			cameraY -= cameraSpeed;

		// Reset camera
		else if (wParam == 'C') {
			cameraX = 0.0f;
			cameraY = 0.0f;
			cameraZ = 5.0f;
			lookAtX = 0.0f;
			lookAtY = 0.0f;
			lookAtZ = 0.0f;
		}
		else if (wParam == VK_TAB) {
			if (swordEquiped == false) swordEquiped = true;
			else if (swordEquiped == true) swordEquiped = false;
			else swordEquiped = false;
		}

		break;
	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
//--------------------------------------------------------------------

bool initPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.cAlphaBits = 8;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 0;

	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;

	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;

	// choose pixel format returns the number most similar pixel format available
	int n = ChoosePixelFormat(hdc, &pfd);

	// set pixel format returns whether it sucessfully set the pixel format
	if (SetPixelFormat(hdc, n, &pfd))
	{
		return true;
	}
	else
	{
		return false;
	}
}
//--------------------------------------------------------------------

void drawSphere(float r) {
	glLineWidth(2.0);
	GLUquadricObj* sphere = NULL; // create quadratic obj pointer
	sphere = gluNewQuadric(); // create qudratic obj in the memory

	gluQuadricDrawStyle(sphere, GLU_LINE); // set draw style to use line
	gluSphere(sphere, r, 30, 30); //draw sphere
	gluDeleteQuadric(sphere);
}

void cylinder(float br, float tr, float h) {
	GLUquadricObj* cyclinder = NULL; // create quadratic obj pointer
	cyclinder = gluNewQuadric(); // create qudratic obj in the memory
	gluQuadricDrawStyle(cyclinder, GLU_LINE); // set draw style to use line
	gluCylinder(cyclinder, br, tr, h, 10, 10); //draw cyclinder
	gluDeleteQuadric(cyclinder);
}

void cylinderFill(float br, float tr, float h) {
	GLUquadricObj* cyclinder = NULL; // create quadratic obj pointer
	cyclinder = gluNewQuadric(); // create qudratic obj in the memory
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	gluQuadricDrawStyle(cyclinder, GLU_FILL); // set draw style to use fill
	gluCylinder(cyclinder, br, tr, h, 10, 10); //draw cyclinder
	gluDeleteQuadric(cyclinder);
}

void drawCone(float br, float h) {
	GLUquadricObj* cone = NULL; // create quadratic obj pointer
	cone = gluNewQuadric(); // create qudratic obj in the memory
	gluQuadricDrawStyle(cone, GLU_LINE); // set draw style to use line
	gluCylinder(cone, br, 0, h, 10, 10); //draw cone
	gluDeleteQuadric(cone);
}

void drawDisk(float inr, float outr) {
	glLineWidth(2.0);
	GLUquadricObj* disk = NULL; // create quadratic obj pointer
	disk = gluNewQuadric(); // create qudratic obj in the memory

	gluQuadricDrawStyle(disk, GLU_LINE); // set draw style to use line
	gluDisk(disk, inr, outr, 30, 30); //draw disk
	gluDeleteQuadric(disk);
}

void drawConeFill(float br, float h) {
	GLUquadricObj* cone = NULL; // create quadratic obj pointer
	cone = gluNewQuadric(); // create qudratic obj in the memory
	glRotatef(270.0f, 1.0f, 0.0f, 0.0f);
	gluQuadricDrawStyle(cone, GLU_FILL); // set draw style to use line
	gluCylinder(cone, br, 0, h, 20, 10); //draw cone
	glTranslatef(0.0f, 0.0f, 0.0);
	drawDisk(0.0f, br);
	gluDeleteQuadric(cone);
}

void drawCuboid(float width, float height, float depth) {
	glBegin(GL_QUADS);

	// Front face
	glVertex3f(-width / 2, -height / 2, depth / 2);
	glVertex3f(width / 2, -height / 2, depth / 2);
	glVertex3f(width / 2, height / 2, depth / 2);
	glVertex3f(-width / 2, height / 2, depth / 2);

	// Back face
	glVertex3f(-width / 2, -height / 2, -depth / 2);
	glVertex3f(width / 2, -height / 2, -depth / 2);
	glVertex3f(width / 2, height / 2, -depth / 2);
	glVertex3f(-width / 2, height / 2, -depth / 2);

	// Top face
	glVertex3f(-width / 2, height / 2, -depth / 2);
	glVertex3f(width / 2, height / 2, -depth / 2);
	glVertex3f(width / 2, height / 2, depth / 2);
	glVertex3f(-width / 2, height / 2, depth / 2);

	// Bottom face
	glVertex3f(-width / 2, -height / 2, -depth / 2);
	glVertex3f(width / 2, -height / 2, -depth / 2);
	glVertex3f(width / 2, -height / 2, depth / 2);
	glVertex3f(-width / 2, -height / 2, depth / 2);

	// Right face
	glVertex3f(width / 2, -height / 2, -depth / 2);
	glVertex3f(width / 2, height / 2, -depth / 2);
	glVertex3f(width / 2, height / 2, depth / 2);
	glVertex3f(width / 2, -height / 2, depth / 2);

	// Left face
	glVertex3f(-width / 2, -height / 2, -depth / 2);
	glVertex3f(-width / 2, height / 2, -depth / 2);
	glVertex3f(-width / 2, height / 2, depth / 2);
	glVertex3f(-width / 2, -height / 2, depth / 2);

	glEnd();
}


// Draw a basic cube
void drawCube(float width, float height, float depth)
{
	glBegin(GL_QUADS);
	// Front face
	glVertex3f(-width / 2, -height / 2, depth / 2);
	glVertex3f(width / 2, -height / 2, depth / 2);
	glVertex3f(width / 2, height / 2, depth / 2);
	glVertex3f(-width / 2, height / 2, depth / 2);

	// Back face
	glVertex3f(-width / 2, -height / 2, -depth / 2);
	glVertex3f(width / 2, -height / 2, -depth / 2);
	glVertex3f(width / 2, height / 2, -depth / 2);
	glVertex3f(-width / 2, height / 2, -depth / 2);

	// Top face
	glVertex3f(-width / 2, height / 2, -depth / 2);
	glVertex3f(width / 2, height / 2, -depth / 2);
	glVertex3f(width / 2, height / 2, depth / 2);
	glVertex3f(-width / 2, height / 2, depth / 2);

	// Bottom face
	glVertex3f(-width / 2, -height / 2, -depth / 2);
	glVertex3f(width / 2, -height / 2, -depth / 2);
	glVertex3f(width / 2, -height / 2, depth / 2);
	glVertex3f(-width / 2, -height / 2, depth / 2);

	// Right face
	glVertex3f(width / 2, -height / 2, -depth / 2);
	glVertex3f(width / 2, height / 2, -depth / 2);
	glVertex3f(width / 2, height / 2, depth / 2);
	glVertex3f(width / 2, -height / 2, depth / 2);

	// Left face
	glVertex3f(-width / 2, -height / 2, -depth / 2);
	glVertex3f(-width / 2, height / 2, -depth / 2);
	glVertex3f(-width / 2, height / 2, depth / 2);
	glVertex3f(-width / 2, -height / 2, depth / 2);
	glEnd();
}

void drawCircle(float radius, int segments) {
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0.0f, 0.0f, 0.0f); // Center of the circle
	for (int i = 0; i <= segments; ++i) {
		float angle = 2.0f * M_PI * i / segments;
		float x = radius * cos(angle);
		float y = radius * sin(angle);
		glVertex3f(x, y, 0.0f);
	}
	glEnd();
}

void drawCylinder(float radius, float height, int segments) {
	float angleIncrement = 2.0f * M_PI / segments;
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <= segments; ++i) {
		float angle = i * angleIncrement;
		float x = radius * cos(angle);
		float y = radius * sin(angle);

		// Bottom circle
		glVertex3f(x, y, -height / 2.0f);

		// Top circle
		glVertex3f(x, y, height / 2.0f);
	}
	glEnd();

	// Optionally, draw the top and bottom faces for better visibility
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0.0f, 0.0f, -height / 2.0f); // center of the bottom circle
	for (int i = 0; i <= segments; ++i) {
		float angle = i * angleIncrement;
		float x = radius * cos(angle);
		float y = radius * sin(angle);
		glVertex3f(x, y, -height / 2.0f);
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0.0f, 0.0f, height / 2.0f); // center of the top circle
	for (int i = 0; i <= segments; ++i) {
		float angle = i * angleIncrement;
		float x = radius * cos(angle);
		float y = radius * sin(angle);
		glVertex3f(x, y, height / 2.0f);
	}
	glEnd();
}

// Draw a trapezoidal shape
void drawTrapezoid(float topWidth, float bottomWidth, float height, float depth)
{
	glBegin(GL_QUADS);
	// Front face
	glVertex3f(-bottomWidth / 2, -height / 2, depth / 2);
	glVertex3f(bottomWidth / 2, -height / 2, depth / 2);
	glVertex3f(topWidth / 2, height / 2, depth / 2);
	glVertex3f(-topWidth / 2, height / 2, depth / 2);

	// Back face
	glVertex3f(-bottomWidth / 2, -height / 2, -depth / 2);
	glVertex3f(bottomWidth / 2, -height / 2, -depth / 2);
	glVertex3f(topWidth / 2, height / 2, -depth / 2);
	glVertex3f(-topWidth / 2, height / 2, -depth / 2);

	// Top face
	glVertex3f(-topWidth / 2, height / 2, -depth / 2);
	glVertex3f(topWidth / 2, height / 2, -depth / 2);
	glVertex3f(topWidth / 2, height / 2, depth / 2);
	glVertex3f(-topWidth / 2, height / 2, depth / 2);

	// Bottom face
	glVertex3f(-bottomWidth / 2, -height / 2, -depth / 2);
	glVertex3f(bottomWidth / 2, -height / 2, -depth / 2);
	glVertex3f(bottomWidth / 2, -height / 2, depth / 2);
	glVertex3f(-bottomWidth / 2, -height / 2, depth / 2);

	// Right face
	glVertex3f(bottomWidth / 2, -height / 2, -depth / 2);
	glVertex3f(bottomWidth / 2, -height / 2, depth / 2);
	glVertex3f(topWidth / 2, height / 2, depth / 2);
	glVertex3f(topWidth / 2, height / 2, -depth / 2);

	// Left face
	glVertex3f(-bottomWidth / 2, -height / 2, -depth / 2);
	glVertex3f(-bottomWidth / 2, -height / 2, depth / 2);
	glVertex3f(-topWidth / 2, height / 2, depth / 2);
	glVertex3f(-topWidth / 2, height / 2, -depth / 2);
	glEnd();
}

//center big waist
void drawCenterWaist()
{
	// Central cube (main waist)
	glPushMatrix();
	glColor3f(0.8f, 0.0f, 0.5f); // Light grey color
	drawCube(1.1f, 1.0f, 2.5f); // Main waist cuboid
	glPopMatrix();

	// Front trapezoid (V-shaped armor) down
	glPushMatrix();
	glTranslatef(0.0f, -0.5f, 1.0f); // Move to the front
	glColor3f(0.3f, 0.3f, 0.8f); // Slightly darker color
	glRotatef(20.0f, 1.0f, 0.0f, 0.0f);
	drawTrapezoid(1.5f, 1.0f, 1.0f, 0.3f);
	glPopMatrix();

	// Front trapezoid (V-shaped armor) Up
	glPushMatrix();
	glTranslatef(0.0f, 0.5f, 1.0f); // Move to the front
	glColor3f(0.3f, 0.3f, 0.8f); // Slightly darker color
	glRotatef(-20.0f, 1.0f, 0.0f, 0.0f);
	drawTrapezoid(1.0f, 1.5f, 1.0f, 0.3f);
	glPopMatrix();

	// Rear trapezoid (Back support armor) down
	glPushMatrix();
	glTranslatef(0.0f, -0.5f, -1.0f); // Move to the back
	glColor3f(0.3f, 0.3f, 0.8f); // Slightly darker color
	glRotatef(-20.0f, 1.0f, 0.0f, 0.0f);
	drawTrapezoid(1.5f, 1.0f, 1.0f, 0.3f);
	glPopMatrix();

	// Rear trapezoid (Back support armor) up
	glPushMatrix();
	glTranslatef(0.0f, 0.5f, -1.0f); // Move to the back
	glColor3f(0.3f, 0.3f, 0.8f); // Slightly darker color
	glRotatef(20.0f, 1.0f, 0.0f, 0.0f);
	drawTrapezoid(1.0f, 1.5f, 1.0f, 0.3f);
	glPopMatrix();

	// Left trapezoidal panel (side decoration)
	glPushMatrix();
	glTranslatef(-0.7f, -0.4f, 0.0f); // Move to the left side
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f); // Rotate to align with side
	glColor3f(0.3f, 0.5f, 0.9f); // Slightly blueish color
	drawTrapezoid(0.6f, 1.0f, 1.2f, 0.2f);
	glPopMatrix();

	// Right trapezoidal panel (side decoration)
	glPushMatrix();
	glTranslatef(0.7f, -0.4f, 0.0f); // Move to the right side
	glRotatef(90.0f, 0.0f, -1.0f, 0.0f); // Rotate to align with side
	glColor3f(0.3f, 0.5f, 0.9f); // Slightly blueish color
	drawTrapezoid(0.6f, 1.0f, 1.2f, 0.2f);
	glPopMatrix();

}
void drawLeftWaist(float swingAngle) {
	glRotatef(swingAngle, 1.0f, 0.0f, 0.0f);
	// Left cube connecting to the left thigh
	glPushMatrix();
	glTranslatef(-1.1f, -0.5f, 0.0f);
	glColor3f(0.7f, 0.7f, 0.7f); // Slightly darker grey
	drawCube(1.1f, 1.0f, 1.5f);
	glPopMatrix();

	// Armor plates (light blue trapezoidal shapes)
	// Front armor on the left thigh waist
	glPushMatrix();
	glTranslatef(-1.25f, -0.8f, 0.8f); // Position in front of the left thigh waist
	glColor3f(0.5f, 0.8f, 1.0f); // Light blue color
	glRotatef(10.0f, -1.0f, 0.0f, 0.0f);
	drawTrapezoid(0.8f, 1.2f, 1.5f, 0.2f); // Trapezoidal armor plate
	glPopMatrix();

	// Rear armor on the left thigh waist
	glPushMatrix();
	glTranslatef(-1.25f, -0.8f, -0.8f);
	glColor3f(0.5f, 0.8f, 1.0f);
	drawTrapezoid(0.8f, 1.2f, 1.5f, 0.2f);
	glPopMatrix();

	// Side armor plates
	// Left side armor plate
	glPushMatrix();
	glTranslatef(-2.0f, -1.0f, 0.0f); // Position to the left of the waist
	glColor3f(0.5f, 0.8f, 1.0f);
	glRotatef(-15.0f, 0.0f, 0.0f, 1.0f); // Tilt slightly outward
	drawTrapezoid(0.3f, 0.2f, 2.0f, 1.0f);  // Long vertical trapezoidal plate
	glPopMatrix();
}
void drawRightWaist(float swingrightAngle) {

	glRotatef(swingrightAngle, 1.0f, 0.0f, 0.0f);
	// Right cube connecting to the right thigh
	glPushMatrix();
	glTranslatef(1.1f, -0.5f, 0.0f);
	glColor3f(0.7f, 0.7f, 0.7f);
	drawCube(1.1f, 1.0f, 1.5f);
	glPopMatrix();

	// Front armor on the right thigh waist
	glPushMatrix();
	glTranslatef(1.25f, -0.8f, 0.8f); // Position in front of the right thigh waist
	glColor3f(0.5f, 0.8f, 1.0f);
	glRotatef(10.0f, -1.0f, 0.0f, 0.0f);
	drawTrapezoid(0.8f, 1.2f, 1.5f, 0.2f);
	glPopMatrix();

	// Rear armor on the right thigh waist
	glPushMatrix();
	glTranslatef(1.25f, -0.8f, -0.8f); // Position behind the right thigh waist
	glColor3f(0.5f, 0.8f, 1.0f);
	drawTrapezoid(0.8f, 1.2f, 1.5f, 0.2f);
	glPopMatrix();

	// Right side armor plate
	glPushMatrix();
	glTranslatef(2.0f, -1.0f, 0.0f); // Position to the right of the waist
	glColor3f(0.5f, 0.8f, 1.0f);
	glRotatef(15.0f, 0.0f, 0.0f, 1.0f); // Tilt slightly outward
	drawTrapezoid(0.3f, 0.2f, 2.0f, 1.0f); // Long vertical trapezoidal plate
	glPopMatrix();
}

void drawLeg(float swingAngle, float kneeBendAngle, float ankleAngle)
{
	glPushMatrix();
	glColor3f(0.0f, 0.0f, 0.8f);
	glRotatef(swingAngle, 1.0f, 0.0f, 0.0f);
	drawCube(0.6f, 2.0f, 0.6f);

	// Thigh armor
	glPushMatrix();
	glTranslatef(0.0f, 0.5f, 0.0f);
	glColor3f(0.3f, 0.8f, 0.9f);
	drawTrapezoid(0.8f, 1.0f, 1.5f, 0.3f); // Main thigh armor
	glPopMatrix();

	// Additional thigh armor - angled front
	glPushMatrix();
	glTranslatef(0.0f, 0.5f, 0.3f);
	glRotatef(-20.0f, 1.0f, 0.0f, 0.0f);
	glColor3f(0.3f, 0.8f, 0.9f);
	drawTrapezoid(0.7f, 0.9f, 1.0f, 0.2f);
	glPopMatrix();

	// Additional thigh armor - angled back
	glPushMatrix();
	glTranslatef(0.0f, 0.5f, -0.5f);
	glRotatef(20.0f, 2.0f, 0.0f, 0.0f);
	glColor3f(0.5f, 1.0f, 0.9f);
	drawTrapezoid(0.7f, 0.9f, 1.0f, 0.2f);
	glPopMatrix();

	// Additional thigh armor - angled side
	glPushMatrix();
	glTranslatef(0.3f, 0.5f, 0.0f);
	glRotatef(-20.0f, 0.0f, 0.0f, 1.0f);
	glColor3f(0.3f, 0.8f, 0.9f);
	drawTrapezoid(0.6f, 0.8f, 1.0f, 0.2f);
	glPopMatrix();

	// Additional thigh armor - angled other side
	glPushMatrix();
	glTranslatef(-0.3f, 0.5f, 0.0f);
	glRotatef(20.0f, 0.0f, 0.0f, 1.0f);
	glColor3f(0.3f, 0.8f, 0.9f);
	drawTrapezoid(0.6f, 0.8f, 1.0f, 0.2f);
	glPopMatrix();

	// Add more armor to the leg
	glPushMatrix();
	// Position the armor at the front-bottom of the thigh
	glTranslatef(0.0f, 0.0f, 0.4f);
	glRotatef(-10.0f, 1.0f, 0.0f, 0.0f);
	glColor3f(0.4f, 0.9f, 1.0f); // Slightly different color for contrast
	drawTrapezoid(0.6f, 0.8f, 1.2f, 0.3f);
	glPopMatrix();


	glPushMatrix();
	// Add a plate-like armor at the back of the ankle
	glTranslatef(0.0f, -1.2f, -0.4f);
	glRotatef(-20.0f, 1.0f, 0.0f, 0.0f);
	glColor3f(0.3f, 0.8f, 0.9f);
	drawCube(0.4f, 0.6f, 0.1f); // Flat armor
	glPopMatrix();

	// Circle on the left side of the shin
	glPushMatrix();
	glTranslatef(-0.35f, -2.0f, 0.0f); // Position on the left shin
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f); // Rotate to face outwards
	glColor3f(0.7f, 0.8f, 0.7f); // Green color for contrast
	drawCylinder(0.3f, 0.05f, 32);  // Smaller radius for side details
	glPopMatrix();

	// Circle on the right side of the shin
	glPushMatrix();
	glTranslatef(0.35f, -2.0f, 0.0f); // Position on the right shin
	glRotatef(90.0f, 0.0f, -1.0f, 0.0f); // Rotate to face outwards
	glColor3f(0.7f, 0.8f, 0.7f); // Same color for symmetry
	drawCylinder(0.3f, 0.05f, 32);
	glPopMatrix();

	// Circle in front of the knee
	glPushMatrix();
	glTranslatef(0.0f, -1.25f, 0.35f); // Adjust the position to align with the knee
	glColor3f(0.0f, 0.5f, 0.2f); // Red color for contrast
	drawCylinder(0.4f, 0.1f, 32); // Radius of 0.2, 32 segments for smoothness
	glPopMatrix();

	// shin cube
	glColor3f(0.0f, 0.0f, 0.9f);
	glTranslatef(0.0f, -1.0f, 0.0f);
	glColor3f(0.5f, 0.0f, 0.9f);
	drawCube(0.5f, 0.5f, 0.5f);

	//knee cube
	glTranslatef(0.0f, -0.25f, 0.0f);
	glRotatef(kneeBendAngle, 1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, -0.5f, 0.0f);
	glColor3f(0.0f, 0.0f, 0.9f);
	drawCube(0.6f, 1.0f, 0.6f);


	// Shin armor
	glPushMatrix();
	glTranslatef(0.0f, -0.5f, 0.0f);
	glColor3f(0.3f, 0.8f, 0.9f);
	drawTrapezoid(0.6f, 0.8f, 1.0f, 0.3f); // Main shin armor
	glPopMatrix();

	// Additional shin armor - angled front
	glPushMatrix();
	glTranslatef(0.0f, -0.5f, 0.3f);
	glRotatef(15.0f, 1.0f, 0.0f, 0.0f);
	glColor3f(0.3f, 0.8f, 0.9f);
	drawTrapezoid(0.5f, 0.7f, 0.8f, 0.2f);
	glPopMatrix();

	// Additional shin armor - angled side
	glPushMatrix();
	glTranslatef(0.3f, -0.5f, 0.0f);
	glRotatef(15.0f, 0.0f, 0.0f, 1.0f);
	glColor3f(0.3f, 0.8f, 0.9f);
	drawTrapezoid(0.5f, 0.7f, 0.8f, 0.2f);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.3f, -0.5f, 0.0f);
	glRotatef(-15.0f, 0.0f, 0.0f, 1.0f);
	glColor3f(0.3f, 0.8f, 0.9f);
	drawTrapezoid(0.5f, 0.7f, 0.8f, 0.2f);
	glPopMatrix();

	glTranslatef(0.0f, -0.75f, 0.0f);
	glRotatef(ankleAngle, 1.0f, 0.0f, 0.0f);
	glColor3f(0.3f, 0.8f, 0.9f);
	drawCube(0.6f, 0.3f, 0.6f);

	// Foot armor
	glPushMatrix();
	glTranslatef(0.0f, -0.15f, 0.5f);
	glColor3f(0.3f, 0.8f, 0.9f);
	drawTrapezoid(0.6f, 0.8f, 0.3f, 0.6f); // Main foot armor
	glPopMatrix();

	// Additional foot armor - angled top
	glPushMatrix();
	glTranslatef(0.0f, -0.1f, 0.5f);
	glRotatef(10.0f, 1.0f, 0.0f, 0.0f);
	glColor3f(0.3f, 0.8f, 0.9f);
	drawTrapezoid(0.5f, 0.7f, 0.3f, 0.2f);
	glPopMatrix();

	glTranslatef(0.0f, -0.25f, 0.5f);
	glColor3f(0.3f, 1.0f, 0.5f);
	drawCube(0.6f, 0.3f, 1.0f);
	glPopMatrix();
}


// Draw the Gundam
void drawGundam()
{

	float leftLegSwing = sin(walkCycle) * 30.0f;
	float rightLegSwing = -sin(walkCycle) * 30.0f;

	float leftKneeBend = fmax(0.0f, sin(walkCycle) * 30.0f);
	float rightKneeBend = fmax(0.0f, -sin(walkCycle) * 30.0f);

	float leftAnkleAngle = sin(walkCycle) * 10.0f;
	float rightAnkleAngle = -sin(walkCycle) * 10.0f;


	glPushMatrix();
	glTranslatef(0.0f, -0.6f, 0.0f);
	drawCenterWaist();
	glPopMatrix();

	glPushMatrix();
	drawLeftWaist(sin(walkCycle) * 10.0f);
	drawRightWaist(-sin(walkCycle) * 20.0f);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.25f, -2.5f, 0.0f);
	drawLeg(leftLegSwing, leftKneeBend, leftAnkleAngle);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.25f, -2.5f, 0.0f);
	drawLeg(rightLegSwing, rightKneeBend, rightAnkleAngle);
	glPopMatrix();

	if (walkCycle > 0) {
		// Shoulder movement (rotates between -45  and 45 )
		if (shoulderUp) {
			leftShoulderAngle = rightLegSwing;
			rightShoulderAngle = leftLegSwing;
			if (leftShoulderAngle >= 45.0f) shoulderUp = false;
		}
		else {
			leftShoulderAngle = rightLegSwing;
			rightShoulderAngle = leftLegSwing;
			if (leftShoulderAngle <= -45.0f) shoulderUp = true;
		}
	}

}


void drawFaceMask() {
	glPushMatrix();

	// Color for the mask
	glColor3f(0.5f, 0.0f, 0.7f); // Purple tone

	// Layer 1 - Bottom Jaw Plate
	glPushMatrix();
	glTranslatef(0.0f, -0.1f, 0.3f);
	drawCuboid(1.0f, 1.25f, 0.1f); // Bottom jaw piece
	glPopMatrix();

	// Layer 2 - Mid Plate
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.4f); // Slightly above the jaw
	drawCuboid(0.9f, 0.2f, 0.1f); // Middle covering
	glPopMatrix();

	// Layer 3 - Top Plate
	glPushMatrix();
	glTranslatef(0.0f, 0.4f, 0.5f); // Topmost layer
	drawCuboid(1.0f, 0.1f, 0.1f); // Slightly curved upper plate
	glPopMatrix();

	// Side Plates - Add angular details
	glPushMatrix();
	glTranslatef(-0.6f, 0.1f, 0.4f); // Left side
	glRotatef(30.0f, 0.0f, 1.0f, 0.0f); // Rotate slightly outward
	drawCuboid(0.3f, 0.7f, 0.05f); // Side shield
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.6f, 0.1f, 0.4f); // Right side
	glRotatef(-30.0f, 0.0f, 1.0f, 0.0f);
	drawCuboid(0.3f, 0.7f, 0.05f);
	glPopMatrix();

	// Eyes or Sensors
	glPushMatrix();
	glColor3f(1.0f, 0.8f, 0.5f); // Light yellow glow
	glTranslatef(0.4f, 0.4f, 0.5f); // Right eye
	drawSphere(0.1f);
	glTranslatef(-0.8f, 0.0f, 0.0f); // Left eye
	drawSphere(0.1f);
	glPopMatrix();

	glPopMatrix();
}

void drawBattleFace() {
	glPushMatrix();

	// Set color for the face
	glColor3f(0.3f, 0.3f, 0.3f); // Dark metallic gray

	// Eyebrow Plates (Angled Plates)
	glPushMatrix();
	glTranslatef(0.4f, 0.5f, 0.5f); // Position above eyes
	glRotatef(15, 0, 0, 1); // Angle for sharpness
	drawCuboid(0.6f, 0.1f, 0.1f);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.4f, 0.5f, 0.5f);
	glRotatef(-15, 0, 0, 1); // Opposite angle
	drawCuboid(0.6f, 0.1f, 0.1f);
	glPopMatrix();

	// Eyes (Narrow and Thin)
	glPushMatrix();
	glColor3f(0.8f, 0.0f, 0.0f); // Red glowing eyes
	glTranslatef(0.3f, 0.3f, 0.5f);
	drawCuboid(0.3f, 0.1f, 0.1f);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.3f, 0.3f, 0.5f);
	drawCuboid(0.3f, 0.1f, 0.1f);
	glPopMatrix();

	glPopMatrix();
}


void drawHeadModel() {
	glPushMatrix();

	// -------------------------
	// Main Head - Cuboid
	// -------------------------
	glPushMatrix();
	glColor3f(0.8f, 0.8f, 0.8f); // Light gray color
	drawCuboid(1.0f, 1.2f, 1.0f); // Head block
	glPopMatrix();


	// -------------------------
	// Antennas - Cones
	// -------------------------
	glPushMatrix();
	glTranslatef(-0.5f, 0.6f, 0.0f); // Left antenna
	glRotatef(45, 0.0f, 0.0f, 1.0f); // Rotate outward
	glColor3f(1.0f, 0.2f, 0.2f); // Red
	drawConeFill(0.05f, 0.7f); // Left antenna
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5f, 0.6f, 0.0f); // Right antenna
	glRotatef(-45, 0.0f, 0.0f, 1.0f); // Rotate outward
	glColor3f(1.0f, 0.2f, 0.2f); // Red
	drawConeFill(0.05f, 0.7f); // Right antenna
	glPopMatrix();

	// -------------------------
	// Ears - Cylinders
	// -------------------------
	glPushMatrix();
	glTranslatef(-0.7f, 0.0f, 0.0f); // Left ear
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f); // Rotate to side
	glColor3f(0.7f, 0.7f, 0.7f); // Gray
	cylinderFill(0.1f, 0.1f, 0.3f); // Left ear
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.7f, 0.0f, 0.0f); // Right ear
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f); // Rotate to side
	glColor3f(0.7f, 0.7f, 0.7f); // Gray
	cylinderFill(0.1f, 0.1f, 0.3f); // Right ear
	glPopMatrix();

	glPushMatrix();
	// Top Helmet 
	glTranslatef(0.0f, 0.25f, 0.0f); // Move upward
	glColor3f(0.7f, 0.7f, 0.7f);
	drawConeFill(1.1f, 1.1f); // Rounded top
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -0.95f); // Move back
	drawFaceMask();		//back of helmet
	glPopMatrix();

	glPushMatrix();
	//glScalef(0.5, 0.5, 0.5);
	glTranslatef(0.0f, -0.3f, 0.0f); // Move back
	drawBattleFace();
	glPopMatrix();

	glPopMatrix();
}


void drawSword() {
	// Blade
	glPushMatrix();
	glColor3f(0.7f, 0.7f, 0.7f); // Silver color for the blade
	glTranslatef(0.0f, 2.0f, 0.0f); // Position the blade relative to the hilt
	drawCube(0.1f, 3.0f, 0.3f); // Long, narrow blade with depth
	glPopMatrix();

	// Guard
	glPushMatrix();
	glColor3f(0.4f, 0.2f, 0.0f); // Brown color for the guard
	glTranslatef(0.0f, 0.8f, 0.0f); // Position the guard below the blade
	drawCube(0.5f, 0.1f, 0.5f); // Wide guard with depth
	glPopMatrix();

	// Handle
	glPushMatrix();
	glColor3f(0.2f, 0.2f, 0.2f); // Dark gray for the handle
	glTranslatef(0.0f, 0.4f, 0.0f); // Position the handle below the guard
	drawCube(0.3f, 0.7f, 0.4f); // Thick, short handle
	glPopMatrix();

	// Pommel
	glPushMatrix();
	glColor3f(0.3f, 0.3f, 0.3f); // Gray for the pommel
	glTranslatef(0.0f, 0.1f, 0.0f); // Position the pommel at the bottom of the handle
	drawCube(0.25f, 0.1f, 0.25f); // Small cube as pommel
	glPopMatrix();
}



void drawUpperBody() {
	glPushMatrix();

	// -------------------------
	// Chest Block
	// -------------------------
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 1.0f); // Light gray
	drawCuboid(2.0f, 1.5f, 1.5f); // Chest
	glPopMatrix();

	//armor
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.78f);
	glRotatef(-5.0f, 1.0f, 0.0f, 0.0f);
	glColor3f(1.0f, 1.0f, 0.9f);
	drawTrapezoid(2.0f, 2.0f, 1.75f, 0.1f);
	glPopMatrix();

	// -------------------------
	// Shoulder Blocks
	// -------------------------
	glPushMatrix();
	glTranslatef(-1.25f, 0.6f, 0.0f); // Left Shoulder
	glColor3f(0.6f, 0.6f, 0.6f); // Gray
	drawCuboid(0.8f, 1.0f, 0.8f);
	glPopMatrix();

	//shoulder guard right
	glPushMatrix();
	glTranslatef(1.7f, 0.4f, 0.71f);
	glRotatef(-55.0f, 0.0f, 0.0f, 1.0f);
	glRotatef(-15.0f, 1.0f, 0.0f, 0.0f);
	glColor3f(0.3f, 0.8f, 0.9f);
	drawTrapezoid(2.0f, 2.0f, 0.75f, 0.2f);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.7f, 0.4f, -0.71f);
	glRotatef(-55.0f, 0.0f, 0.0f, 1.0f);
	glRotatef(15.0f, 1.0f, 0.0f, 0.0f);
	glColor3f(0.3f, 0.8f, 0.9f);
	drawTrapezoid(2.0f, 2.0f, 0.75f, 0.2f);
	glPopMatrix();

	//plates
	glPushMatrix();
	glTranslatef(1.75f, 1.0f, 0.0f);
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	glRotatef(-35.0f, 1.0f, 0.0f, 0.0f);
	glColor3f(1.3f, 0.8f, 0.9f);
	drawTrapezoid(1.0f, 1.1f, 1.0f, 0.1f);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2.0f, 0.5f, 0.0f);
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	glRotatef(-35.0f, 1.0f, 0.0f, 0.0f);
	glColor3f(1.3f, 0.8f, 0.9f);
	drawTrapezoid(1.0f, 1.1f, 1.0f, 0.1f);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2.25f, 0.0f, 0.0f);
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	glRotatef(-35.0f, 1.0f, 0.0f, 0.0f);
	glColor3f(1.3f, 0.8f, 0.9f);
	drawTrapezoid(1.0f, 1.1f, 1.0f, 0.1f);
	glPopMatrix();

	//top plate
	glPushMatrix();
	glTranslatef(1.1f, 1.2f, 0.0f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
	glRotatef(35.0f, 1.0f, 0.0f, 0.0f);
	glColor3f(1.3f, 1.0f, 1.0f);
	drawTrapezoid(1.5f, 01.0f, 0.75f, 0.2f);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.25f, 0.6f, 0.0f); // Right Shoulder
	glColor3f(0.6f, 0.6f, 0.6f); // Gray
	drawCuboid(0.8f, 1.0f, 0.8f);
	glPopMatrix();


	// Shoulder Guard Left
	glPushMatrix();
	glTranslatef(-1.7f, 0.4f, 0.71f); // Mirrored X-axis
	glRotatef(55.0f, 0.0f, 0.0f, 1.0f); // Mirrored Rotation
	glRotatef(-15.0f, 1.0f, 0.0f, 0.0f);
	glColor3f(0.3f, 0.8f, 0.9f);
	drawTrapezoid(2.0f, 2.0f, 0.75f, 0.2f);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.7f, 0.4f, -0.71f); // Mirrored X-axis
	glRotatef(55.0f, 0.0f, 0.0f, 1.0f); // Mirrored Rotation
	glRotatef(15.0f, 1.0f, 0.0f, 0.0f);
	glColor3f(0.3f, 0.8f, 0.9f);
	drawTrapezoid(2.0f, 2.0f, 0.75f, 0.2f);
	glPopMatrix();

	// Plates (Mirrored)
	glPushMatrix();
	glTranslatef(-1.75f, 1.0f, 0.0f); // Mirrored X-axis
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	glRotatef(35.0f, 1.0f, 0.0f, 0.0f);
	glColor3f(1.3f, 0.8f, 0.9f);
	drawTrapezoid(1.0f, 1.1f, 1.0f, 0.1f);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2.0f, 0.5f, 0.0f); // Mirrored X-axis
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	glRotatef(35.0f, 1.0f, 0.0f, 0.0f);
	glColor3f(1.3f, 0.8f, 0.9f);
	drawTrapezoid(1.0f, 1.1f, 1.0f, 0.1f);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2.25f, 0.0f, 0.0f); // Mirrored X-axis
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	glRotatef(35.0f, 1.0f, 0.0f, 0.0f);
	glColor3f(1.3f, 0.8f, 0.9f);
	drawTrapezoid(1.0f, 1.1f, 1.0f, 0.1f);
	glPopMatrix();

	// Top Plate (Mirrored)
	glPushMatrix();
	glTranslatef(-1.1f, 1.2f, 0.0f); // Mirrored X-axis
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
	glRotatef(35.0f, 1.0f, 0.0f, 0.0f);
	glColor3f(1.3f, 1.0f, 1.0f);
	drawTrapezoid(1.5f, 1.0f, 0.75f, 0.2f);
	glPopMatrix();


	// -------------------------
	// Arms
	// -------------------------

	// Left Arm
	glPushMatrix();

	// Shoulder position and rotation
	glTranslatef(-1.5f, -0.6f, 0.0f);
	glRotatef(leftShoulderAngle, 1.0f, 0.0f, 0.0f); // Shoulder rotation
	drawCuboid(0.8f, 1.2f, 0.8f); // Upper Arm

	// Elbow position and rotation
	glTranslatef(0.0f, -1.0f, 0.0f);
	glRotatef(leftElbowAngle, 1.0f, 0.0f, 0.0f); // Elbow rotation
	drawCuboid(0.7f, 1.0f, 0.7f); // Lower Arm

	// Wrist position and rotation
	glTranslatef(0.0f, -0.7f, 0.0f);
	glRotatef(leftWristAngle, 0.0f, 0.0f, 1.0f); // Wrist rotation
	drawCuboid(0.4f, 0.4f, 0.55f); // Hand

	// Fingers
	glPushMatrix();
	glColor3f(1.0, 0, 0);
	glTranslatef(-0.1f, -0.4f, -0.2f); // Base position for fingers

	for (int i = 0; i < 4; i++) { // Draw 4 fingers
		drawCuboid(0.1f, 0.4f, 0.1f);
		glTranslatef(0.0f, 0.0f, 0.15f); // Move to next finger position
	}
	glPopMatrix();

	// Thumb
	glTranslatef(0.2f, -0.3f, 0.2f); // Position thumb
	drawCuboid(0.1f, 0.2f, 0.2f); // Draw thumb

	glPopMatrix(); // End arm drawing

	if (swordEquiped == true) {
		//// Right Arm
		//glPushMatrix();
		//glTranslatef(1.5f, -0.6f, 0.0f); // Position right arm
		//drawCuboid(0.8f, 1.2f, 0.8f); // Upper Arm
		//glTranslatef(0.0f, -1.0f, 0.0f); // Lower Arm
		//drawCuboid(0.7f, 1.0f, 0.7f); // Lower Arm
		//glTranslatef(0.0f, -0.7f, 0.0f); // Hand
		//drawCuboid(0.4f, 0.4f, 0.55f); // Hand


		glPushMatrix();

		// Shoulder
		glTranslatef(1.5f, -0.6f, 0.0f);
		glRotatef(rightShoulderAngle, 1.0f, 0.0f, 0.0f);
		drawCuboid(0.8f, 1.2f, 0.8f); // Upper Arm

		// Elbow
		glTranslatef(0.0f, -1.0f, 0.0f);
		glRotatef(rightElbowAngle, 1.0f, 0.0f, 0.0f);
		drawCuboid(0.7f, 1.0f, 0.7f); // Lower Arm

		// Wrist
		glTranslatef(0.0f, -0.7f, 0.0f);
		glRotatef(rightWristAngle, 0.0f, 0.0f, 1.0f);
		drawCuboid(0.4f, 0.4f, 0.55f); // Hand



		// Sword in Hand
		glPushMatrix();
		glTranslatef(0.0f, -0.2f, -0.3f); // Adjust position relative to hand
		glRotatef(90.0f, 1.0f, 0.0f, 0.0f); // Rotate to make the sword face forward
		drawSword();
		glPopMatrix();

		glPopMatrix();
	}
	else if (swordEquiped == false)
	{
		glPushMatrix();

		// Shoulder
		glTranslatef(1.5f, -0.6f, 0.0f);
		glRotatef(rightShoulderAngle, 1.0f, 0.0f, 0.0f);
		drawCuboid(0.8f, 1.2f, 0.8f); // Upper Arm

		// Elbow
		glTranslatef(0.0f, -1.0f, 0.0f);
		glRotatef(rightElbowAngle, 1.0f, 0.0f, 0.0f);
		drawCuboid(0.7f, 1.0f, 0.7f); // Lower Arm

		// Wrist
		glTranslatef(0.0f, -0.7f, 0.0f);
		glRotatef(rightWristAngle, 0.0f, 0.0f, 1.0f);
		drawCuboid(0.4f, 0.4f, 0.55f); // Hand

		// Fingers
		glPushMatrix();
		glColor3f(1.0, 1, 1);
		glTranslatef(0.1f, -0.4f, -0.2f);
		for (int i = 0; i < 4; i++) { // Draw 4 fingers
			drawCuboid(0.1f, 0.4f, 0.1f);
			glTranslatef(0.0f, 0.0f, 0.15f); // Move to next finger position
		}
		glPopMatrix();

		// Thumb
		glTranslatef(-0.2f, -0.3f, 0.2f);
		drawCuboid(0.1f, 0.2f, 0.2f); // Thumb

		glPopMatrix();

		glPushMatrix();

		glRotatef(180.0f, 1.0f, 0.0f, 0.0f); // Rotate to make the sword face forward
		glRotatef(45.0f, 0.0f, 0.0f, 1.0f); // Rotate to make the sword face forward
		glTranslatef(0.5f, -1.5f, 1.0f); // Adjust position relative to hand
		drawSword();
		glPopMatrix();
	}



	// -------------------------
	// Torso Details
	// -------------------------

	glPushMatrix();
	glTranslatef(0.0f, -2.05f, 0.0f); // Waist
	glColor3f(1.0f, 0.4f, 0.4f); // Light gray
	drawCuboid(1.1f, 0.5f, 0.5f); // Waist
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, -1.0f, 0.0f); // Position mid-torso
	glColor3f(1.0f, 0.5f, 0.5f); // Dark gray
	drawCuboid(1.5f, 0.5f, 0.8f); // Mid-section
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, -1.35f, 0.0f); // Position mid-torso
	glColor3f(1.0f, 0.6f, 0.6f); // Dark gray
	drawCuboid(1.3f, 0.5f, 0.7f); // Mid-section
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, -1.7f, 0.0f); // Waist
	glColor3f(1.0f, 0.7f, 0.7f); // Light gray
	drawCuboid(1.2f, 0.5f, 0.6f); // Waist
	glPopMatrix();


	//rib
	glPushMatrix();
	glTranslatef(0.0f, -1.5f, 0.25f); // Position mid-torso
	glColor3f(1.0f, 0.5f, 1.0f); // Dark gray
	glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
	glRotatef(-25.0f, 0.0f, 1.0f, 0.0f);
	drawCuboid(3.0f, 0.5f, 0.5f); // Mid-section
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, -1.5f, -0.25f); // Position mid-torso
	glColor3f(1.0f, 0.5f, 1.0f); // Dark gray
	glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
	glRotatef(-25.0f, 0.0f, 1.0f, 0.0f);
	drawCuboid(1.8f, 0.5f, 0.355f); // Mid-section
	glPopMatrix();


	//spenders
	glPushMatrix();
	glTranslatef(0.85f, -1.5f, 0.05f); // Position mid-torso
	glRotatef(-75.0f, 1.0f, 0.0f, 0.0f);
	glColor3f(1.0f, 0.0f, 0.0f); // Dark gray
	drawCylinder(0.05, 1.5, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.8f, -1.5f, 0.25f); // Position mid-torso
	glRotatef(75.0f, 1.0f, 0.0f, 0.0f);
	glColor3f(1.0f, 0.0f, 0.0f); // Dark gray
	drawCylinder(0.05, 1.5, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.85f, -1.5f, 0.05f); // Position mid-torso
	glRotatef(-75.0f, 1.0f, 0.0f, 0.0f);
	glColor3f(1.0f, 0.0f, 0.0f); // Dark gray
	drawCylinder(0.05, 1.5, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.8f, -1.5f, 0.25f); // Position mid-torso
	glRotatef(75.0f, 1.0f, 0.0f, 0.0f);
	glColor3f(1.0f, 0.0f, 0.0f); // Dark gray
	drawCylinder(0.05, 1.5, 10);
	glPopMatrix();

	glPopMatrix();
}

void updateHeadRotation() {
	if (rotate == true)
	{
		// Check direction and update rotation
		if (rotateRight) {
			headRotation += 0.02f; // Rotate clockwise
			if (headRotation >= 45.0f) { // Limit to 45 degrees
				headRotation = 45.0f;   // Clamp value
				rotateRight = false;   // Change direction
			}
		}
		else {
			headRotation -= 0.02f; // Rotate counter-clockwise
			if (headRotation <= -45.0f) { // Limit to -45 degrees
				headRotation = -45.0f;   // Clamp value
				rotateRight = true;     // Change direction
			}
		}
	}

}



void fullrobot() {

	glPushMatrix();

	// Translate to the position of the head
	glTranslatef(0, 4.5, 0);

	// Apply rotation around the Y-axis
	glRotatef(headRotation, 0.0f, 1.0f, 0.0f);

	// Draw the head
	drawHeadModel();

	glPopMatrix();


	glPushMatrix();
	glTranslatef(0, 2.75, 0);
	glScalef(2.25, 1.25, 1.25);
	drawUpperBody();
	glPopMatrix();


	glPushMatrix();

	glScalef(1.25, 1.25, 1.25);
	drawGundam();
	glPopMatrix();

}

void updateMovement()
{
	walkSpeed = fminf(fmaxf(walkSpeed, 0.0f), maxSpeed);
	walkCycle += walkSpeed * 10.0f;
}

void display()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);


	//projection(); //projetion

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, 1.0, 1.0, 100.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(tx, ty, tz);
	glRotatef(rotationX, 1.0, 0.0, 0.0);
	glRotatef(rotationY, 0.0, 1.0, 0.0);

	// Set the camera using gluLookAt
	gluLookAt(
		cameraX, cameraY, cameraZ,  // Camera position
		lookAtX, lookAtY, lookAtZ, // Look-at position
		upX, upY, upZ              // Up direction
	);


	r += 0.05;		// rotation speed
	
	glPushMatrix();
	glRotatef(r, 0.0, 1.0, 0.0);	// to view the model in 360

	//glScalef(0.5, 0.5, 0.5);
	updateMovement();
	updateHeadRotation();
	fullrobot();


	glPopMatrix();
}


//--------------------------------------------------------------------

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpfnWndProc = WindowProcedure;
	wc.lpszClassName = WINDOW_TITLE;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&wc)) return false;

	HWND hWnd = CreateWindow(WINDOW_TITLE, WINDOW_TITLE, WS_OVERLAPPEDWINDOW,
		10, 10, 800, 800,
		NULL, NULL, wc.hInstance, NULL);

	//--------------------------------
	//	Initialize window for OpenGL
	//--------------------------------

	HDC hdc = GetDC(hWnd);

	//	initialize pixel format for the window
	initPixelFormat(hdc);

	//	get an openGL context
	HGLRC hglrc = wglCreateContext(hdc);

	//	make context current
	if (!wglMakeCurrent(hdc, hglrc)) return false;

	//--------------------------------
	//	End initialization
	//--------------------------------

	ShowWindow(hWnd, nCmdShow);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	//projection();

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		display();

		SwapBuffers(hdc);
	}

	UnregisterClass(WINDOW_TITLE, wc.hInstance);

	return true;
}
//--------------------------------------------------------------------