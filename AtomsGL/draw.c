/*
 *	AtomsGL - GLUT Based atom simulation framework
 *	draw.c - frame drawing source file
 *	(c)2002 George Waksman
 *
 */

#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include "main.h"
#include "draw.h"
#include "pair.h"
#include "globals.h"

//internal prototypes
void increment(int incpassed);

void draw(void)	//this is the function that redraws the screen
{
	float fps;
	int oldtime;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();

	//begin fps routines
	oldtime = time;
	time = glutGet(GLUT_ELAPSED_TIME);
	timepassed += time - oldtime;
	if(time - timebase > 1000)
	{
		fps = frame * 1000.0 / (time - timebase);
		timebase = time;
		frame = 0;
		printf("%d s - %.3f fps - t=%f\n", time/1000, fps, t);
	}

	//update objects
	pairiterate();

	//draw stuff here
	glTranslatef(0.0f,0.0f,1.0f);
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
	glRotatef(zRot, 0.0f, 0.0f, 1.0f);
	drawedges();
	drawallobjects();
	
	//stop drawing stuff and switch buffers
	glPopMatrix();
	glutSwapBuffers();
	frame++;	//one more frame has passed
}

void resize(int width, int height)	//if the window is resized...
{
	windowMain.width = width;	//update our window dimension records
	windowMain.height = height;

	if(height == 0)		//divide by zero = doubleplusungood
		height = 1;

	ratio = 1.0 * width / height;	//calculate aspect ratio

	//do resizing stuff
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, width, height);
	gluPerspective(45,ratio,1,1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//set up current camera
	gluLookAt(0,0,5, 0,0,-1, 0,1,0)
}

void toggleFullScreen()	//this toggles fullscreen status
{
	stateFullScreen = !stateFullScreen;	//change what we know of our current status

	if(stateFullScreen)
	{
		windowTemp.x = windowMain.x;	//record what it was before toggle
		windowTemp.y = windowMain.y;
		windowTemp.width = windowMain.width;
		windowTemp.height = windowMain.height;
		glutFullScreen();	//go fullscreen
	}
	else
	{
		glutPositionWindow(windowTemp.x, windowTemp.y);	//retrn to the previous state
		glutReshapeWindow(windowTemp.width, windowTemp.height);
	}
}
