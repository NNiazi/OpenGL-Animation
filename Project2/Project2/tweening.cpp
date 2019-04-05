// #include loads up library files, the order can matter
// generally load glut.h last

#include "stdafx.h"

#include <stdio.h>
#include <math.h>
#include "glut.h"

// this is an example of creating a global variable called counter
int counter = 0;

int plane[18][2] = {
	{ 20.0, 160.0 },
	{ 60.0, 140.0 },
	{ 200.0, 140.0 },
	{ 210.0, 142.0 },
	{ 212.0, 144.0 },
	{ 214.0, 148.0 },
	{ 215.0, 150.0 },
	{ 215.0, 150.0 },
	{ 215.0, 153.0 },
	{ 215.0, 155.0 },
	{ 214.0, 155.0 },
	{ 213.0, 156.0 },
	{ 212.0, 157.0 },
	{ 211.0, 158.0 },
	{ 190.0, 160.0 },
	{ 80.0,160.0 },
	{ 53.0,205.0 },
	{ 53.0,160.0 }
};

int balloon[18][2] = {
	{ 100.0,100.0 },
	{ 200.0,100.0 },
	{ 200.0,0.0 },
	{ 100.0,0.0 },
	{ 100.0,0.0 },
	{ 100.0,0.0 },
	{ 100.0,0.0 },
	{ 100.0,0.0 },
	{ 100.0,0.0 },
	{ 100.0,0.0 },
	{ 100.0,0.0 },
	{ 100.0,0.0 },
	{ 100.0,0.0 },
	{ 100.0,0.0 },
	{ 100.0,0.0 },
	{ 100.0,0.0 },
	{ 100.0,0.0 },
	{ 100.0,0.0 }
};

int tweenPoly[18][2];
float proportion = 0.0;

void tween(int source[18][2], int destination[18][2], int numPoints, double proportion, int tweenPoly[18][2])
{

	for (int p = 0; p < numPoints; p++)
	{
		// get the source point
		double sourceX = source[p][0];
		double sourceY = source[p][1];

		// get the destination point
		double destinationX = destination[p][0];
		double destinationY = destination[p][1];

		// get the difference between source and destination
		double differenceX = (destinationX - sourceX);
		double differenceY = (destinationY - sourceY);

		// tween point is source position + proportion
		// of distance between source and destination
		double tweenX = sourceX + (differenceX * proportion);
		double tweenY = sourceY + (differenceY * proportion);

		// create point with tween co-ordinates in tween array
		tweenPoly[p][0] = int(tweenX);
		tweenPoly[p][1] = int(tweenY);
	}


}


// this is the initialisation function, called once only
void init() {
	glClearColor(1.0, 0.0, 0.0, 0.0); // set what colour you want the background to be
	glMatrixMode(GL_PROJECTION); // set the matrix mode, we will look at this later
								 // set the projection window size in x an y. Window will then
								 // be mapped to the window created in the main function
	gluOrtho2D(0.0, 600, 0.0, 600.0);

}

void renderSpacedBitmapString(float x, float y, void *font, char *string) {
	char *c;
	int x1 = x;
	for (c = string; *c != '\0'; c++) {
		glRasterPos2f(x1, y);
		glutBitmapCharacter(font, *c);
		x1 = x1 + glutBitmapWidth(font, *c);
	}
}



int frame = 0;

// This is the display function it is called when ever you want to draw something
// all drawing should be called form here
void display() {
	glClear(GL_COLOR_BUFFER_BIT); // clear the screen using the background colour

	if (frame > 0 && frame < 100) {
		glBegin(GL_LINE_LOOP);
		for (int i = 0; i<10; i++)
			glVertex2i(plane[i][0], plane[i][1]); // Morphing W to Arrow.
		glEnd();
	}
	else if (frame > 100 && frame < 200) {
		glBegin(GL_LINE_LOOP);
		for (int i = 0; i<10; i++)
			glVertex2i(balloon[i][0], balloon[i][1]); // Morphing W to Arrow.
		glEnd();
	}
	else if ((frame>200) && (frame<300))
	{
		glColor3f(0.0, 0.0, 1.0);
		tween(plane, balloon, 10, proportion, tweenPoly);

		glBegin(GL_LINE_LOOP);
		for (int i = 0; i<10; i++)
			glVertex2i(tweenPoly[i][0], tweenPoly[i][1]); // Morphing W to Arrow.
		glEnd();

		renderSpacedBitmapString(50, 400, GLUT_BITMAP_HELVETICA_18, "3rd Scene"); // Add text on the scene

		proportion += 0.01;

		if (proportion >1.0) proportion = 0.0;
	}


	else if (frame > 300)
	{
		frame = 0;
		proportion = 0.0;
	}
	frame++;

	glFlush(); // force all drawing to finish
	printf("%s %d\n", "hello ", counter); // print some output to console window
	counter++; // increment the counter
}

// This is the idle function it is called whenever the program is idle
// This has the effect of repeatedly calling the display function
void idle() {
	display();
}

void Timer(int value)
{
	if (value) glutPostRedisplay();
	glutTimerFunc(40, Timer, value);
}

void visibility(int state)
{
	switch (state)
	{
	case GLUT_VISIBLE:
		Timer(1);
		break;
	case GLUT_NOT_VISIBLE:
		Timer(0);
		break;
	default:
		break;
	}
}

// As with many programming languages the main() function
// is the entry point for execution of the program
int main(int argc, char** argv) {
	glutInit(&argc, argv);  //perform the GLUT initialization

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // more initialisation
	glutInitWindowSize(600, 600); // set window position
	glutInitWindowPosition(100, 100); // set window size
	glutCreateWindow("Hello"); // create a display with a given caption for the title bar
	init(); // call init function defined above
	glutVisibilityFunc(visibility);
	glutDisplayFunc(display); // define what function to call to draw
							  // the last function in the program puts the program into infinite loop
							  // effectively the loop repeatedly calls the idle function
	glutMainLoop();

	// this line exits the program
	return 0;
}