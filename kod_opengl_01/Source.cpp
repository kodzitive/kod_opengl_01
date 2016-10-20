// Author : Anh H. T. Nguyen
// High Performance Computing / Computer Graphics - OpenGL
// 08 September 2016


/////////////////////////////////////////

// 1. A line is drawn to link two end points clicked by the mouse : Done
// 2. The circles on the line are drawn as required : Done
// - Center of each circle on the line : Done
// - No overlaps : Done
// - No gaps : Done
// 3. Colors are used as required : Done
// 4. key + and - work as required : Done
// 5. Animation on the triangle works as required : Done
// - A triangle is drawing as required : Done
// - The animation works as required : Done
// 6. Animation on a half circle works as required : Done
// - A half circle is drown as required : Done
// - The animation works as required : Done
// 7. The user is able to choose two new points again to restart with above functions : Done
// 8. A key (Esc) is used to exit the program : Done
// 9. The program has good layout and is documented properly and good structure and efficient coding : Done

///////////////////////////////////////////

// A bief instruction on how to use your program:
// 1) Compile this source code file or run the precompiled file
// 2) Click on the white screen two times as two differents points.
// They will be connected automatically by a white line and the circles will appear on the lines
// 3) You can click at two different points and repeat step 2
// 4) You can press 'a' to activate triangle and circle animation
// - If you press 'a', wait until the animation complete before you can do anything else
// 5) You can repeat step 3 or step 4, or press 'c' for half circle animation
// - If you press 'c', wait until the animation complete before you can do anything else
// 6. You can repeat step 3 / 4 / 5 or press 'Esc', or right click to exit

// Thank you!

/////////////////////////////////////////

#include <GL/glut.h>     

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <windows.h>
//#include <unistd.h> //For Linux

// usleep function for Windows

void usleep(__int64 usec)
{
	HANDLE timer;
	LARGE_INTEGER ft;

	ft.QuadPart = -(10 * usec);

	timer = CreateWaitableTimer(NULL, TRUE, NULL);
	SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
	WaitForSingleObject(timer, INFINITE);
	CloseHandle(timer);
}


////
#include <iostream>
using std::cin;
using std::cout;
////

//////////////////////////////////
float PI = 3.14159265358979323846;
int R = 10; // Default R 

GLsizei winWidth = 640, winHeight = 480;  // Window size initilization 
GLint endPtCtr = 0;                       // Line endpoint counter initilization 
GLint start = 0;

//////////////////////////////////

class screenPoints
{
public:
	GLint x, y;
};

static screenPoints point00, point01, point02;
// Two points from user input by mouse and the third point for Part B 2

void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);   // Set display-window color to white

	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, 200.0, 0.0, 150.0);
}

void windowResize(int widthResize, int heightResize)
{
	/*  Reset viewport and projection parameters  */
	glViewport(0, 0, widthResize, heightResize);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, GLdouble(widthResize), 0.0, GLdouble(heightResize));

	/*  Reset display-window size parameters.  */
	winWidth = widthResize;
	winHeight = heightResize;
}


void setPixel(float xcoordinate, float ycoordinate)
{
	glBegin(GL_POINTS);
	glVertex2i(xcoordinate, ycoordinate);
	glEnd();
	glFlush();
}
///////////////////////////////////////////////////////////////////////////////////

// Core function of the program to draw the filled circle using fans of triangles
void drawCircleFanFilled(GLfloat x, GLfloat y, GLfloat radius)
{

	int i;
	int noOfTriangle = 20; // Number of triangles 

	GLfloat PI_2 = 2.0f * PI;

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x, y); // center of circle
	for (i = 0; i <= noOfTriangle; i++) {
		glVertex2f(
			x + (radius * cos(i *  PI_2 / noOfTriangle)),
			y + (radius * sin(i * PI_2 / noOfTriangle))
		);
	}
	glEnd();
}


// Draw line with circles for question 1 and 2
void drawLineCircles(screenPoints point00, screenPoints point01)
{

	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 1.0);
	// Draw the base line in white color
	glVertex2i(point00.x, point00.y);
	glVertex2i(point01.x, point01.y);

	// Calculate delta x and delta y
	int dx = (point01.x - point00.x);
	int dy = (point01.y - point00.y);

	int steps, k;
	float xIncrement, yIncrement, x = point00.x, y = point00.y;

	// Decide dx or dy will be steps
	if (abs(dx)>abs(dy))
		steps = abs(dx);
	else
		steps = abs(dy);


	setPixel(round(x), round(y));

	// lenght of the line
	int d = sqrt((dx*dx) + (dy*dy));

	// Number of circles
	int stepsCircle = round(d / (2 * R));

	// Increment
	xIncrement = GLfloat(dx) / d * (2 * R);
	yIncrement = GLfloat(dy) / d * (2 * R);

	int colour = 0; // Initial colour is red
	for (k = 0; k <= stepsCircle; k++)
	{
		setPixel(round(x), round(y));

		// Cycle through different colours
		if (colour == 0)
			glColor3f(1.0, 0.0, 0.0);
		else if (colour == 1)
			glColor3f(0.0, 0.0, 1.0);
		else if (colour == 2)
			glColor3f(0.0, 1.0, 0.0);

		// Draw the circle on the line at x, y
		drawCircleFanFilled(x, y, R);

		// Next point to draw the circle
		x += xIncrement;
		y += yIncrement;


		colour = colour + 1;
		if (colour >= 3)
			colour = 0;
	}

	glEnd();
}

///////////////////////////////////////////////////////////////////

// Question 3

// Draw the three sides of the triangles
void drawTriangle(screenPoints point00, screenPoints point01)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_LINES);
	glColor3f(0.0, 0.0, 1.0);
	point02.x = point01.x;
	point02.y = point00.y;

	// Draw three sides of the triangle
	glVertex2i(point00.x, point00.y);
	glVertex2i(point01.x, point01.y);

	glVertex2i(point00.x, point00.y);
	glVertex2i(point02.x, point02.y);

	glVertex2i(point01.x, point01.y);
	glVertex2i(point02.x, point02.y);
	glEnd();

}

// Animate three sides of the triangle and the moving circle
void animateTriangle(screenPoints pointX, screenPoints pointY)
{

	int dx = (pointY.x - pointX.x);
	int dy = (pointY.y - pointX.y);
	int steps, k;
	float xIncrement, yIncrement, x = pointX.x, y = pointX.y;


	if (abs(dx)>abs(dy))
		steps = abs(dx);
	else
		steps = abs(dy);
	setPixel(round(x), round(y));
	xIncrement = GLfloat(dx) / GLfloat(steps);
	yIncrement = GLfloat(dy) / GLfloat(steps);

	// Loop, redraw triangle and circle, delay with usleep()
	for (k = 0; k <= steps; k++)
	{
		setPixel(round(x), round(y));
		//Draw the 3 sides of triangle
		drawTriangle(point00, point01);
		glColor3f(1.0, 0.0, 0.0);
		// Draw the circle
		drawCircleFanFilled(x, y, 10);
		// Delay
		usleep(10000);
		x += xIncrement;
		y += yIncrement;
	}

}

/////////////////////////////////////////////////////////////

// Question 4


// Get the angle of the line, so we can calculate the rotation for the arc
float getAngle(screenPoints p1, screenPoints p2)
{
	return atan2(p1.y - p2.y, p1.x - p2.x);
}

// Draw the Arc
void DrawArc(float cx, float cy, float r, int num_segments)
{
	glBegin(GL_LINE_STRIP);

	float extraTheta = getAngle(point00, point01);
	for (int ii = 0; ii <= num_segments / 2; ii++)
	{
		float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments) + extraTheta;
		//Angle

		float x = r * cosf(theta);//x component 
		float y = r * sinf(theta);//y component 

		glVertex2f(x + cx, y + cy); // Vertices on the arc
	}
	glEnd();
}

// Draw the line as a base before the arc
void drawLine(screenPoints point00, screenPoints point01)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_LINES);
	glColor3f(0.0, 0.0, 0.0);

	glVertex2i(point00.x, point00.y);
	glVertex2i(point01.x, point01.y);

	glEnd();

}


// Animate the circle to run along the line before running along the arc
void animateLineArc(screenPoints pointX, screenPoints pointY)
{

	int dx = (pointY.x - pointX.x);
	int dy = (pointY.y - pointX.y);
	int steps, k;
	float xIncrement, yIncrement, x = pointX.x, y = pointX.y;



	if (abs(dx)>abs(dy))
		steps = abs(dx);
	else
		steps = abs(dy);
	setPixel(round(x), round(y));
	xIncrement = GLfloat(dx) / GLfloat(steps);
	yIncrement = GLfloat(dy) / GLfloat(steps);


	int d = sqrt((dx*dx) + (dy*dy));
	int xMiddle = (pointX.x + pointY.x) / 2;
	int yMiddle = (pointX.y + pointY.y) / 2;
	float cx = xMiddle - d / 2 * 1;
	float cy = yMiddle - d / 2 * 1;

	for (k = 0; k <= steps; k++)
	{
		setPixel(round(x), round(y));
		glColor3f(0.0, 0.0, 0.0);
		// Draw line
		drawLine(pointX, pointY);
		// Draw the arc
		DrawArc(xMiddle, yMiddle, d / 2, d);
		glColor3f(1.0, 0.0, 0.0);
		// Draw the red circle
		drawCircleFanFilled(x, y, 10);
		usleep(10000);
		// Next values for the circle
		x += xIncrement;
		y += yIncrement;
	}

}

// Animate the red circle to move along the arc
void animateArc(screenPoints pointX, screenPoints pointY)
{
	int dx = (pointY.x - pointX.x);
	int dy = (pointY.y - pointX.y);
	int d = sqrt((dx*dx) + (dy*dy));


	float r = d / 2;
	float num_segments = d;

	int xMiddle = (pointX.x + pointY.x) / 2;
	int yMiddle = (pointX.y + pointY.y) / 2;

	float cx = xMiddle;
	float cy = yMiddle;

	// Loop through all vertices on the arc
	for (int ii = 0; ii <= num_segments / 2; ii++)
	{
		float extraTheta = getAngle(pointX, pointY);


		float theta = -2.0f * 3.1415926f * float(ii) / float(num_segments) + extraTheta;
		// Angle 

		float x = r * cosf(theta); //x component 
		float y = r * sinf(theta); //y component 

		setPixel(round(x + cx), round(y + cy));

		// set black color for the line and the arc
		glColor3f(0.0, 0.0, 0.0);
		drawLine(pointX, pointY);
		DrawArc(xMiddle, yMiddle, d / 2, d);
		//set red color for the circle
		glColor3f(1.0, 0.0, 0.0);
		drawCircleFanFilled(x + cx, y + cy, 10);
		// Delay
		usleep(10000);
	}

}

/////////////////////////////////////////////////////////////


// Display Part B question 1 and 2
void partB_01(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	if (start != 0)
	{
		drawLineCircles(point00, point01);

	}
	glFlush();

}


// Display Part B question 3
void partB_02(void)
{

	glClear(GL_COLOR_BUFFER_BIT);

	screenPoints point02;
	point02.x = point01.x;
	point02.y = point00.y;

	animateTriangle(point00, point01);
	animateTriangle(point01, point02);
	animateTriangle(point02, point00);

	glFlush();

}

// Display Part B Question 4
void partB_03(void)
{

	glClear(GL_COLOR_BUFFER_BIT);

	animateLineArc(point00, point01);
	animateArc(point01, point00);

	glFlush();

}

// Mouse input
void mouseAction(GLint button, GLint action, GLint xMouse, GLint yMouse)
{
	start = 1;

	// Mouse capture for question 5
	if (endPtCtr == 0) {
		if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN)
		{

			point00.x = xMouse;
			point00.y = winHeight - yMouse;
			endPtCtr = 1;
		}
		else
			if (button == GLUT_RIGHT_BUTTON)        // Exit
				exit(0);
	}
	else
		if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN)
		{
			glutDisplayFunc(partB_01);
			point01.x = xMouse;
			point01.y = winHeight - yMouse;

			glutPostRedisplay();

			endPtCtr = 0;
		}
		else
			if (button == GLUT_RIGHT_BUTTON)        // Exit
				exit(0);
}

// Keyboard input
void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case '+':
		if (R == 20) {
			printf("Radius range is 0-20\n");
			printf("Try to make it smaller\n");
		}
		else {
			R = R + 1;
			glutPostRedisplay();
		}
		break;
	case '-':
		if (R == 1) {
			printf("Radius range is 0-20\n");
			printf("Try to make it bigger\n");
		}
		else {
			R = R - 1;
			glutPostRedisplay();
		}
		break;

	case 'a':

		glutDisplayFunc(partB_02);
		glutPostRedisplay();

		break;


	case 'c':

		glutDisplayFunc(partB_03);
		glutPostRedisplay();

		break;
		// Exit Esc button for question 6
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}

//////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Anh Nguyen - ID 100863203 - Assignment 1");

	init();
	glutDisplayFunc(partB_01);
	glutReshapeFunc(windowResize);
	glutMouseFunc(mouseAction);
	glutKeyboardFunc(keyboard);

	glutMainLoop();
	return(0);
}
