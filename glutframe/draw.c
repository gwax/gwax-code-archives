/*
 *	Generic GLUT framework
 *	draw.c - frame drawing source file
 *	(c)2002 George Waksman
 *
 */

#include <GL/glut.h>
#include <stdio.h>
#include "main.h"
#include "draw.h"

//internal prototypes
void increment(int incpassed);

void draw(void)	//this is the function that redraws the screen
{
	float fps;
	int oldtime;
	int incspassed;

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
		printf("%.2f\n",fps);
	}

	//calculate how many increments have passed and update as needed
	incspassed = timepassed / mspinc;
	timepassed -= incspassed * mspinc;
	increment(incspassed);

	//draw stuff here
	
	glPopMatrix();
	glutSwapBuffers();
	frame++;	//one more frame has passed
}

void resize(int width, int height)	//if the window is resized...
{
	float ratio;

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
	gluLookAt(cameraCurrent->location.x,cameraCurrent->location.y,cameraCurrent->location.z,
			  cameraCurrent->target.x,cameraCurrent->target.y,cameraCurrent->target.z,
			  vectorUp.x,vectorUp.y,vectorUp.z);
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

void increment(int incpassed)	//this is used to help with animation timing
{
	while(incpassed > 0)
	{
		//put animation information here

		incpassed--;
	}
}