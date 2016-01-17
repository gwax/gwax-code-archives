/*
 *	3.021j PS1 Problem 1
 *	randwalkgl.c - random walk program source (ANSI C & GLUT)
 *	(c)2003 George Waksman
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <time.h>
#include <math.h>
#include <GL/glut.h>

/*macro functions*/
#define	max(a,b)	(((a) > (b)) ? (a) : (b))
#define	min(a,b)	(((a) > (b)) ? (b) : (a))

/*constants*/
#define	pi	3.1415926535897932384626433832795028841971693993751058209749445923078164
#define	e	2.7182818284590452353602874713526624977572470936999595749669676277240766

/*point in 2D space*/
typedef struct point
{
	double x;
	double y;
} point;

/*an individual walker*/
typedef struct walker
{
	int step;			/*knows how many steps it's taken*/
	double rmsd;		/*knows its current rms distance*/
	point* locations;	/*knows everywhere it has been*/
} walker;

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

/*GLUT related variables*/
int mainWindow, subWindow1, subWindow2, subWindow3;	/*handles to the window and subwindows*/
int w, h, b;	/*main window width, height and border width for subwindows*/
int frame, timepassed, timebase;	/*for fps calculations*/
double fps;

/*program variables*/
int M, N, currentstep;	/*# walkers, # steps, current step #*/
double* rmsD;			/*pointer to a list of all rms distance values so far*/
walker* walkers;		/*pointer to a list of all walkers*/

/*histogram stuff*/
int* histogram;		/*pointer to the histogram*/
int histsize;		/*number of bins in the histogram*/
double maxrmsd;		/*we scale the histogram horizontally based on the max distance reached*/

int main(int argc, char* argv[], char* envp[])
{
	int i;		/*counter*/

	w = 640;		/*640x480 window is usually pretty good*/
	h = 480;
	b = 4;			/*border 4 is just a nice small border*/

	frame = 0;		/*clear time for fps calculation*/
	timebase = 0;
	timepassed = 0;
	fps = 0;

	srand((int)time(NULL));		/*seed random number generator*/

	printf("How many random walkers? ");	/*ask user for walkers and steps*/
	fscanf(stdin, "%d", &M);
	printf("How many steps? ");
	fscanf(stdin, "%d", &N);

	if(M<1 || N<1)		/*set default on mistake*/
	{
		M = 100;
		N = 100;
	}

	currentstep = 1;	/*clear current step and max rms distance*/
	maxrmsd = 0.0;

	histsize = (int)sqrt(1.0*M)+1;	/*number of bins for the histogram, sqrt(1.0*M) seems reasonable*/

	histogram = (int*)malloc(histsize*sizeof(int));		/*allocate histogram memory*/
	if(histogram == NULL)
		return 4;

	rmsD = (double*)malloc((N+1)*sizeof(double));		/*allocate rms list memory*/
	if(rmsD == NULL)
		return 5;
	rmsD[0] = 0;

	walkers = (walker*)malloc(M*sizeof(walker));		/*allocate memory for walkers*/
	if(walkers == NULL)
		return 6;

	for(i=0; i<M; i++)			/*reset all walkers*/
	{
		walkers[i].step = 0;
		walkers[i].locations = (point*)malloc((N+1)*sizeof(point));
		if(walkers[i].locations == NULL)
			return (100+i);
		walkers[i].locations[0].x = 0;
		walkers[i].locations[0].y = 0;
		walkers[i].rmsd = 0;
	}


	/*initialize GLUT*/
	glutInit(&argc, argv);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(w, h);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	mainWindow = glutCreateWindow("Random Walkers GL");	/*create window*/
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

void renderBitmapString(float x, float y, float z, void *font, char *string)
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
	int i,j;	/*counter variables*/
	char buffer[255];

	glutSetWindow(subWindow1);	/*switch to sub window 1*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	/*draw stuff here*/
	if(M>1)
	{
		sprintf(buffer,"%d Random Walkers on step %d of %d at %5.3f fps", M, min(currentstep,N), N, fps);
	}
	else
	{
		sprintf(buffer,"%d Random Walker on step %d of %d at %5.3f fps", M, min(currentstep,N), N, fps);
	}
	glColor3f(0.0,1.0,1.0);
	renderBitmapString(-1.8,1.95,-5,GLUT_BITMAP_HELVETICA_12,buffer);
	for(i=0; i<M; i++)	/*for each walker*/
	{
		glBegin(GL_LINE_STRIP);	/*start a line strip*/
		for(j=0; j<currentstep; j++)	/*for each point in the walkers path*/
		{
			glColor3f(1.0,1.0,1.0);	/*use the white color*/
			if(j == currentstep-1)		/*unless we're at the current location*/
				glColor3f(1.0,0.0,0.0);	/*then use red*/
			glVertex3f(walkers[i].locations[j].x/sqrt(1.0*N), walkers[i].locations[j].y/sqrt(1.0*N),-7);	/*make a point in the line strip*/
		}
		glEnd();	/*finish line strip*/
	}

	glutSwapBuffers();	/*do the double buffer thing*/
}

void renderSub2(void)	/*sub window for rms distance as a function of steps taken*/
{
	int i;	/*counter thing*/
	float D;
	char buffer[255];

	glutSetWindow(subWindow2);	/*switch to sub window 2*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	/*draw stuff here*/
	glColor3f(1.0,0.0,0.0);		/*use red*/
	renderBitmapString(-1.75,1.9,-5,GLUT_BITMAP_HELVETICA_12,"rms distance (y) vs steps taken (x)");
	renderBitmapString(-1.75,1.66,-5,GLUT_BITMAP_HELVETICA_12,"scaling appears diffusive");

	D=0;	/*calculate diffusion constant*/
	for(i=1; i<currentstep; i++)
	{
		D += pow(rmsD[i],2) - pow(rmsD[i-1],2);
	}
	D = D/(currentstep-1)/2;
	sprintf(buffer, "D ~= %f", D);
	renderBitmapString(-1.75,1.42,-5,GLUT_BITMAP_HELVETICA_12,buffer);

	glBegin(GL_LINE_STRIP);		/*start a line strip*/
	for(i=0; i<currentstep; i++)	/*for each step taken so far*/
	{
		glVertex3f(-5+i*10.0/N,-5+rmsD[i]*10.0/sqrt((double)N), -20);
	}	/*plot the rms distance scaled to fit horizontally and vertically*/
	glEnd();	/*finish the line strip*/
	
	glutSwapBuffers();	/*do the double buffer thing*/
}

void renderSub3(void)	/*sub window for distribution histogram*/
{
	int i, temp, histmax;	/*some variables*/
	double r;	/*r for calculating CLT*/
	
	histmax = 0;		/*this gives the highest value in the histogram, for vertical scaling*/
	for(i=0; i<histsize; i++)	/*clear the histogram*/
	{
		histogram[i] = 0;
	}
	for(i=0; i<M; i++)	/*for each walker*/
	{
		temp = walkers[i].rmsd*histsize/maxrmsd; /*find which bin to put it in*/
		if(temp >= histsize)
			temp = histsize-1;
		histogram[temp]++;		/*put it in the bin*/
		histmax = max(histmax, histogram[temp]);	/*find the height of the highest bin*/
	}

	glutSetWindow(subWindow3);	/*switch to sub window 3*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	/*draw stuff here*/
	glColor3f(0.0,0.0,1.0);	/*let's use blue*/
	renderBitmapString(-1.75,1.9,-5,GLUT_BITMAP_HELVETICA_12,"rms distance histogram");
	glBegin(GL_LINE_STRIP);	/*begin a line strip*/
	for(i=0; i<histsize; i++)
	{
		glVertex3f(-5+i*10.0/histsize, -5+histogram[i]*10.0/histmax, -20);
	}	/*draw the properly scaled histogram*/
	glEnd();	/*finish line strip*/

	/*let's draw the central limit therem results for comparison*/
	glColor3f(0.0,1.0,0.0);	/*is green good for you?*/
	renderBitmapString(-1.75,1.68,-5,GLUT_BITMAP_HELVETICA_12,"central limit theorem prediction");
	glBegin(GL_LINE_STRIP);
	for(i=0; i<histsize; i++)
	{
		r = i*maxrmsd/histsize;
		glVertex3f(-5+i*10.0/histsize, -5+(2*r/currentstep)*exp(-pow(r,2)/currentstep)*10.0/sqrt(2/e/currentstep), -20);
	}
	glEnd();

	glColor3f(1.0,0.0,0.0);	/*is green good for you?*/
	renderBitmapString(-1.75,1.46,-5,GLUT_BITMAP_HELVETICA_12,"vertical scales slightly off");
	
	glutSwapBuffers(/*I'm not going to say it again*/);
}

void calcNextStep(void)	/*this takes a step with every walker*/
{
	int i,j;	/*some variables*/
	double randangle, sd;

	if(currentstep<N+1)	/*if we've taken all the steps we want, go away*/
	{
		i = currentstep;	/*i is easier to write*/
		sd = 0.0;		/*clear the current rms distance*/
		for(j=0; j<M; j++)	/*for each walker*/
		{
			randangle = 2*pi*(double)rand()/(double)RAND_MAX;	/*choose a random angle*/

			walkers[j].locations[i].x = walkers[j].locations[i-1].x + cos(randangle);
			walkers[j].locations[i].y = walkers[j].locations[i-1].y + sin(randangle);
			walkers[j].step = i;						/*step one unit in that direction*/

			walkers[j].rmsd = pow((pow(walkers[j].locations[i].x,2)+pow(walkers[j].locations[i].y,2)),.5);
			/*figure out the rms distance for this walker*/

			maxrmsd = max(maxrmsd, walkers[j].rmsd); /*it helps if it keeps track of its own rms distance*/

			sd += walkers[j].rmsd * walkers[j].rmsd;	/*get the square distance*/
		}
		rmsD[i] = pow(sd/M, 0.5); /*route of mean of square distance*/

		currentstep++;	/*we've taken another step*/
	}
}
