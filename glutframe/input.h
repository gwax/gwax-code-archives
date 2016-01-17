/*
 *	Generic GLUT framework
 *	input.h - input handling header file
 *	(c)2002 George Waksman
 *
 */

#ifndef _glutframework_input_h_
#define _glutframework_input_h_

void normalKey(unsigned char key, int x, int y);
void specialKey(int key, int x, int y);
void mouse(int button, int state, int x, int y);
void mouseActive(int x, int y);
void mousePassive(int x, int y);
void mouseEntry(int state);

#endif