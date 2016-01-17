/*
 *	AtomsGL - GLUT Based atom simulation framework
 *	graphics.h - header file for everything graphics
 *	(c)2002 George Waksman
 *
 */

#ifndef _atomsgl_graphics_h_	//prevent multiple instances
#define _atomsgl_graphics_h_

//prototypes
int graphicsinit(int argc, char **argv);
int graphicsdeinit();

//structures
typedef struct _window
{
	int x;
	int y;
	int width;
	int height;
} window;

#endif