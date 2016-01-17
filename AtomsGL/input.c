/*
 *	AtomsGL - GLUT Based atom simulation framework
 *	input.c - input handling source file
 *	(c)2002 George Waksman
 *
 */

#include <GL/glut.h>
#include "draw.h"
#include "main.h"
#include "pair.h"
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
		objdetail = 16;
		break;
	case '-':
	case '_':
		if(objdetail>4)
			objdetail = objdetail/2;
		break;
	case '=':
	case '+':
		if(objdetail<64)
			objdetail = objdetail*2;
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
		tempupflag = 1;
		tempdownflag = 0;
		break;
	case GLUT_KEY_DOWN:
		tempupflag = 0;
		tempdownflag = 1;
		break;
	case GLUT_KEY_LEFT:
		break;
	case GLUT_KEY_RIGHT:
		break;
	case GLUT_KEY_PAGE_UP:
		break;
	case GLUT_KEY_PAGE_DOWN:
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