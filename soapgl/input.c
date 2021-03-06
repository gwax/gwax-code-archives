/*
 *	Soap Surface Simulator in GLUT
 *	input.c - input handling source file
 *	(c)2002 George Waksman
 *
 */

#include <GL/glut.h>
#include "draw.h"
#include "main.h"
#include "soap.h"
#include "globals.h"

void normalKey(unsigned char key, int x, int y)	//normal key input handling
{
	int mods;
	mods = glutGetModifiers();

	switch(key)
	{
	case 27:	//quit on Esc key
		endprog(0);
		break;
	case 13:
		if(mods == GLUT_ACTIVE_ALT)	//fullscreen toggle on Alt+Enter
			toggleFullScreen();
		break;
	case 32:
		xRot = 0;
		yRot = 0;
		zRot = 0;
		break;
	case '-':
	case '_':
		break;
	case '=':
	case '+':
		break;
	}
}

void specialKey(int key, int x, int y)	//special key input handling
{
	int mods;
	mods = glutGetModifiers();

	switch(key)
	{
	case GLUT_KEY_F1:
		toggleFullScreen();	//fullscreen toggle on F1
		break;
	case GLUT_KEY_F4:		//quit on Alt+F4
		if(mods == GLUT_ACTIVE_ALT)
			endprog(0);
		break;
	case GLUT_KEY_UP:
		xRot -= 5;
		break;
	case GLUT_KEY_DOWN:
		xRot += 5;
		break;
	case GLUT_KEY_LEFT:
		yRot -= 5;
		break;
	case GLUT_KEY_RIGHT:
		yRot += 5;
		break;
	case GLUT_KEY_PAGE_UP:
		zRot += 5;
		break;
	case GLUT_KEY_PAGE_DOWN:
		zRot -= 5;
		break;
	}
}

void mouse(int button, int state, int x, int y) //mouse button input handling
{
}

void mouseActive(int x, int y)	//active mouse movement handling
{
}

void mousePassive(int x, int y)	//passive mouse movement handling
{
}

void mouseEntry(int state)		//mouse window entry handling
{
}