/*
 *	AtomsGL - GLUT Based atom simulation framework
 *	graphics.c - source file for everything graphics
 *	(c)2002 George Waksman
 *
 */

#include <GL/glut.h>
#include "graphics.h"

int graphicsinit(int argc, char **argv)
{
	windowTitle = (char *)&defaultTitle;	//set the window title to be the default
	windowMain.x = 50;				//set up initial window shape
	windowMain.y = 50;
	windowMain.width = 640;
	windowMain.height = 480;

	ratio = 1.0 * windowMain.width / windowMain.height;

	stateFullScreen = 0;	//default is to start in window mode
							//to start in fullscreen mode do NOT
							//change this value, instead call
							//toggleFullScreen() after the window
							//has been created

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

	//initialize quadratics
	quadratic=gluNewQuadric();
	gluQuadricNormals(quadratic, GLU_SMOOTH);

	printf("Initialization complete\n");

	return 0;
}

int graphicsdeinit()	//deinitialization function
{
	gluDeleteQuadric(quadratic);
	return 1;
}