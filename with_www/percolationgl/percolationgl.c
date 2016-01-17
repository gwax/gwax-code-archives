/*
 *	3.021j PS1 Problem 2
 *	percolationgl.c - percolation program source (ANSI C & GLUT)
 *	(c)2003 George Waksman
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <time.h>
#include <math.h>
#include <GL/glut.h>

/*some prototypes*/
void mainLoop(void);
void changeWindowSize(int w1, int h1);
void changeWindowMatrix(int w1, int h1);
void renderBitmapString(float x, float y, float z, void *font, char *string);
void renderMain(void);
void renderSub1(void);
void renderSub2(void);
void renderSub3(void);
void calcNextStep(void);
int testspot(int i, int j);

/*GLUT related variables*/
int mainWindow, subWindow1, subWindow2, subWindow3;	/*handles to the window and subwindows*/
int w, h, b;	/*main window width, height and border width for subwindows*/
int frame, timepassed, timebase;	/*for fps calculations*/
double fps;

/*program variables*/
float p, pmin, pmax;	/*probability of site occupation and min and max for multiple tests*/
float pc;				/*holder for our calculated p sub c*/
int** grid;	/*the grid of sites*/
int* graph;	/*array to store results*/
int graphsize;	/*for scaling the graph*/
int r, d;	/*radius and diameter*/
int tests, currenttest;	/*how many tests to run*/
int span;	/*0 to test just one probability, other to test range from 0 to 1*/

int main(int argc, char* argv[], char* envp[])
{
	int ch;		/*character holder*/
	int i,j;	/*counters*/

	w = 640;		/*640x480 window is usually pretty good*/
	h = 480;
	b = 4;			/*border 4 is just a nice small border*/

	frame = 0;		/*clear time for fps calculation*/
	timebase = 0;
	timepassed = 0;
	fps = 0;

	srand((int)time(NULL));	/*seed the random number generator*/

	printf("Would you like to run more than 1 test? ");	/*get user input*/
	fflush(stdin);
	ch = fgetc(stdin);
	if(tolower(ch) != 'y')
	{
		tests = 1;
		span = 0;
		printf("What site occupation probability would you like? ");
		fflush(stdin);
		fscanf(stdin, "%f", &p);
		printf("What radius would you like? ");
		fflush(stdin);
		fscanf(stdin, "%d", &r);
		currenttest = 1;
		pmin = 0;
		pmax = 1;
	}
	else
	{
		printf("How many tests would you like to run? ");
		fflush(stdin);
		fscanf(stdin, "%d", &tests);
		span = 1;
		printf("Run from what probability? ");
		fflush(stdin);
		fscanf(stdin, "%f", &pmin);
		printf("Run to what probability? ");
		fflush(stdin);
		fscanf(stdin, "%f", &pmax);
		printf("What radius would you like? ");
		fflush(stdin);
		fscanf(stdin, "%d", &r);
		p = 0;
		currenttest = 0;
	}

	if(pmin>=pmax || pmin<0 || pmax>1)
	{
		pmin = 0;
		pmax = 0;
	}

	if(p>1 || p<0 || r<1)	/*set defaults on error*/
	{
		tests = 1;
		span = 0;
		p = 0.5;
		r = 25;
	}
	d = 2*r+3;		/*calculate diameter*/

	pc = 1.0;		/*reset p sub c to a max value*/

	graphsize = (int)sqrt((double)tests);	/*this gives a decent number of bins for the graph*/

	grid = (int**)malloc(d*sizeof(int*));	/*allocate memory for the grid*/
	if(grid == NULL)
		return 99;
	for(i=0; i<d; i++)
	{
		grid[i] = (int*)malloc(d*sizeof(int));
		if(grid[i] == NULL)
			return (100+i);
	}

	graph = (int*)malloc((graphsize+1)*sizeof(int));	/*allocate memory for the graph*/
	if(graph == NULL)
		return 98;

	for(i=0; i<=graphsize; i++)	/*clear the graph*/
	{
		graph[i] = 0;
	}

	for(i=0; i<d; i++)		/*clear the grid*/
	{
		for(j=0; j<d; j++)
		{
			grid[i][j] = -1;
		}
	}

	/*initialize GLUT*/
	glutInit(&argc, argv);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(w, h);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	mainWindow = glutCreateWindow("Percolation GL");	/*create window*/
		glutReshapeFunc(changeWindowSize);
		glutDisplayFunc(renderMain);
		glutIdleFunc(mainLoop);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

	subWindow1 = glutCreateSubWindow(mainWindow,		/*create subwindows*/
									b, b,
									2*w/3-3*b/2, h-2*b);
		glutDisplayFunc(renderSub1);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

	subWindow2 = glutCreateSubWindow(mainWindow,
									2*w/3+b/2, b,
									w/3-3*b/2, h/2-3*b/2);
		glutDisplayFunc(renderSub2);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

	subWindow3 = glutCreateSubWindow(mainWindow,
									2*w/3+b/2, h/2+b/2,
									w/3-3*b/2, h/2-3*b/2);
		glutDisplayFunc(renderSub3);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

	if(span == 0)		/*if we're only doing one test, do it now and be done*/
		testspot(r+1, r+1);

	glutMainLoop();		/*start glut loop*/

	return 0;
}

void mainLoop(void)		/*this is our primary loop*/
{
	calcNextStep();	/*calculate the next step*/

	renderMain();	/*render the window and subwindows*/
	renderSub1();
	renderSub2();
	renderSub3();

	frame++;		/*calculate fps*/
	timepassed = glutGet(GLUT_ELAPSED_TIME);
	if(timepassed - timebase > 1000)
	{
		fps = frame*1000.0/(timepassed-timebase);
		printf("%5.3f fps\n", fps);
		timebase = timepassed;
		frame = 0;
	}
}

void changeWindowSize(int w1, int h1)	/*this function is called on window resize*/
{
	if(h1 == 0)	/*no div by zero*/
		h1 = 1;

	w = w1;
	h = h1;

	glutSetWindow(subWindow1);		/*rescale all subwindows*/
		glutPositionWindow(b, b);
		glutReshapeWindow(2*w/3-3*b/2, h-2*b);
		changeWindowMatrix(2*w/3-3*b/2, h-2*b);

	glutSetWindow(subWindow2);
		glutPositionWindow(2*w/3+b/2, b);
		glutReshapeWindow(w/3-3*b/2, h/2-3*b/2);
		changeWindowMatrix(w/3-3*b/2, h/2-3*b/2);

	glutSetWindow(subWindow3);
		glutPositionWindow(2*w/3+b/2, h/2+b/2);
		glutReshapeWindow(w/3-3*b/2, h/2-3*b/2);
		changeWindowMatrix(w/3-3*b/2, h/2-3*b/2);
}

void changeWindowMatrix(int w1, int h1)	/*rescale the projection matrix*/
{
	float ratio;

	if(h1 == 0)
		h1 = 1;

	ratio = 1.0f * w1 / h1;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0, 0, w1, h1);

	gluPerspective(45, ratio, 0.1, 1000);
	glMatrixMode(GL_MODELVIEW);
}

void renderBitmapString(float x, float y, float z, void *font, char *string)	/*renders strings in GL*/
{
	char *c;
	glRasterPos3f(x, y,z);
	for (c=string; *c != '\0'; c++)
	{
		glutBitmapCharacter(font, *c);
	}
}

void renderMain(void)	/*renders main window*/
{
	glutSetWindow(mainWindow);	/*switch to main window*/
	glClearColor(0.1,0.6,0.8,0.0);	/*set clear color to a nice blue*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	/*clear to blue*/
	glClearColor(0.0,0.0,0.0,0.0);	/*set clear color back to black*/
	glutSwapBuffers();	/*do the double buffer thing*/
}

void renderSub1(void)	/*renders walker path display window*/
{
	int i,j;
	char buffer[255];

	glutSetWindow(subWindow1);	/*switch to sub window 1*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	/*draw stuff here*/
	glLoadIdentity();
	glColor3f(0.0,1.0,0.0);		/*first give some information*/
	sprintf(buffer, "Occupation probability %.5f on test %d of %d", p, currenttest, tests);
	renderBitmapString(-1.8,1.95,-5,GLUT_BITMAP_HELVETICA_12,buffer);
	sprintf(buffer, "cluster testing for radius of %d", r);
	renderBitmapString(-1.8,1.83,-5,GLUT_BITMAP_HELVETICA_12,buffer);
	sprintf(buffer, "running at %5.3f fps", fps);
	renderBitmapString(-1.8,1.71,-5,GLUT_BITMAP_HELVETICA_12,buffer);
	glTranslatef(0.0, 0.0, -3.0*r);	/*then draw the grid*/
	glTranslatef(-1.1*(r+1), -1.1*(r+1), 0);
    for(i=0; i<d; i++)
	{
		for(j=0; j<d; j++)
		{
			if((r+1-i)*(r+1-i)+(r+1-j)*(r+1-j) > r*r-1)	/*only in the radius*/
			{
				glColor3f(0.0, 0.0, 0.0);
			}
			else
			{
				switch(grid[i][j])	/*choose color based on status of cell*/
				{
				case -1:
					glColor3f(1.0, 0.0, 0.0);
					break;
				case 0:
					glColor3f(1.0, 1.0, 0.0);
					break;
				default:
					glColor3f(0.0, 1.0, 0.0);
				}
			}

			glBegin(GL_QUADS);	/*one square at a time*/
				glVertex3f( 0.5, 0.5, 0.5);
				glVertex3f(-0.5, 0.5, 0.5);
				glVertex3f(-0.5,-0.5, 0.5);
				glVertex3f( 0.5,-0.5, 0.5);
			glEnd();

			glTranslatef(1.1, 0.0, 0.0);
		}
		glTranslatef(-1.1*d, 1.1, 0.0);
	}

	glutSwapBuffers();	/*do the double buffer thing*/
}

void renderSub2(void)	/*sub window for rms distance as a function of steps taken*/
{
	char buffer[255];

	glutSetWindow(subWindow2);	/*switch to sub window 2*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	/*draw stuff here*/
	glLoadIdentity();
	glColor3f(1.0,0.0,0.0);		/*display info about p sub c*/
	renderBitmapString(-1.75,1.9,-5,GLUT_BITMAP_HELVETICA_12,"p sub c is being estimated");
	renderBitmapString(-1.75,1.66,-5,GLUT_BITMAP_HELVETICA_12,"as the lowest occupation");
	renderBitmapString(-1.75,1.42,-5,GLUT_BITMAP_HELVETICA_12,"probability that results in");
	renderBitmapString(-1.75,1.18,-5,GLUT_BITMAP_HELVETICA_12,"a 50% chance of spanning");

	sprintf(buffer, "p sub c ~= %f", pc);
	renderBitmapString(-1.75,0.70,-5,GLUT_BITMAP_HELVETICA_12,buffer);
	
	glutSwapBuffers();	/*do the double buffer thing*/
}

void renderSub3(void)	/*sub window for distribution histogram*/
{
	int i, graphmax;	/*some variables*/
	char buffer[255];

	graphmax = (int)((double)tests/(double)graphsize)-1;

	glutSetWindow(subWindow3);	/*switch to sub window 3*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	/*draw stuff here*/
	glLoadIdentity();
	glColor3f(1.0,0.0,0.0);		/*show the scale bars*/
	renderBitmapString(-1.45,1.2,-5,GLUT_BITMAP_HELVETICA_12,"1");
	renderBitmapString(-1.45,-1.30,-5,GLUT_BITMAP_HELVETICA_12,"0");
	sprintf(buffer, "%.2f", pmin);
	renderBitmapString(-1.35,-1.5,-5,GLUT_BITMAP_HELVETICA_12,buffer);
	sprintf(buffer, "%.2f", pmax);
	renderBitmapString(1.25,-1.5,-5,GLUT_BITMAP_HELVETICA_12,buffer);
	glBegin(GL_LINE_STRIP);
		glVertex3f(-5.1,5.1,-20);
		glVertex3f(-5.1,-5.1,-20);
		glVertex3f(5.1,-5.1,-20);
	glEnd();

	glColor3f(0.0,0.0,1.0);	/*let's use blue*/
	renderBitmapString(-1.75,1.9,-5,GLUT_BITMAP_HELVETICA_12,"spanning probability (y)");	/*heading*/
	renderBitmapString(-1.75,1.66,-5,GLUT_BITMAP_HELVETICA_12,"vs occupation probability (x)");
	glBegin(GL_LINE_STRIP);	/*begin a line strip*/
	for(i=0; i<=graphsize; i++)
	{
		glVertex3f(-5+i*10.0/graphsize, -5+graph[i]*10.0/graphmax, -20);	/*draw the graph*/
		if(graph[i] > .5*graphmax && i < pc*graphsize)	/*if we find a place where the graph is*/
			pc = pmin+(pmax-pmin)*(float)i/(float)graphsize;				/*above 0.5 before our current pc, move pc down to match*/
	}
	glEnd();	/*finish line strip*/

	glutSwapBuffers(/*I'm not going to say it again*/);
}

void calcNextStep(void)	/*this takes a step with every walker*/
{
	int i, j;

	if(span == 0)	/*if we're not running tests spanning probabilities, don't do anything*/
		return;

	if(currenttest >= tests)	/*if we've done enough tests already, don't do anything*/
		return;

	for(i=0; i<d; i++)	/*clear the grid for the next step*/
	{
		for(j=0; j<d; j++)
		{
			grid[i][j] = -1;
		}
	}

	p = pmin + (pmax-pmin)*(float)currenttest/(float)tests;	/*set occupancy probability for this test*/

	graph[(int)((float)(graphsize+1)*(p-pmin)/(pmax-pmin))] += testspot(r+1, r+1);		/*run the test and increment the graph if we succeed*/
	
	currenttest++;	/*next test*/
}

int testspot(int i, int j)
{
	int temp;	/*temporary variable*/
	temp = 0;

	if(grid[i][j] != -1)	/*if we've already dealt with this cell, don't do it again*/
		return 0;

	if(i == r+1 && j == r+1)	/*if this is the starting cell*/
	{
		grid[i][j] = 1;			/*it is always occupies*/
	}
	else
	{
		if((double)rand()/(double)RAND_MAX < p)	/*otherwise check if it's occupied*/
		{	
			grid[i][j] = 1;		/*if it is, mark it as such*/
		}
		else
		{
			grid[i][j] = 0;		/*otherwise, mark it empty and return failure*/
			return 0;
		}
	}

	if((r+1-i)*(r+1-i)+(r+1-j)*(r+1-j) > r*r-1)	/*have we hit the radial edge*/
	{
		if(grid[i][j] == 1)		/*and are we occupied*/
			return 1;			/*if so return success*/
		else
			return 0;			/*otherwise, fail*/
	}

	/*check if any of a cell's neighbors return success*/
	temp = testspot(i+1,j) + testspot(i-1,j) + testspot(i,j+1) + testspot(i,j-1);

	if(temp>0)	/*if so, return success*/
		return 1;

	return 0;	/*otherwise, fail*/
}