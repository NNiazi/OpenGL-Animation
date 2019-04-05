#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "glut.h"

#define RAINSIZE 4000

int frame = 0;
float proportion = 0.0;
float tx = 0;
float xShiftCurtain1 = 0;
float xShiftCurtain2 = 0;
float yShiftCurtain1 = 0;
float yShiftCurtain2 = 0;
float xShiftBoxP1 = 0;
float yShiftBoxP1 = 0;
float xShiftBoxP2 = 0;
float yShiftBoxP2 = 0;
float xShiftBoxP3 = 0;
float yShiftBoxP3 = 0;
float xShiftBoxP4 = 0;
float yShiftBoxP4 = 0;
float xShiftPlane1 = 0;
float yShiftPlane1 = 0;
float xShiftPlaneSide = 0;
float yShiftPlaneSide = 0;
float yRunwayShift = 0;
float yGroundShift = 0;
float yShiftRock = 0;
float yShiftCloud = 0;
float yShiftTree = 0;
float yShiftTreeBush = 0;
float yShiftFrontCloud = 0;
float yShiftFrontPlane = 0;
float rotaAngle2 = 0;
float rotaAngle = 0;
float langle = 0;
float linc = 3;
int mode;
int winWidth = 800, winHeight = 800;


float star[1][1] = {
	{ 0.3 },
};

void drawsnowFlaks(float xc, float yc)
{
	// GL_POLYGON can only draw convex shapes, i.e. shapes that only have internal angles less tha 180 degrees
	// a star is not convex so we have to draw the 5 triangles making up the "arms" separately 
	// and then draw the middle section
	// draws a solid white outline over the polygons
	glColor3f(1, 1, 1);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 10; i++) {
		glVertex2d(xc + star[i][0], yc + star[i][1]);
	}
	glEnd();

}

void circle(double radius, double xc, double yc)
{
	int i;
	double angle = 2 * 3.1415 / 20;	// circle is drawn using 20 line.
	double circle_xy[40][2];

	circle_xy[0][0] = radius + xc;
	circle_xy[0][1] = yc;


	glBegin(GL_LINE_LOOP);
	for (i = 1; i<20; i++)
	{
		circle_xy[i][0] = radius * cos(i *angle) + xc;
		circle_xy[i][1] = radius * sin(i * angle) + yc;

		glVertex2f(circle_xy[i - 1][0], circle_xy[i - 1][1]);
		glVertex2f(circle_xy[i][0], circle_xy[i][1]);

	}
	glEnd();
}

struct drop {
	float x = 400;
	float y = 400;
	float inc = 0.01;
	//float r = 1; float g = 1;  float b = 1;
	float radius = 5;
	float scale = 1.0;
	float rotationAngle = 0;
	float rotationInc = 1;
	float r, g, b, a;
};

drop rain[RAINSIZE];;

void initRain()
{
	//	srand((unsigned)time(&t));
	for (int i = 0; i < RAINSIZE; i++) {
		rain[i].x = rand() % winWidth;
		rain[i].y = rand() % winHeight;
		rain[i].inc = 10 + (float)(rand() % 100) / 1000.0;
		rain[i].r = (float)(rand() % 1000) / 1000.0;
		rain[i].g = (float)(rand() % 1000) / 1000.0;
		rain[i].b = (float)(rand() % 1000) / 1000.0;
		rain[i].a = 0.5;
		//rain[i].radius = (float)(rand() % 15);
		rain[i].scale = (float)(rand() % 20000) / 1000.0;
		rain[i].rotationAngle = (float)(rand() % 3000) / 1000.0;
		rain[i].rotationInc = (float)(rand() % 100) / 1000.0;
		if ((rand() % 100) > 50) {
			rain[i].rotationInc = -rain[i].rotationInc;
		}
	}
}
float rotationAngle = 0;
void drawParticleShape(int i)
{
	glPushMatrix();
	// scale the shape around its center
	glTranslatef(rain[i].x, rain[i].y, 0.0);
	glScalef(rain[i].scale, rain[i].scale, 1);
	glTranslatef(-rain[i].x, -rain[i].y, 0.0);
	// rotate the shape around its center
	glTranslatef(rain[i].x, rain[i].y, 0.0);
	glRotatef(rain[i].rotationAngle, 0, 0, 1);
	glTranslatef(-rain[i].x, -rain[i].y, 0.0);

	glColor4f(rain[i].r, rain[i].g, rain[i].b, rain[i].a);
	drawsnowFlaks(rain[i].x, rain[i].y);
	glPopMatrix();
	rain[i].rotationAngle += rain[i].rotationInc;
}


void drawDrop(int i)
{
	//glColor3f(rain[i].r, rain[i].g, rain[i].b);

	glLineWidth(2);

	drawParticleShape(i);
	rain[i].y -= rain[i].inc;
	if (rain[i].y < 0) {
		rain[i].y = winHeight;
	}
}

void drawRain()
{
	for (int i = 0; i < RAINSIZE; i++)
	{
		drawDrop(i);
	}
}

int window[4][2] = {
	{0, 0},
	{400, 0},
	{220, 154},
	{180, 154}
};

int cockpit[4][2] = {
	{0, 0},
	{400, 0},
	{400, 300},
	{0, 300}
};

int tweenPoly[4][2];

void tween(int source[4][2], int destination[4][2], int numPoints, double proportion, int tweenPoly[4][2])
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

void init(void) {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	/* initialize viewing values  */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//Controls size of stickman
	gluOrtho2D(0.0, 400.0, 0.0, 300.0);

}


void curtainBar() {
	glLineWidth(2.4);
	glBegin(GL_POLYGON);
	glColor3f(0.0, 0.0, 0.0);
	glVertex2d(5.0, 280.0);
	glVertex2d(395.0, 280.0);
	glVertex2d(395.0, 291.0);
	glVertex2d(5.0, 291.0);
	glEnd();
}

void curtain1() {
	glLineWidth(2.0);
	glBegin(GL_POLYGON);
	glColor3f(1.0, 0.0, 0.0); // set colour to red?
	glVertex2d(5.0 + xShiftCurtain1, 5.0);
	glVertex2d(197.0 + xShiftCurtain1 + yShiftCurtain1, 5.0);
	glVertex2d(197.0 + xShiftCurtain1, 280.0);
	glVertex2d(5.0 + xShiftCurtain1, 280.0);
	glEnd();
}

void curtain2() {
	glLineWidth(2.0);
	glBegin(GL_POLYGON);
	glColor3f(1.0, 0.0, 0.0); // set colour to red
	glVertex2d(194.0 + xShiftCurtain2 + yShiftCurtain2, 5.0);
	glVertex2d(395.0 + xShiftCurtain2, 5.0);
	glVertex2d(395.0 + xShiftCurtain2, 280.0);
	glVertex2d(194.0 + xShiftCurtain2, 280.0);
	glEnd();
}

void runway() {
	glBegin(GL_POLYGON);
	glColor3f(0.0, 0.0, 0.0); // set colour to black
	glVertex2d(160, 0.0 + yShiftBoxP1);
	glVertex2d(240, 0.0 + yShiftBoxP2);
	glVertex2d(240, 1300.0 + yShiftBoxP3);
	glVertex2d(160, 1300.0 + yShiftBoxP4);
	glEnd();
}

void runwayStrips1() {
	glLineWidth(2.4);
	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 1.0); // set colour to black
	glVertex2d(200, 10.0 + yShiftBoxP1);
	glVertex2d(200, 35.0 + yShiftBoxP1);
	glEnd();
	glLineWidth(2.4);
	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 1.0); // set colour to black
	glVertex2d(200, 50.0 + yShiftBoxP1);
	glVertex2d(200, 85.0 + yShiftBoxP1);
	glEnd();
	glLineWidth(2.4);
	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 1.0); // set colour to black
	glVertex2d(200, 105.0 + yShiftBoxP1);
	glVertex2d(200, 135.0 + yShiftBoxP1);
	glEnd();
	glLineWidth(2.4);
	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 1.0); // set colour to black
	glVertex2d(200, 155.0 + yShiftBoxP1);
	glVertex2d(200, 185.0 + yShiftBoxP1);
	glEnd();
	glLineWidth(2.4);
	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 1.0); // set colour to black
	glVertex2d(200, 205.0 + yShiftBoxP1);
	glVertex2d(200, 235.0 + yShiftBoxP1);
	glEnd();
	glLineWidth(2.4);
	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 1.0); // set colour to black
	glVertex2d(200, 255.0 + yShiftBoxP1);
	glVertex2d(200, 285.0 + yShiftBoxP1);
	glEnd();
	glLineWidth(2.4);
	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 1.0); // set colour to black
	glVertex2d(200, 305.0 + yShiftBoxP1);
	glVertex2d(200, 335.0 + yShiftBoxP1);
	glEnd();
	glLineWidth(2.4);
	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 1.0); // set colour to black
	glVertex2d(200, 355.0 + yShiftBoxP1);
	glVertex2d(200, 385.0 + yShiftBoxP1);
	glEnd();
	glLineWidth(2.4);
	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 1.0); // set colour to black
	glVertex2d(200, 405.0 + yShiftBoxP1);
	glVertex2d(200, 435.0 + yShiftBoxP1);
	glEnd();
	glLineWidth(2.4);
	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 1.0); // set colour to black
	glVertex2d(200, 455.0 + yShiftBoxP1);
	glVertex2d(200, 485.0 + yShiftBoxP1);
	glEnd();
	glLineWidth(2.4);
	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 1.0); // set colour to black
	glVertex2d(200, 505.0 + yShiftBoxP1);
	glVertex2d(200, 535.0 + yShiftBoxP1);
	glEnd();
	glLineWidth(2.4);
	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 1.0); // set colour to black
	glVertex2d(200, 555.0 + yShiftBoxP1);
	glVertex2d(200, 585.0 + yShiftBoxP1);
	glEnd();
	glLineWidth(2.4);
	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 1.0); // set colour to black
	glVertex2d(200, 605.0 + yShiftBoxP1);
	glVertex2d(200, 635.0 + yShiftBoxP1);
	glEnd();
	glLineWidth(2.4);
	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 1.0); // set colour to black
	glVertex2d(200, 655.0 + yShiftBoxP1);
	glVertex2d(200, 685.0 + yShiftBoxP1);
	glEnd();
	glLineWidth(2.4);
	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 1.0); // set colour to black
	glVertex2d(200, 705.0 + yShiftBoxP1);
	glVertex2d(200, 735.0 + yShiftBoxP1);
	glEnd();
	glLineWidth(2.4);
	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 1.0); // set colour to black
	glVertex2d(200, 755.0 + yShiftBoxP1);
	glVertex2d(200, 785.0 + yShiftBoxP1);
	glEnd();
	glLineWidth(2.4);
	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 1.0); // set colour to black
	glVertex2d(200, 805.0 + yShiftBoxP1);
	glVertex2d(200, 835.0 + yShiftBoxP1);
	glEnd();
	glLineWidth(2.4);
	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 1.0); // set colour to black
	glVertex2d(200, 855.0 + yShiftBoxP1);
	glVertex2d(200, 885.0 + yShiftBoxP1);
	glEnd();
	glLineWidth(2.4);
	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 1.0); // set colour to black
	glVertex2d(200, 905.0 + yShiftBoxP1);
	glVertex2d(200, 934.0 + yShiftBoxP1);
	glEnd();
	glLineWidth(2.4);
	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 1.0); // set colour to black
	glVertex2d(200, 955.0 + yShiftBoxP1);
	glVertex2d(200, 985.0 + yShiftBoxP1);
	glEnd();
	glLineWidth(2.4);
	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 1.0); // set colour to black
	glVertex2d(200, 1005.0 + yShiftBoxP1);
	glVertex2d(200, 1035.0 + yShiftBoxP1);
	glEnd();
	glLineWidth(2.4);
	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 1.0); // set colour to black
	glVertex2d(200, 1055.0 + yShiftBoxP1);
	glVertex2d(200, 1085.0 + yShiftBoxP1);
	glEnd();
	glLineWidth(2.4);
	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 1.0); // set colour to black
	glVertex2d(200, 1105.0 + yShiftBoxP1);
	glVertex2d(200, 1135.0 + yShiftBoxP1);
	glEnd();
	glLineWidth(2.4);
	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 1.0); // set colour to black
	glVertex2d(200, 1155.0 + yShiftBoxP1);
	glVertex2d(200, 1185.0 + yShiftBoxP1);
	glEnd();
	glLineWidth(2.4);
	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 1.0); // set colour to black
	glVertex2d(200, 1205.0 + yShiftBoxP1);
	glVertex2d(200, 1235.0 + yShiftBoxP1);
	glEnd();
	glLineWidth(2.4);
	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 1.0); // set colour to black
	glVertex2d(200, 1255.0 + yShiftBoxP1);
	glVertex2d(200, 1285.0 + yShiftBoxP1);
	glEnd();
	glLineWidth(2.4);
	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 1.0); // set colour to black
	glVertex2d(200, 1305.0 + yShiftBoxP1);
	glVertex2d(200, 1335.0 + yShiftBoxP1);
	glEnd();
	glLineWidth(2.4);
	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 1.0); // set colour to black
	glVertex2d(200, 1355.0 + yShiftBoxP1);
	glVertex2d(200, 1385.0 + yShiftBoxP1);
	glEnd();
}

void rock1() {
	glBegin(GL_POLYGON);
	glColor3ub(139, 69, 19); // set colour to WHITE
	glVertex2d(60, 24.0 + yShiftRock);
	glVertex2d(85, 24.0 + yShiftRock);
	glVertex2d(85, 45.0 + yShiftRock);
	glVertex2d(71, 53.0 + yShiftRock);
	glVertex2d(64, 35.0 + yShiftRock);
	glEnd();
}

void rock2() {
	glBegin(GL_POLYGON);
	glColor3ub(139, 69, 19); // set colour to WHITE
	glVertex2d(120, 75.0 + yShiftRock);
	glVertex2d(155, 75.0 + yShiftRock);
	glVertex2d(155, 95.0 + yShiftRock);
	glVertex2d(136, 105.0 + yShiftRock);
	glVertex2d(130, 100.0 + yShiftRock);
	glVertex2d(126, 82.0 + yShiftRock);
	glEnd();
}


void planeBody() {
	glBegin(GL_POLYGON);
	glColor3ub(169, 169, 169); // set colour to WHITE
	glVertex2d(200.0, 10.0);
	glVertex2d(220.0, 35.0);
	glVertex2d(220.0, 150.0);
	glVertex2d(213.0, 157.0);
	glVertex2d(205.0, 159.0);
	glVertex2d(195.0, 159.0);
	glVertex2d(189.0, 157.0);
	glVertex2d(180.0, 150.0);
	glVertex2d(180.0, 35.0);
	glEnd();
}

void planeWing1() {
	glBegin(GL_TRIANGLES);
	glColor3ub(169, 169, 169); // set colour to WHITE
	glVertex2d(180.0, 85.0);
	glVertex2d(180.0, 120.0);
	glVertex2d(70.0, 85.0);
	glEnd();
}

void planeWing2() {
	glBegin(GL_TRIANGLES);
	glColor3ub(169, 169, 169); // set colour to WHITE
	glVertex2d(220.0, 85.0);
	glVertex2d(220.0, 120.0);
	glVertex2d(330.0, 85.0);
	glEnd();
}

void planeWing3() {
	glBegin(GL_TRIANGLES);
	glColor3ub(169, 169, 169); // set colour to WHITE
	glVertex2d(220.0, 35.0);
	glVertex2d(220.0, 50.0);
	glVertex2d(255.0, 35.0);
	glEnd();
}

void planeWing4() {
	glBegin(GL_TRIANGLES);
	glColor3ub(169, 169, 169); // set colour to WHITE
	glVertex2d(180.0, 35.0);
	glVertex2d(180.0, 50.0);
	glVertex2d(145.0, 35.0);
	glEnd();
}

void engines1() {
	glBegin(GL_POLYGON);
	glColor3ub(105, 105, 105); // set colour to WHITE
	glVertex2d(140.0, 85.0);
	glVertex2d(160.0, 85.0);
	glVertex2d(160.0, 120.0);
	glVertex2d(140.0, 120.0);
	glEnd();
}

void engines2() {
	glBegin(GL_POLYGON);
	glColor3ub(105, 105, 105); // set colour to WHITE
	glVertex2d(240.0, 85.0);
	glVertex2d(260.0, 85.0);
	glVertex2d(260.0, 120.0);
	glVertex2d(240.0, 120.0);
	glEnd();
}

void ground() {
	glBegin(GL_POLYGON);
	glColor3ub(124, 252, 0); // set colour to WHITE
	glVertex2d(1.0, 1.0 + yGroundShift);
	glVertex2d(1000.0, 1.0 + yGroundShift);
	glVertex2d(1000.0, 125.0 + yGroundShift);
	glVertex2d(1.0, 125.0 + yGroundShift);
	glEnd();
}

void runway2() {
	glBegin(GL_POLYGON);
	glColor3ub(1, 1, 1); // set colour to WHITE
	glVertex2d(0.0, 125.0 + yRunwayShift);
	glVertex2d(1000.0, 125.0 + yRunwayShift);
	glVertex2d(1000.0, 131.0 + yRunwayShift);
	glVertex2d(0.0, 131.0 + yRunwayShift);
	glEnd();
}

void planeSideBody() {
	glBegin(GL_POLYGON);
	glColor3ub(169, 169, 169); // set colour to WHITE
	glVertex2d(20.0 + xShiftPlaneSide, 160.0 + yShiftPlaneSide);
	glVertex2d(60.0 + xShiftPlaneSide, 140.0 + yShiftPlaneSide);
	glVertex2d(200.0 + xShiftPlaneSide, 140.0 + yShiftPlaneSide);
	glVertex2d(210.0 + xShiftPlaneSide, 142.0 + yShiftPlaneSide);
	glVertex2d(212.0 + xShiftPlaneSide, 144.0 + yShiftPlaneSide);
	glVertex2d(214.0 + xShiftPlaneSide, 148.0 + yShiftPlaneSide);
	glVertex2d(215.0 + xShiftPlaneSide, 150.0 + yShiftPlaneSide);
	glVertex2d(215.0 + xShiftPlaneSide, 150.0 + yShiftPlaneSide);
	glVertex2d(215.0 + xShiftPlaneSide, 153.0 + yShiftPlaneSide);
	glVertex2d(215.0 + xShiftPlaneSide, 155.0 + yShiftPlaneSide);
	glVertex2d(214.0 + xShiftPlaneSide, 155.0 + yShiftPlaneSide);
	glVertex2d(213.0 + xShiftPlaneSide, 156.0 + yShiftPlaneSide);
	glVertex2d(212.0 + xShiftPlaneSide, 157.0 + yShiftPlaneSide);
	glVertex2d(211.0 + xShiftPlaneSide, 158.0 + yShiftPlaneSide);
	glVertex2d(190.0 + xShiftPlaneSide, 160.0 + yShiftPlaneSide);
	glEnd();
	glBegin(GL_POLYGON);
	glColor3ub(0, 9, 0); // set colour to WHITE
	glVertex2d(185.0 + xShiftPlaneSide, 147.0 + yShiftPlaneSide);
	glVertex2d(194.0 + xShiftPlaneSide, 147.0 + yShiftPlaneSide);
	glVertex2d(194.0 + xShiftPlaneSide, 153.0 + yShiftPlaneSide);
	glVertex2d(185.0 + xShiftPlaneSide, 153.0 + yShiftPlaneSide);
	glEnd();
	glBegin(GL_POLYGON);
	glColor3ub(0, 9, 0); // set colour to WHITE
	glVertex2d(165.0 + xShiftPlaneSide, 147.0 + yShiftPlaneSide);
	glVertex2d(174.0 + xShiftPlaneSide, 147.0 + yShiftPlaneSide);
	glVertex2d(174.0 + xShiftPlaneSide, 153.0 + yShiftPlaneSide);
	glVertex2d(165.0 + xShiftPlaneSide, 153.0 + yShiftPlaneSide);
	glEnd();
	glBegin(GL_POLYGON);
	glColor3ub(0, 9, 0); // set colour to WHITE
	glVertex2d(145.0 + xShiftPlaneSide, 147.0 + yShiftPlaneSide);
	glVertex2d(154.0 + xShiftPlaneSide, 147.0 + yShiftPlaneSide);
	glVertex2d(154.0 + xShiftPlaneSide, 153.0 + yShiftPlaneSide);
	glVertex2d(145.0 + xShiftPlaneSide, 153.0 + yShiftPlaneSide);
	glEnd();
	glBegin(GL_POLYGON);
	glColor3ub(0, 9, 0); // set colour to WHITE
	glVertex2d(125.0 + xShiftPlaneSide, 147.0 + yShiftPlaneSide);
	glVertex2d(134.0 + xShiftPlaneSide, 147.0 + yShiftPlaneSide);
	glVertex2d(134.0 + xShiftPlaneSide, 153.0 + yShiftPlaneSide);
	glVertex2d(125.0 + xShiftPlaneSide, 153.0 + yShiftPlaneSide);
	glEnd();
	glBegin(GL_POLYGON);
	glColor3ub(0, 9, 0); // set colour to WHITE
	glVertex2d(105.0 + xShiftPlaneSide, 147.0 + yShiftPlaneSide);
	glVertex2d(114.0 + xShiftPlaneSide, 147.0 + yShiftPlaneSide);
	glVertex2d(114.0 + xShiftPlaneSide, 153.0 + yShiftPlaneSide);
	glVertex2d(105.0 + xShiftPlaneSide, 153.0 + yShiftPlaneSide);
	glEnd();
	glBegin(GL_TRIANGLES);
	glColor3ub(169, 169, 169); // set colour to WHITE
	glVertex2d(35.0 + xShiftPlaneSide, 160.0 + yShiftPlaneSide);
	glVertex2d(90.0 + xShiftPlaneSide, 160.0 + yShiftPlaneSide);
	glVertex2d(35.0 + xShiftPlaneSide, 185.0 + yShiftPlaneSide);
	glEnd();
}

void clouds1() {
	glBegin(GL_POLYGON);
	glColor3f(255, 255, 255); // set colour to black
	circle(5, 15, 260 + yShiftCloud);
	glBegin(GL_POLYGON);
	glColor3f(255, 255, 255); // set colour to black
	circle(8, 25, 260 + yShiftCloud);
	glBegin(GL_POLYGON);
	glColor3f(255, 255, 255); // set colour to black
	circle(12, 35, 260 + yShiftCloud);
	glBegin(GL_POLYGON);
	glColor3f(255, 255, 255); // set colour to black
	circle(9, 50, 260 + yShiftCloud);
	glBegin(GL_POLYGON);
	glColor3f(255, 255, 255); // set colour to black
	circle(4, 61, 260 + yShiftCloud);
	glEnd();
	//
	glBegin(GL_POLYGON);
	glColor3f(255, 255, 255); // set colour to black
	circle(4, 90, 235 + yShiftCloud);
	glBegin(GL_POLYGON);
	glColor3f(255, 255, 255); // set colour to black
	circle(5, 95, 235 + yShiftCloud);
	glBegin(GL_POLYGON);
	glColor3f(255, 255, 255); // set colour to black
	circle(7, 105, 235 + yShiftCloud);
	glBegin(GL_POLYGON);
	glColor3f(255, 255, 255); // set colour to black
	circle(12, 117, 235 + yShiftCloud);
	glBegin(GL_POLYGON);
	glColor3f(255, 255, 255); // set colour to black
	circle(15, 130, 235 + yShiftCloud);
	glBegin(GL_POLYGON);
	glColor3f(255, 255, 255); // set colour to black
	circle(7, 141, 235 + yShiftCloud);
	glBegin(GL_POLYGON);
	glColor3f(255, 255, 255); // set colour to black
	circle(5, 148, 235 + yShiftCloud);
	glBegin(GL_POLYGON);
	glColor3f(255, 255, 255); // set colour to black
	circle(4, 153, 235 + yShiftCloud);
	glBegin(GL_POLYGON);
	glColor3f(255, 255, 255); // set colour to black
	circle(2, 157, 235 + yShiftCloud);
	glEnd();
	//
	glBegin(GL_POLYGON);
	glColor3f(255, 255, 255); // set colour to black
	circle(5, 178, 260 + yShiftCloud);
	glBegin(GL_POLYGON);
	glColor3f(255, 255, 255); // set colour to black
	circle(8, 186, 260 + yShiftCloud);
	glBegin(GL_POLYGON);
	glColor3f(255, 255, 255); // set colour to black
	circle(12, 199, 260 + yShiftCloud);
	glBegin(GL_POLYGON);
	glColor3f(255, 255, 255); // set colour to black
	circle(9, 216, 260 + yShiftCloud);
	glBegin(GL_POLYGON);
	glColor3f(255, 255, 255); // set colour to black
	circle(4, 227, 260 + yShiftCloud);
	glEnd();
	//
	glBegin(GL_POLYGON);
	glColor3f(255, 255, 255); // set colour to black
	circle(4, 257, 235 + yShiftCloud);
	glBegin(GL_POLYGON);
	glColor3f(255, 255, 255); // set colour to black
	circle(5, 262, 235 + yShiftCloud);
	glBegin(GL_POLYGON);
	glColor3f(255, 255, 255); // set colour to black
	circle(7, 270, 235 + yShiftCloud);
	glBegin(GL_POLYGON);
	glColor3f(255, 255, 255); // set colour to black
	circle(12, 282, 235 + yShiftCloud);
	glBegin(GL_POLYGON);
	glColor3f(255, 255, 255); // set colour to black
	circle(15, 291, 235 + yShiftCloud);
	glBegin(GL_POLYGON);
	glColor3f(255, 255, 255); // set colour to black
	circle(7, 309, 235 + yShiftCloud);
	glBegin(GL_POLYGON);
	glColor3f(255, 255, 255); // set colour to black
	circle(5, 314, 235 + yShiftCloud);
	glBegin(GL_POLYGON);
	glColor3f(255, 255, 255); // set colour to black
	circle(4, 319, 235 + yShiftCloud);
	glBegin(GL_POLYGON);
	glColor3f(255, 255, 255); // set colour to black
	circle(2, 323, 235 + yShiftCloud);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(255, 255, 255); // set colour to black
	circle(5, 346, 260 + yShiftCloud);
	glBegin(GL_POLYGON);
	glColor3f(255, 255, 255); // set colour to black
	circle(8, 346, 260 + yShiftCloud);
	glBegin(GL_POLYGON);
	glColor3f(255, 255, 255); // set colour to black
	circle(12, 359, 260 + yShiftCloud);
	glBegin(GL_POLYGON);
	glColor3f(255, 255, 255); // set colour to black
	circle(9, 367, 260 + yShiftCloud);
	glBegin(GL_POLYGON);
	glColor3f(255, 255, 255); // set colour to black
	circle(4, 378, 260 + yShiftCloud);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(255, 255, 255); // set colour to black
	circle(4, 90, 271 + yShiftCloud);
	glBegin(GL_POLYGON);
	glColor3f(255, 255, 255); // set colour to black
	circle(5, 95, 271 + yShiftCloud);
	glBegin(GL_POLYGON);
	glColor3f(255, 255, 255); // set colour to black
	circle(7, 105, 271 + yShiftCloud);
	glBegin(GL_POLYGON);
	glColor3f(255, 255, 255); // set colour to black
	circle(12, 117, 271 + yShiftCloud);
	glBegin(GL_POLYGON);
	glColor3f(255, 255, 255); // set colour to black
	circle(15, 130, 271 + yShiftCloud);
	glBegin(GL_POLYGON);
	glColor3f(255, 255, 255); // set colour to black
	circle(7, 141, 271 + yShiftCloud);
	glBegin(GL_POLYGON);
	glColor3f(255, 255, 255); // set colour to black
	circle(5, 148, 271 + yShiftCloud);
	glBegin(GL_POLYGON);
	glColor3f(255, 255, 255); // set colour to black
	circle(4, 153, 271 + yShiftCloud);
	glBegin(GL_POLYGON);
	glColor3f(255, 255, 255); // set colour to black
	circle(2, 157, 271 + yShiftCloud);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(255, 255, 255); // set colour to black
	circle(4, 257, 271 + yShiftCloud);
	glBegin(GL_POLYGON);
	glColor3f(255, 255, 255); // set colour to black
	circle(5, 262, 271 + yShiftCloud);
	glBegin(GL_POLYGON);
	glColor3f(255, 255, 255); // set colour to black
	circle(7, 270, 271 + yShiftCloud);
	glBegin(GL_POLYGON);
	glColor3f(255, 255, 255); // set colour to black
	circle(12, 282, 271 + yShiftCloud);
	glBegin(GL_POLYGON);
	glColor3f(255, 255, 255); // set colour to black
	circle(15, 291, 271 + yShiftCloud);
	glBegin(GL_POLYGON);
	glColor3f(255, 255, 255); // set colour to black
	circle(7, 309, 271 + yShiftCloud);
	glBegin(GL_POLYGON);
	glColor3f(255, 255, 255); // set colour to black
	circle(5, 314, 271 + yShiftCloud);
	glBegin(GL_POLYGON);
	glColor3f(255, 255, 255); // set colour to black
	circle(4, 319, 271 + yShiftCloud);
	glBegin(GL_POLYGON);
	glColor3f(255, 255, 255); // set colour to black
	circle(2, 323, 271 + yShiftCloud);
	glEnd();

}

void tree() {
	glColor3ub(139, 69, 19);
	glBegin(GL_POLYGON);
	glVertex2f(100, 15 + yShiftTree);
	glVertex2f(56, 15 + yShiftTree);
	glVertex2f(70, 46 + yShiftTree);
	glVertex2f(85, 46 + yShiftTree);
	glEnd();
	glBegin(GL_POLYGON);
	glColor3ub(34, 139, 34); // set colour to black
	circle(24, 78, 57 + yShiftTreeBush);
	glEnd();
}

void frontPlane() {
	circle(25, 200, 150 + yShiftFrontPlane);
}

void frontEngine1() {
	circle(8, 120, 145 + yShiftFrontPlane);
}

void frontEngine2() {
	circle(8, 280, 145 + yShiftFrontPlane);
}

void frontEngineCase1() {
	circle(6, 120, 145 + yShiftFrontPlane);
}

void frontEngineCase2() {
	circle(6, 280, 145 + yShiftFrontPlane);
}

void frontWings() {
	glBegin(GL_TRIANGLES);
	glColor3ub(169, 169, 169); // set colour to WHITE
	glVertex2d(200.0, 135.0 + yShiftFrontPlane);
	glVertex2d(350.0, 153.0 + yShiftFrontPlane);
	glVertex2d(50.0, 153.0 + yShiftFrontPlane);
	glEnd();
	glBegin(GL_TRIANGLES);
	glColor3ub(169, 169, 169); // set colour to WHITE
	glVertex2d(190.0, 160.0 + yShiftFrontPlane);
	glVertex2d(210.0, 160.0 + yShiftFrontPlane);
	glVertex2d(200.0, 205.0 + yShiftFrontPlane);
	glEnd();
	glBegin(GL_TRIANGLES);
	glColor3ub(169, 169, 169); // set colour to WHITE
	glVertex2d(200.0, 150.0 + yShiftFrontPlane);
	glVertex2d(240.0, 160.0 + yShiftFrontPlane);
	glVertex2d(160.0, 160.0 + yShiftFrontPlane);
	glEnd();
}

void frontPlaneWindow() {
	glBegin(GL_POLYGON);
	glColor3ub(255, 255, 255); // set colour to black
	glVertex2d(190.0, 147.0 + yShiftFrontPlane);
	glVertex2d(210.0, 147.0 + yShiftFrontPlane);
	glVertex2d(220.0, 154.0 + yShiftFrontPlane);
	glVertex2d(180.0, 154.0 + yShiftFrontPlane);
	glEnd();
}

void frontPlaneFans() {
	glLineWidth(2.4);
	glPushMatrix();

	glTranslatef(120, 145 + yShiftFrontPlane, 0);
	glRotatef(rotaAngle, 0, 0, 1);
	glTranslatef(-120, -145 - yShiftFrontPlane, 0);

	glBegin(GL_LINE_STRIP);
	glColor3f(255, 255, 255); // set colour to black
	glVertex2f(120.0, 140.0 + yShiftFrontPlane);
	glVertex2f(120.0, 150.0 + yShiftFrontPlane);
	glEnd();
	rotaAngle += 15;
	glLineWidth(2.4);
	glBegin(GL_LINE_STRIP);
	glColor3f(255, 255, 255); // set colour to black
	glVertex2f(125.0, 145.0 + yShiftFrontPlane);
	glVertex2f(115.0, 145.0 + yShiftFrontPlane);
	glEnd();
	glPopMatrix();

	glLineWidth(2.4);
	glPushMatrix();
	glTranslatef(280, 145 + yShiftFrontPlane, 0);
	glRotatef(rotaAngle2, 0, 0, 1);
	glTranslatef(-280, -145 - yShiftFrontPlane, 0);

	glBegin(GL_LINE_STRIP);
	glColor3f(255, 255, 255); // set colour to black
	glVertex2f(280.0, 140.0 + yShiftFrontPlane);
	glVertex2f(280.0, 150.0 + yShiftFrontPlane);
	glEnd();
	rotaAngle2 += 15;
	glLineWidth(2.4);
	glBegin(GL_LINE_STRIP);
	glColor3f(255, 255, 255); // set colour to black
	glVertex2f(275.0, 145.0 + yShiftFrontPlane);
	glVertex2f(285.0, 145.0 + yShiftFrontPlane);
	glEnd();
	glPopMatrix();
}

void speechBubble() {
	glBegin(GL_POLYGON);
	glColor3ub(255, 0, 0); // set colour to black
	glVertex2d(230, 159.0 + yShiftFrontPlane);
	glVertex2d(379.0, 159.0 + yShiftFrontPlane);
	glVertex2d(379.0, 175.0 + yShiftFrontPlane);
	glVertex2d(230.0, 175.0 + yShiftFrontPlane);
	glEnd();
	glBegin(GL_TRIANGLES);
	glColor3ub(255, 0, 0); // set colour to black
	glVertex2d(230, 159.0 + yShiftFrontPlane);
	glVertex2d(265.0, 159.0 + yShiftFrontPlane);
	glVertex2d(210.0, 150.0 + yShiftFrontPlane);
	glEnd();
}

void windowCockpit() {
	glBegin(GL_POLYGON);
	glColor3ub(255, 255, 255); // set colour to black
	glVertex2d(0, 80.0);
	glVertex2d(400.0, 80.0);
	glVertex2d(400.0, 240.0);
	glVertex2d(0.0, 240.0);
	glEnd();
}

void cockpitDash() {
	glBegin(GL_POLYGON);
	glColor3ub(128, 128, 128); // set colour to black
	glVertex2d(0, 80.0);
	glVertex2d(400.0, 80.0);
	glVertex2d(400.0, 105.0);
	glVertex2d(340.0, 115.0);
	glVertex2d(60.0, 115.0);
	glVertex2d(0, 105.0);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glColor3ub(0, 0, 0); // set colour to black
	glVertex2d(0, 80.0);
	glVertex2d(400.0, 80.0);
	glVertex2d(400.0, 105.0);
	glVertex2d(340.0, 115.0);
	glVertex2d(60.0, 115.0);
	glVertex2d(0, 105.0);
	glVertex2d(0, 105.0);
	glEnd();
}

void cockpitTopPannels() {
	glBegin(GL_POLYGON);
	glColor3ub(128, 128, 128); // set colour to black
	glVertex2d(0, 240.0);
	glVertex2d(25.0, 220.0);
	glVertex2d(375.0, 220.0);
	glVertex2d(400.0, 240.0);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glColor3ub(0, 0, 0); // set colour to black
	glVertex2d(0, 240.0);
	glVertex2d(25.0, 220.0);
	glVertex2d(375.0, 220.0);
	glVertex2d(400.0, 240.0);
	glVertex2d(0, 240.0);
	glEnd();
}

void cockpitDoor() {
	glBegin(GL_POLYGON);
	glColor3ub(128, 128, 128); // set colour to black
	glVertex2d(175, 160.0);
	glVertex2d(230.0, 160.0);
	glVertex2d(230.0, 220.0);
	glVertex2d(175.0, 220.0);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glColor3ub(0, 0, 0); // set colour to black
	glVertex2d(175, 160.0);
	glVertex2d(230.0, 160.0);
	glVertex2d(230.0, 220.0);
	glVertex2d(175.0, 220.0);
	glVertex2d(175.0, 160.0);
	glEnd();
}

void cockpitSeat1() {
	glBegin(GL_POLYGON);
	glColor3ub(220, 220, 220); // set colour to black
	glVertex2d(95, 115.0);
	glVertex2d(135.0, 115.0);
	glVertex2d(135.0, 165.0);
	glVertex2d(95.0, 165.0);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glColor3ub(0, 0, 0); // set colour to black
	glVertex2d(95, 115.0);
	glVertex2d(135.0, 115.0);
	glVertex2d(135.0, 165.0);
	glVertex2d(95.0, 165.0);
	glVertex2d(95, 115.0);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3ub(220, 220, 220); // set colour to black
	glVertex2d(265, 115.0);
	glVertex2d(305.0, 115.0);
	glVertex2d(305.0, 165.0);
	glVertex2d(265.0, 165.0);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glColor3ub(0, 0, 0); // set colour to black
	glVertex2d(265, 115.0);
	glVertex2d(305.0, 115.0);
	glVertex2d(305.0, 165.0);
	glVertex2d(265.0, 165.0);
	glVertex2d(265, 115.0);
	glEnd();
}

void stockmanCockpit() {
	glBegin(GL_POLYGON);
	glColor3ub(0, 0, 0); // set colour to black
	circle(12, 115, 148);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glColor3ub(0, 0, 0); // set colour to black
	glVertex2d(115, 115.0);
	glVertex2d(115, 140.0);
	glEnd();
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

void display(void)
{
	/* clear all pixels  */
	int i;
	glClear(GL_COLOR_BUFFER_BIT);
	frame++;

	if ((frame >= 0) && (frame<210))
	{
		//1st scene drawing goes here
		/* select white for all lines  */
		curtainBar();
		curtain1();
		curtain2();

		xShiftCurtain2 += 1.0;
		yShiftCurtain2 += 0.50;

		xShiftCurtain1 += -1.0;
		yShiftCurtain1 += -0.50;

		glColor3f(0.0, 0.0, 0.0);
		glLineWidth(5.0);
		glPushMatrix();
		glTranslatef(tx, 0, 0);// move right
		glTranslatef(-150, -100, 0);// move to start position
		glTranslatef(200, 160, 0);// move to position
		glScalef(0.5, 0.5, 0);
		glTranslatef(-200, -160, 0);// move to origin

		glPopMatrix();
		tx += 2;
		glFlush();


	}

	

	else if ((frame >= 210) && (frame<336))
	{
		//2nd scene drawing goes here
		/* select white for all lines  */
		glClearColor(0.0, 1.0, 0.0, 0.0);

		glPushMatrix();
		glTranslatef(40, 50, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0, 70, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-60, 110, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-90, 180, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(15, 210, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(250, 50, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(280, 70, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(310, 90, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(255, 150, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(290, 190, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(40, 50, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0, 70, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-60, 110, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-90, 180, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(15, 210, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(250, 50, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(280, 70, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(310, 90, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(255, 150, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(290, 190, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(40, 210, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0, 227, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-60, 249, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-90, 253, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(15, 280, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(250, 321, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(280, 350, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(310, 385, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(255, 430, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(290, 456, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(40, 486, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0, 529, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-60, 420, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-90, 487, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(15, 576, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(250, 623, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(280, 679, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(310, 718, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(255, 728, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(290, 765, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(40, 798, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0, 824, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-60, 546, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-90, 582, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(15, 624, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(250, 858, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(280, 895, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(310, 1026, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(255, 1075, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(290, 1104, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(40, 1159, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0, 679, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-60, 731, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-90, 781, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(15, 835, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(250, 1184, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(280, 1234, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(310, 1273, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(255, 1298, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(290, 1345, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		runway();
		runwayStrips1();

		glPushMatrix();
		glScalef(1.0, 1.0, 0.0);
		planeBody();
		glPopMatrix();

		engines1();
		engines2();
		planeWing1();
		planeWing2();
		planeWing3();
		planeWing4();
		speechBubble();

		glLineWidth(5.0);
		glPushMatrix();
		glTranslatef(0, -100, 0);// move to start position
		glPopMatrix();
		glFlush();

		glColor3ub(0, 0, 0);
		renderSpacedBitmapString(238, 166, GLUT_BITMAP_HELVETICA_10, "Flaps good, fuel guag check.");
	}


	else if ((frame>336) && (frame<380))
	{
		//2nd scene drawing goes here
		/* select white for all lines  */
		glClearColor(0.0, 1.0, 0.0, 0.0);

		glPushMatrix();
		glTranslatef(40, 50, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0, 70, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-60, 110, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-90, 180, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(15, 210, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(250, 50, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(280, 70, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(310, 90, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(255, 150, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(290, 190, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(40, 50, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0, 70, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-60, 110, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-90, 180, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(15, 210, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(250, 50, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(280, 70, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(310, 90, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(255, 150, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(290, 190, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(40, 210, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0, 227, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-60, 249, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-90, 253, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(15, 280, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(250, 321, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(280, 350, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(310, 385, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(255, 430, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(290, 456, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(40, 486, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0, 529, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-60, 420, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-90, 487, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(15, 576, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(250, 623, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(280, 679, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(310, 718, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(255, 728, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(290, 765, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(40, 798, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0, 824, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-60, 546, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-90, 582, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(15, 624, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(250, 858, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(280, 895, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(310, 1026, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(255, 1075, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(290, 1104, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(40, 1159, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0, 679, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-60, 731, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-90, 781, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(15, 835, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(250, 1184, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(280, 1234, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(310, 1273, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(255, 1298, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(290, 1345, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();


		runway();
		runwayStrips1();

		glPushMatrix();
		glScalef(1.0, 1.0, 0.0);
		planeBody();
		glPopMatrix();

		engines1();
		engines2();
		planeWing1();
		planeWing2();
		planeWing3();
		planeWing4();
		speechBubble();

		yShiftRock -= 0.4;
		yShiftBoxP1 -= 0.4;

		glLineWidth(5.0);
		glPushMatrix();
		glTranslatef(0, -100, 0);// move to start position
		glPopMatrix();
		glFlush();

		glColor3ub(0, 0, 0);
		renderSpacedBitmapString(238, 166, GLUT_BITMAP_HELVETICA_10, "Clear for takeoff Runway 22L");
	}

	else if ((frame>460) && (frame<550))
	{
		//2nd scene drawing goes here
		/* select white for all lines  */
		glClearColor(0.0, 1.0, 0.0, 0.0);

		glPushMatrix();
		glTranslatef(40, 50, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0, 70, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-60, 110, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-90, 180, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(15, 210, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(250, 50, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(280, 70, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(310, 90, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(255, 150, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(290, 190, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(40, 50, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0, 70, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-60, 110, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-90, 180, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(15, 210, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(250, 50, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(280, 70, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(310, 90, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(255, 150, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(290, 190, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(40, 210, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0, 227, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-60, 249, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-90, 253, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(15, 280, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(250, 321, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(280, 350, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(310, 385, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(255, 430, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(290, 456, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(40, 486, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0, 529, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-60, 420, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-90, 487, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(15, 576, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(250, 623, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(280, 679, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(310, 718, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(255, 728, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(290, 765, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(40, 798, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0, 824, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-60, 546, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-90, 582, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(15, 624, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(250, 858, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(280, 895, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(310, 1026, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(255, 1075, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(290, 1104, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(40, 1159, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0, 679, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-60, 731, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-90, 781, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(15, 835, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(250, 1184, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(280, 1234, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(310, 1273, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(255, 1298, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(290, 1345, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();


		runway();
		runwayStrips1();

		glPushMatrix();
		glScalef(1.0, 1.0, 0.0);
		planeBody();
		glPopMatrix();

		engines1();
		engines2();
		planeWing1();
		planeWing2();
		planeWing3();
		planeWing4();
		speechBubble();

		yShiftRock -= 0.9;
		yShiftBoxP1 -= 0.9;

		glLineWidth(5.0);
		glPushMatrix();
		glTranslatef(0, -100, 0);// move to start position
		glPopMatrix();
		glFlush();

		glColor3ub(0, 0, 0);
		renderSpacedBitmapString(238, 166, GLUT_BITMAP_HELVETICA_10, "Speed 60 Knots, all good");
	}

	else if ((frame>550) && (frame<750))
	{
		//2nd scene drawing goes here
		/* select white for all lines  */
		glClearColor(0.0, 1.0, 0.0, 0.0);

		glPushMatrix();
		glTranslatef(40, 50, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0, 70, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-60, 110, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-90, 180, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(15, 210, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(250, 50, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(280, 70, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(310, 90, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(255, 150, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(290, 190, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(40, 50, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0, 70, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-60, 110, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-90, 180, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(15, 210, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(250, 50, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(280, 70, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(310, 90, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(255, 150, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(290, 190, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(40, 210, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0, 227, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-60, 249, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-90, 253, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(15, 280, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(250, 321, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(280, 350, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(310, 385, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(255, 430, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(290, 456, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(40, 486, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0, 529, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-60, 420, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-90, 487, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(15, 576, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(250, 623, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(280, 679, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(310, 718, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(255, 728, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(290, 765, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(40, 798, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0, 824, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-60, 546, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-90, 582, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(15, 624, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(250, 858, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(280, 895, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(310, 1026, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(255, 1075, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(290, 1104, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(40, 1159, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0, 679, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-60, 731, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-90, 781, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(15, 835, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(250, 1184, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(280, 1234, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(310, 1273, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(255, 1298, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(290, 1345, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		runway();
		runwayStrips1();

		glPushMatrix();
		glScalef(1.0, 1.0, 0.0);
		planeBody();
		glPopMatrix();
		engines1();
		engines2();
		planeWing1();
		planeWing2();
		planeWing3();
		planeWing4();
		speechBubble();

		yShiftRock -= 2.4;
		yShiftBoxP1 -= 2.4;

		glLineWidth(5.0);
		glPushMatrix();
		glTranslatef(0, -100, 0);// move to start position
		glPopMatrix();
		glFlush();

		glColor3ub(0, 0, 0);
		renderSpacedBitmapString(238, 166, GLUT_BITMAP_HELVETICA_10, "Speed 140 Knots, all good");
	}

	else if ((frame>750) && (frame<865))
	{
		//2nd scene drawing goes here
		/* select white for all lines  */
		glClearColor(0.0, 1.0, 0.0, 0.0);

		glPushMatrix();
		glTranslatef(40, 50, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0, 70, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-60, 110, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-90, 180, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(15, 210, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(250, 50, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(280, 70, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(310, 90, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(255, 150, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(290, 190, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(40, 50, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0, 70, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-60, 110, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-90, 180, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(15, 210, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(250, 50, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(280, 70, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(310, 90, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(255, 150, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(290, 190, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(40, 210, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0, 227, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-60, 249, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-90, 253, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(15, 280, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(250, 321, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(280, 350, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(310, 385, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(255, 430, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(290, 456, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(40, 486, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0, 529, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-60, 420, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-90, 487, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(15, 576, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(250, 623, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(280, 679, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(310, 718, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(255, 728, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(290, 765, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(40, 798, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0, 824, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-60, 546, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-90, 582, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(15, 624, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(250, 858, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(280, 895, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(310, 1026, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(255, 1075, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(290, 1104, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(40, 1159, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0, 679, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-60, 731, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-90, 781, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(15, 835, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(250, 1184, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(280, 1234, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(310, 1273, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(255, 1298, 0);//tranlate by -p where p is the point you want to rotate about
		rock2();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(290, 1345, 0);//tranlate by -p where p is the point you want to rotate about
		rock1();
		glPopMatrix();

		runway();
		runwayStrips1();

		glPushMatrix();
		glScalef(1.0, 1.0, 0.0);
		planeBody();
		glPopMatrix();

		engines1();
		engines2();
		planeWing1();
		planeWing2();
		planeWing3();
		planeWing4();
		speechBubble();

		yShiftRock -= 4.4;
		yShiftBoxP1 -= 4.4;

		glLineWidth(5.0);
		glPushMatrix();
		glTranslatef(0, -100, 0);// move to start position
		glPopMatrix();
		glFlush();

		glColor3ub(0, 0, 0);
		renderSpacedBitmapString(238, 166, GLUT_BITMAP_HELVETICA_10, "Speed 220 Knots, rotate");
	}


	else if ((frame>868) && (frame<1311))
	{
		//2nd scene drawing goes here
		/* select white for all lines  */
		glClearColor(0.0, 0.0, 1.0, 0.0);

		clouds1();
		ground();
		runway2();
		planeSideBody();

		glPushMatrix();
		glTranslatef(1, 55, 0);//tranlate by -p where p is the point you want to rotate about
		tree();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(16, 32, 0);//tranlate by -p where p is the point you want to rotate about
		tree();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(58, 94, 0);//tranlate by -p where p is the point you want to rotate about
		tree();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(72, 21, 0);//tranlate by -p where p is the point you want to rotate about
		tree();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(100, 55, 0);//tranlate by -p where p is the point you want to rotate about
		tree();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(148, 110, 0);//tranlate by -p where p is the point you want to rotate about
		tree();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(172, 76, 0);//tranlate by -p where p is the point you want to rotate about
		tree();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(190, 40, 0);//tranlate by -p where p is the point you want to rotate about
		tree();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(223, 110, 0);//tranlate by -p where p is the point you want to rotate about
		tree();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(264, 65, 0);//tranlate by -p where p is the point you want to rotate about
		tree();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(325, 12, 0);//tranlate by -p where p is the point you want to rotate about
		tree();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(360, 110, 0);//tranlate by -p where p is the point you want to rotate about
		tree();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(374, 65, 0);//tranlate by -p where p is the point you want to rotate about
		tree();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(394, 12, 0);//tranlate by -p where p is the point you want to rotate about
		tree();
		glPopMatrix();

		xShiftPlaneSide += 0.9;
		yShiftPlaneSide += 0.2;
		yGroundShift -= 0.3;
		yRunwayShift -= 0.3;
		yShiftCloud -= 0.06;
		yShiftTreeBush -= 0.3;
		yShiftTree -= 0.3;

		glLineWidth(5.0);
		glPushMatrix();
		glTranslatef(0, -100, 0);// move to start position
		glPopMatrix();
		glFlush();

		glColor3f(1.0, 1.0, 1.0); // set colour to white
		drawRain();
	}

	else if ((frame> 1311) && (frame<1370))
	{
		//2nd scene drawing goes here
		/* select white for all lines  */
		glClearColor(0.0, 0.0, 1.0, 0.0);

		frontWings();
		glBegin(GL_POLYGON);
		glColor3ub(169, 169, 169);
		frontPlane();
		glEnd();
		glBegin(GL_POLYGON);
		glColor3ub(169, 169, 169);
		frontEngine1();
		glEnd();
		glBegin(GL_POLYGON);
		glColor3ub(169, 169, 169);
		frontEngine2();
		frontPlaneWindow();
		glEnd();
		glBegin(GL_POLYGON);
		glColor3ub(0, 0, 0);
		frontEngineCase1();
		glEnd();
		glBegin(GL_POLYGON);
		glColor3ub(0, 0, 0);
		frontEngineCase2();
		glEnd();

		yShiftFrontPlane -= 0.6;

		glPushMatrix();
		frontPlaneFans();
		glPopMatrix();

	}

	else if ((frame> 1370) && (frame<1420))
	{
		//2nd scene drawing goes here
		/* select white for all lines  */
		glClearColor(0.0, 0.0, 1.0, 0.0);

		frontWings();
		glBegin(GL_POLYGON);
		glColor3ub(169, 169, 169); // set colour to WHITE
		frontPlane();
		glEnd();
		glBegin(GL_POLYGON);
		glColor3ub(169, 169, 169); // set colour to WHITE
		frontEngine1();
		glEnd();
		glBegin(GL_POLYGON);
		glColor3ub(169, 169, 169); // set colour to WHITE
		frontEngine2();
		frontPlaneWindow();
		glEnd();
		glBegin(GL_POLYGON);
		glColor3ub(0, 0, 0);
		frontEngineCase1();
		glEnd();
		glBegin(GL_POLYGON);
		glColor3ub(0, 0, 0);
		frontEngineCase2();
		glEnd();

		yShiftFrontPlane += 0.6;

		glPushMatrix();
		frontPlaneFans();
		glPopMatrix();

	}

	else if ((frame> 1420) && (frame<1528))
	{


		/* select white for all lines  */
		glClearColor(0.0, 0.0, 1.0, 0.0);
		frontWings();
		glBegin(GL_POLYGON);
		glColor3ub(169, 169, 169); // set colour to WHITE
		frontPlane();
		glBegin(GL_POLYGON);
		glColor3ub(169, 169, 169); // set colour to WHITE
		frontEngine1();
		glBegin(GL_POLYGON);
		glColor3ub(169, 169, 169); // set colour to WHITE
		frontEngine2();
		glBegin(GL_POLYGON);
		glColor3ub(0, 0, 0);
		frontEngineCase1();
		glBegin(GL_POLYGON);
		glColor3ub(0, 0, 0);
		frontEngineCase2();



		tween(window, cockpit, 4, proportion, tweenPoly);

		glBegin(GL_POLYGON);
		glColor3ub(255, 255, 255); // set colour to WHITE
		for (int i = 0; i<10; i++)
			glVertex2i(tweenPoly[i][0], tweenPoly[i][1]); // Morphing W to Arrow.
		glEnd();

		proportion += 0.01;

		if (proportion >1.0) proportion = 1.0;


		glPushMatrix();
		frontPlaneFans();
		glPopMatrix();

	}

	else if ((frame> 1528) && (frame<1650))
	{
		glClearColor(0.0, 0.0, 0.0, 0.0);

		glPushMatrix();
		windowCockpit();
		cockpitDash();
		cockpitTopPannels();
		cockpitDoor();
		cockpitSeat1();
		stockmanCockpit();
		glPopMatrix();
		
	}

	else if ((frame> 1650) && (frame<1750))
	{

		glClearColor(0.0, 0.0, 0.0, 0.0);

		glPushMatrix();
		windowCockpit();
		cockpitDash();
		cockpitTopPannels();
		cockpitDoor();
		cockpitSeat1();
		stockmanCockpit();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-90, -5, 0);//tranlate by -p where p is the point you want to rotate about
		speechBubble();
		glPopMatrix();

		glColor3ub(0, 0, 0);
		renderSpacedBitmapString(190, 157, GLUT_BITMAP_HELVETICA_10, "So lonley");
	}

	else if ((frame> 1750) && (frame<1900))
	{

		glClearColor(0.0, 0.0, 0.0, 0.0);

		glPushMatrix();
		windowCockpit();
		cockpitDash();
		cockpitTopPannels();
		cockpitDoor();
		cockpitSeat1();
		stockmanCockpit();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-90, -5, 0);//tranlate by -p where p is the point you want to rotate about
		speechBubble();
		glPopMatrix();

		glColor3ub(0, 0, 0);
		renderSpacedBitmapString(148, 157, GLUT_BITMAP_HELVETICA_10, "If only there were more pilots");
	}

	else if ((frame>1900) && (frame<2100))
	{
		//3rd scene drawing goes here
		/* select white for all lines  */
		glColor3f(0.0, 0.0, 1.0);
		curtainBar();

		glColor3ub(255, 255, 255);
		renderSpacedBitmapString(90, 180, GLUT_BITMAP_HELVETICA_12, "THE PILOT INDUSTRY HAS SEEN A");
		glColor3ub(255, 255, 255);
		renderSpacedBitmapString(90, 160, GLUT_BITMAP_HELVETICA_12, "SHORTAGE OF PILOTS. MANY PILOTS");
		glColor3ub(255, 255, 255);
		renderSpacedBitmapString(90, 140, GLUT_BITMAP_HELVETICA_12, "LIKE NOMAN FEEL SAD AND LONELY");
	}

	else if ((frame>2100) && (frame<2300))
	{
		//3rd scene drawing goes here
		/* select white for all lines  */
		glColor3f(0.0, 0.0, 1.0);
		curtainBar();

		glColor3ub(255, 255, 255);
		renderSpacedBitmapString(90, 180, GLUT_BITMAP_HELVETICA_12, "JOIN THE BRITISH PILOT SCHEME");
		glColor3ub(255, 255, 255);
		renderSpacedBitmapString(90, 160, GLUT_BITMAP_HELVETICA_12, "BY CALLING THE LINE BELLOW");
		glColor3ub(255, 255, 255);
		renderSpacedBitmapString(90, 140, GLUT_BITMAP_HELVETICA_12, "TELL : 07687751322");
		glColor3ub(255, 255, 255);
		renderSpacedBitmapString(90, 120, GLUT_BITMAP_HELVETICA_12, "BECOME A PILOT NOW!!!!!!");
		glColor3ub(255, 255, 255);
		renderSpacedBitmapString(85, 90, GLUT_BITMAP_HELVETICA_12, "B R I T I S H    P I L O T    S C H E M E");
	}

	else if (frame > 6000) {
		frame = 0;
		proportion = 0.0;
	}
	fprintf(stdout, "Frame number= %i\n", frame);
	glFlush();
	glutSwapBuffers();
}

//Controls the timer
void Timer(int value)
{
	if (value) glutPostRedisplay();
	glutTimerFunc(40, Timer, value);
}

//Controls the visibility
void visibility(int state) {
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

//Main function which draws everthing onto the window
int main(int argc, char** argv) {
	initRain();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(winWidth, winHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Frame Change For different Scenes");
	init();
	glutDisplayFunc(display);
	glutVisibilityFunc(visibility);
	glutMainLoop();
	return 0;   /* ANSI C requires main to return int. */
}