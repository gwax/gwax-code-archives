/*
 *	AtomsGL - GLUT Based atom simulation framework
 *	pair.c - simulation source file
 *	(c)2002 George Waksman
 *
 */

#include <GL/glut.h>
#include <math.h>
#include "main.h"
#include "pair.h"
#include "globals.h"

void drawedges()
{
	glBegin(GL_QUADS);
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(lenMul*xMin/yMax, lenMul*yMin/yMax,0);
		glVertex3f(lenMul*(xMin+redge)/yMax, lenMul*(yMin+redge)/yMax,0);
		glVertex3f(lenMul*(xMin+redge)/yMax, lenMul*(yMax-redge)/yMax,0);
		glVertex3f(lenMul*xMin/yMax, lenMul*yMax/yMax,0);

		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(lenMul*xMin/yMax, lenMul*yMin/yMax,0);
		glVertex3f(lenMul*(xMin+redge)/yMax, lenMul*(yMin+redge)/yMax,0);
		glVertex3f(lenMul*(xMax-redge)/yMax, lenMul*(yMin+redge)/yMax,0);
		glVertex3f(lenMul*xMax/yMax, lenMul*yMin/yMax,0);

		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(lenMul*xMax/yMax, lenMul*yMin/yMax,0);
		glVertex3f(lenMul*(xMax-redge)/yMax, lenMul*(yMin+redge)/yMax,0);
		glVertex3f(lenMul*(xMax-redge)/yMax, lenMul*(yMax-redge)/yMax,0);
		glVertex3f(lenMul*xMax/yMax, lenMul*yMax/yMax,0);

		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(lenMul*xMax/yMax, lenMul*yMax/yMax,0);
		glVertex3f(lenMul*(xMax-redge)/yMax, lenMul*(yMax-redge)/yMax,0);
		glVertex3f(lenMul*(xMin+redge)/yMax, lenMul*(yMax-redge)/yMax,0);
		glVertex3f(lenMul*xMin/yMax, lenMul*yMax/yMax,0);
	glEnd();
}

void drawallobjects()
{
	int i;
	double xPos, yPos, zPos, rad;
	double xCur, yCur, zCur;
	
	xCur = 0.0;
	yCur = 0.0;
	zCur = 0.0;
	for(i=0; i<objs; i++)
	{
		switch(dim)
		{
		case 3:
			xPos = lenMul*obj[i].s[0]/yMax;
			yPos = lenMul*obj[i].s[1]/yMax;
			zPos = lenMul*obj[i].s[2]/yMax;
			break;
		case 2:
			xPos = lenMul*obj[i].s[0]/yMax;
			yPos = lenMul*obj[i].s[1]/yMax;
			zPos = 0.0;
			break;
		case 1:
			xPos = lenMul*obj[i].s[0]/yMax;
			yPos = 0.0;
			zPos = 0.0;
			break;
		}
		rad = lenMul*obj[i].rmin/yMax;

		glColor3f(obj[i].R, obj[i].G, obj[i].B);
		glTranslatef(xPos-xCur, yPos-yCur, zPos-zCur);
		gluSphere(quadratic, rad, objdetail, objdetail);

		xCur = xPos;
		yCur = yPos;
		zCur = zPos;
	}
	glTranslatef(-xCur, -yCur, -zCur);
}

void pairinit()
{
	int i,j;
	double c;

	yMin = -65;
	yMax = -yMin;
	xMin = 4*yMin/3;
	xMax = 4*yMax/3;

	objdetail = 16;

	tempupflag = 0;
	tempdownflag = 0;

	dt = 0.01; // if your system 'splodes this is too big
	t = 0;
	rthreshold = 8;	//it seems to be pretty good for now
	redge = 10;	//ok

	epsilon = 100; // constant, damned if I know what it should be

	lenMul = 1.65;

	objs = 999;

    c = 0.8;
	for(i=0; i<37; i++)
	{
		for(j=0; j<27; j++)
		{
			obj[27*i+j].rmin = 2;
			obj[27*i+j].s[0] = -72 + 4*i;
			obj[27*i+j].s[1] = -52 + 4*j;
			obj[27*i+j].s[2] = 0;
			obj[27*i+j].v[0] = 0;
			obj[27*i+j].v[1] = 0;
			obj[27*i+j].v[2] = 0;

			obj[27*i+j].R = c;
			obj[27*i+j].G = c;
			obj[27*i+j].B = c;

			if(c < 0.85)
				c = 0.9;
			else if(c < 0.95)
				c = 1.0;
			else
				c = 0.8;
		}
	}
}

void pairiterate()
{
	int i, j;

	for(i=0; i<dim; i++)
	{
		averagev[i] = 0;
	}

	for(i=0; i<objs; i++)
	{
		updateobjects(&obj[i]);
	}

	for(i=0; i<objs; i++)
	{
		for(j=i+1; j<objs; j++)
		{
			updateobjectv(&obj[i], &obj[j]);
		}
		updateobjectvedge(&obj[i]);
	}

	t += dt;
}

void updateobjectvedge(object* one)
{
	double r;

	r = (one->s[0] - xMin)/redge;
	one->v[0] += dt*144/pow(r,12);

	r = (one->s[0] - xMax)/redge;
	one->v[0] -= dt*144/pow(r,12);
	
	r = (one->s[1] - yMin)/redge;
	one->v[1] += dt*144/pow(r,12);
	
	r = (one->s[1] - yMax)/redge;
	one->v[1] -= dt*144/pow(r,12);
}

void updateobjectv(object* one, object* two)
{
	int i;
	double r, rm;
	double rs[dim];

	rm = one->rmin + two->rmin;

	for(i=0; i<dim; i++)
	{
		rs[i] = one->s[i] - two->s[i];
	}

	r = 0;
	for(i=0; i<dim; i++)
	{
		r += rs[i]*rs[i];
	}
	if(sqrt(r)>rthreshold)
		return;

	for(i=0; i<dim; i++)
	{
		rs[i] = epsilon*(-12*rs[i]*pow(rm,12)/pow(r,7) + 12*rs[i]*pow(rm,6)/pow(r,4));
	}

	for(i=0; i<dim; i++)
	{
		one->v[i] -= dt*rs[i];
		two->v[i] += dt*rs[i];
	}
}

void updateobjects(object* one)
{
	int i;

	if(tempupflag == 1)
	{
		for(i=0; i<dim; i++)
		{
			one->v[i] = one->v[i] * 2;
		}
		tempupflag = 0;
		tempdownflag = 0;
	}

	if(tempdownflag == 1)
	{
		for(i=0; i<dim; i++)
		{
			one->v[i] = one->v[i] / 2;
		}
		tempupflag = 0;
		tempdownflag = 0;
	}

	for(i=0; i<dim; i++)
	{
		one->s[i] += dt*one->v[i];
		averagev[i] += one->v[i];
	}
}