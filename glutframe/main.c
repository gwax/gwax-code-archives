/*
 *	Generic GLUT framework
 *	main.c - main source file
 *	(c)2002 George Waksman
 *
 */

#include <GL/glut.h>
#include <stdio.h>
#include <string.h>
#include "draw.h"
#include "input.h"
#include "main.h"

//internal prototypes
int init(int argc, char **argv);
int deinit();

const char defaultTitle[] = "FOO";	//Default window title

void main(int argc, char *argv[])
{
	if(init(argc, argv))	//initialize and if successful continue
	{						//otherwise quit with errorcode 1
		printf("Initialization failed, exiting\n");
		endprog(1);
	}

	glutMainLoop();			//begin main GLUT loop
}

int init(int argc, char **argv)	//initialization function
{
	printf("Beginning initialization\n");

	vectorUp.x=0;			//assign the up vector to (0,1,0)
	vectorUp.y=1;
	vectorUp.z=0;

	cameraDefault.location.x=0;	//set up a default camera
	cameraDefault.location.y=0;
	cameraDefault.location.z=5;
	cameraDefault.target.x=0;
	cameraDefault.target.y=0;
	cameraDefault.target.z=-1;

	cameraCurrent = &cameraDefault;	//set the default camera as the current camera

	windowTitle = &defaultTitle;	//set the window title to be the default
	windowMain.x = 50;				//set up initial window shape
	windowMain.y = 50;
	windowMain.width = 640;
	windowMain.height = 480;

	stateFullScreen = 0;	//default is to start in window mode
							//to start in fullscreen mode do NOT
							//change this value, instead call
							//toggleFullScreen() after the window
							//has been created

	frame = 0;				//set up counters
	timebase = 0;
	
	incps = 30;				//this value represents the number of
							//animation increments per second
							//change if you want to do more or less
							//every second
	mspinc = 1000/incps;

	//GLUT initialization follows, don't worry about it
	glutInit(&argc, argv);
	glutInitWindowPosition(windowMain.x, windowMain.y);
	glutInitWindowSize(windowMain.width, windowMain.height);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	printf("Creating window \"%s\"\n", windowTitle);
	glutCreateWindow(windowTitle);
	glutDisplayFunc(draw);
	glutIdleFunc(draw);
	glutReshapeFunc(resize);
	glutKeyboardFunc(normalKey);
	glutSpecialFunc(specialKey);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseActive);
	glutPassiveMotionFunc(mousePassive);
	glutEntryFunc(mouseEntry);

	//initialize timers
	time = glutGet(GLUT_ELAPSED_TIME);
	timepassed = 0;

	printf("Initialization complete\n");

	return 0;
}

int deinit()	//deinitialization function
{
	printf("Beginning deinitialization\n");

	printf("Deinitialization complete\n");
	return 1;
}

void endprog(int errorcode)	//call this function with 0 or an errorcode to exit
{
	deinit();			//deinitialize
	printf("Exitting\n");
	exit(errorcode);	//then exit
}