/*
 *	AtomsGL - GLUT Based atom simulation framework
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
#include "pair.h"
#include "globals.h"
#include "graphics.h"

//internal prototypes
int init(int argc, char **argv);
int deinit();

const char defaultTitle[] = "Atom Simulation";	//Default window title

int main(int argc, char *argv[])
{
	if(init(argc, argv))	//initialize and if successful continue
	{						//otherwise quit with errorcode 1
		printf("Initialization failed, exiting\n");
		endprog(1);
	}

	pairinit();

	glutMainLoop();			//begin main GLUT loop

	return(0);
}

int init(int argc, char **argv)	//initialization function
{
	printf("Beginning initialization\n");

	graphicsinit(argc, argv);

	//No initial rotation
    xRot = 0;
	yRot = 0;
	zRot = 0;

	//initialize timers and counters
	time = glutGet(GLUT_ELAPSED_TIME);
	timepassed = 0;
	frame = 0;
	timebase = 0;
	
	printf("Initialization complete\n");

	return 0;
}

int deinit()	//deinitialization function
{
	printf("Beginning deinitialization\n");

	deinitgraphics();

	printf("Deinitialization complete\n");
	return 1;
}

void endprog(int errorcode)	//call this function with 0 or an errorcode to exit
{
	deinit();			//deinitialize
	printf("Exitting\n");
	exit(errorcode);	//then exit
}