#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "atoms.h"
#include "header.h"

#define XMUL 2
#define YMUL 1
#define ZMUL 4
#define N 100

#define TEMP 350.0

const double sqrt2 = 1.4142135623730950488016887242097;
const double sqrt3 = 1.7320508075688772935274463415059;
const double kB = 1.380658E-23;
const double RMo2 = (double)RAND_MAX / 2.0;

int main()
{
	Headerinfo tempheader;
	Atom* allatoms;
	int currentatom;
	int numatoms;
	FILE* file;
	int i,j,k,l,n;
	double currentT;
	double vMul;
	double netmx,netmy,netmz;
	double a;

	file = fopen("infile.atm", "wb");
	if(file == NULL)
	{
		printf("Failed to open infile.atm for writing\n\n");
		exit(6);
	}

	numatoms = (2+6*N)*XMUL*YMUL*ZMUL;

	allatoms = (Atom*)malloc(numatoms*sizeof(Atom));
	if(allatoms == NULL)
	{
		printf("unable to allocate memory for allatoms\n\n");
		fclose(file);
		exit(5);
	}

	/*srand((unsigned int)time(NULL));*/
	srand(2197);

	a = (elements[(enum AtomTypes)H].radius + elements[(enum AtomTypes)C].radius) * 2.0 / sqrt3;

	netmx = netmy = netmz = 0.0;
	currentT = 0.0;
	tempheader.iterations = 1000000;
	tempheader.mincutoff = 10.0 * a;
	tempheader.dt = 1.0E-15;
	tempheader.bigX = a + 6.4 * (double)XMUL * a + a*(double)N;
	tempheader.bigY = a + 8.4 * (double)YMUL * a + a*(double)N;
	tempheader.bigZ = a + 7.8 * (double)ZMUL * a + a*(double)(N/4);
	tempheader.T = TEMP;
	tempheader.desiredT = TEMP;
	tempheader.numatoms = numatoms;

	currentatom = 0;
	for(i=0; i<XMUL; i++)
	{
		for(j=0; j<YMUL; j++)
		{
			for(k=0; k<ZMUL; k++)
			{
				allatoms[currentatom].type = C;
				allatoms[currentatom].x = 2.0 * a + 6.4 * (double)i * a;
				allatoms[currentatom].y = 2.0 * a + 8.4 * (double)j * a;
				allatoms[currentatom].z = 2.0 * a + 7.8 * (double)k * a + 0.25 * a * (double)(N/4);
				allatoms[currentatom].vx = ((double)rand()-RMo2)/RMo2;
				allatoms[currentatom].vy = ((double)rand()-RMo2)/RMo2;
				allatoms[currentatom].vz = ((double)rand()-RMo2)/RMo2;
				allatoms[currentatom].ax = 0.0;
				allatoms[currentatom].ay = 0.0;
				allatoms[currentatom].az = 0.0;
				allatoms[currentatom].ndx = 0.0;
				allatoms[currentatom].ndy = 0.0;
				allatoms[currentatom].ndz = 0.0;
				allatoms[currentatom].potential = 0.0;
				allatoms[currentatom].newpotential = 0.0;
				for(l=0; l<NUMBONDS; l++) allatoms[currentatom].bonds[l] = 0;
				allatoms[currentatom].bonds[0] = 1;
				allatoms[currentatom].bonds[1] = 2;
				allatoms[currentatom].bonds[2] = 3;
				allatoms[currentatom].bonds[3] = 4;
				currentT += 0.5 * elements[allatoms[currentatom].type].mass * (allatoms[currentatom].vx*allatoms[currentatom].vx + allatoms[currentatom].vy*allatoms[currentatom].vy + allatoms[currentatom].vz*allatoms[currentatom].vz);
				netmx += elements[allatoms[currentatom].type].mass * allatoms[currentatom].vx;
				netmy += elements[allatoms[currentatom].type].mass * allatoms[currentatom].vy;
				netmz += elements[allatoms[currentatom].type].mass * allatoms[currentatom].vz;
				currentatom++;

				allatoms[currentatom].type = H;
				allatoms[currentatom].x = 1.5 * a + 6.4 * (double)i * a;
				allatoms[currentatom].y = 1.5 * a + 8.4 * (double)j * a;
				allatoms[currentatom].z = 1.5 * a + 7.8 * (double)k * a + 0.25 * a * (double)(N/4);
				allatoms[currentatom].vx = ((double)rand()-RMo2)/RMo2;
				allatoms[currentatom].vy = ((double)rand()-RMo2)/RMo2;
				allatoms[currentatom].vz = ((double)rand()-RMo2)/RMo2;
				allatoms[currentatom].ax = 0.0;
				allatoms[currentatom].ay = 0.0;
				allatoms[currentatom].az = 0.0;
				allatoms[currentatom].ndx = 0.0;
				allatoms[currentatom].ndy = 0.0;
				allatoms[currentatom].ndz = 0.0;
				allatoms[currentatom].potential = 0.0;
				allatoms[currentatom].newpotential = 0.0;
				for(l=0; l<NUMBONDS; l++) allatoms[currentatom].bonds[l] = 0;
				allatoms[currentatom].bonds[0] = -1;
				currentT += 0.5 * elements[allatoms[currentatom].type].mass * (allatoms[currentatom].vx*allatoms[currentatom].vx + allatoms[currentatom].vy*allatoms[currentatom].vy + allatoms[currentatom].vz*allatoms[currentatom].vz);
				netmx += elements[allatoms[currentatom].type].mass * allatoms[currentatom].vx;
				netmy += elements[allatoms[currentatom].type].mass * allatoms[currentatom].vy;
				netmz += elements[allatoms[currentatom].type].mass * allatoms[currentatom].vz;
				currentatom++;

				allatoms[currentatom].type = H;
				allatoms[currentatom].x = 1.5 * a + 6.4 * (double)i * a;
				allatoms[currentatom].y = 2.5 * a + 8.4 * (double)j * a;
				allatoms[currentatom].z = 2.5 * a + 7.8 * (double)k * a + 0.25 * a * (double)(N/4);
				allatoms[currentatom].vx = ((double)rand()-RMo2)/RMo2;
				allatoms[currentatom].vy = ((double)rand()-RMo2)/RMo2;
				allatoms[currentatom].vz = ((double)rand()-RMo2)/RMo2;
				allatoms[currentatom].ax = 0.0;
				allatoms[currentatom].ay = 0.0;
				allatoms[currentatom].az = 0.0;
				allatoms[currentatom].ndx = 0.0;
				allatoms[currentatom].ndy = 0.0;
				allatoms[currentatom].ndz = 0.0;
				allatoms[currentatom].potential = 0.0;
				allatoms[currentatom].newpotential = 0.0;
				for(l=0; l<NUMBONDS; l++) allatoms[currentatom].bonds[l] = 0;
				allatoms[currentatom].bonds[0] = -2;
				currentT += 0.5 * elements[allatoms[currentatom].type].mass * (allatoms[currentatom].vx*allatoms[currentatom].vx + allatoms[currentatom].vy*allatoms[currentatom].vy + allatoms[currentatom].vz*allatoms[currentatom].vz);
				netmx += elements[allatoms[currentatom].type].mass * allatoms[currentatom].vx;
				netmy += elements[allatoms[currentatom].type].mass * allatoms[currentatom].vy;
				netmz += elements[allatoms[currentatom].type].mass * allatoms[currentatom].vz;
				currentatom++;

				allatoms[currentatom].type = H;
				allatoms[currentatom].x = 2.5 * a + 6.4 * (double)i * a;
				allatoms[currentatom].y = 1.5 * a + 8.4 * (double)j * a;
				allatoms[currentatom].z = 2.5 * a + 7.8 * (double)k * a + 0.25 * a * (double)(N/4);
				allatoms[currentatom].vx = ((double)rand()-RMo2)/RMo2;
				allatoms[currentatom].vy = ((double)rand()-RMo2)/RMo2;
				allatoms[currentatom].vz = ((double)rand()-RMo2)/RMo2;
				allatoms[currentatom].ax = 0.0;
				allatoms[currentatom].ay = 0.0;
				allatoms[currentatom].az = 0.0;
				allatoms[currentatom].ndx = 0.0;
				allatoms[currentatom].ndy = 0.0;
				allatoms[currentatom].ndz = 0.0;
				allatoms[currentatom].potential = 0.0;
				allatoms[currentatom].newpotential = 0.0;
				for(l=0; l<NUMBONDS; l++) allatoms[currentatom].bonds[l] = 0;
				allatoms[currentatom].bonds[0] = -3;
				currentT += 0.5 * elements[allatoms[currentatom].type].mass * (allatoms[currentatom].vx*allatoms[currentatom].vx + allatoms[currentatom].vy*allatoms[currentatom].vy + allatoms[currentatom].vz*allatoms[currentatom].vz);
				netmx += elements[allatoms[currentatom].type].mass * allatoms[currentatom].vx;
				netmy += elements[allatoms[currentatom].type].mass * allatoms[currentatom].vy;
				netmz += elements[allatoms[currentatom].type].mass * allatoms[currentatom].vz;
				currentatom++;

				for(n=0; n<N-1; n++)
				{
					allatoms[currentatom].type = C;
					allatoms[currentatom].x = 2.5 * a + 6.4 * (double)i * a + a*n;
					allatoms[currentatom].y = 2.5 * a + 8.4 * (double)j * a + a*n;
					allatoms[currentatom].z = 1.5 * a + 7.8 * (double)k * a + 0.25 * a * (double)(N/4) + 0.00*a*n;
					allatoms[currentatom].vx = ((double)rand()-RMo2)/RMo2;
					allatoms[currentatom].vy = ((double)rand()-RMo2)/RMo2;
					allatoms[currentatom].vz = ((double)rand()-RMo2)/RMo2;
					allatoms[currentatom].ax = 0.0;
					allatoms[currentatom].ay = 0.0;
					allatoms[currentatom].az = 0.0;
					allatoms[currentatom].ndx = 0.0;
					allatoms[currentatom].ndy = 0.0;
					allatoms[currentatom].ndz = 0.0;
					allatoms[currentatom].potential = 0.0;
					allatoms[currentatom].newpotential = 0.0;
					for(l=0; l<NUMBONDS; l++) allatoms[currentatom].bonds[l] = 0;
					allatoms[currentatom].bonds[0] = -4;
					allatoms[currentatom].bonds[1] = 1;
					allatoms[currentatom].bonds[2] = 2;
					allatoms[currentatom].bonds[3] = 3;
					currentT += 0.5 * elements[allatoms[currentatom].type].mass * (allatoms[currentatom].vx*allatoms[currentatom].vx + allatoms[currentatom].vy*allatoms[currentatom].vy + allatoms[currentatom].vz*allatoms[currentatom].vz);
					netmx += elements[allatoms[currentatom].type].mass * allatoms[currentatom].vx;
					netmy += elements[allatoms[currentatom].type].mass * allatoms[currentatom].vy;
					netmz += elements[allatoms[currentatom].type].mass * allatoms[currentatom].vz;
					currentatom++;

					allatoms[currentatom].type = H;
					allatoms[currentatom].x = 2.0 * a + 6.4 * (double)i * a + a*n;
					allatoms[currentatom].y = 3.0 * a + 8.4 * (double)j * a + a*n;
					allatoms[currentatom].z = 1.0 * a + 7.8 * (double)k * a + 0.25 * a * (double)(N/4) + 0.00*a*n;
					allatoms[currentatom].vx = ((double)rand()-RMo2)/RMo2;
					allatoms[currentatom].vy = ((double)rand()-RMo2)/RMo2;
					allatoms[currentatom].vz = ((double)rand()-RMo2)/RMo2;
					allatoms[currentatom].ax = 0.0;
					allatoms[currentatom].ay = 0.0;
					allatoms[currentatom].az = 0.0;
					allatoms[currentatom].ndx = 0.0;
					allatoms[currentatom].ndy = 0.0;
					allatoms[currentatom].ndz = 0.0;
					allatoms[currentatom].potential = 0.0;
					allatoms[currentatom].newpotential = 0.0;
					for(l=0; l<NUMBONDS; l++) allatoms[currentatom].bonds[l] = 0;
					allatoms[currentatom].bonds[0] = -1;
					currentT += 0.5 * elements[allatoms[currentatom].type].mass * (allatoms[currentatom].vx*allatoms[currentatom].vx + allatoms[currentatom].vy*allatoms[currentatom].vy + allatoms[currentatom].vz*allatoms[currentatom].vz);
					netmx += elements[allatoms[currentatom].type].mass * allatoms[currentatom].vx;
					netmy += elements[allatoms[currentatom].type].mass * allatoms[currentatom].vy;
					netmz += elements[allatoms[currentatom].type].mass * allatoms[currentatom].vz;
					currentatom++;

					allatoms[currentatom].type = H;
					allatoms[currentatom].x = 3.0 * a + 6.4 * (double)i * a + a*n;
					allatoms[currentatom].y = 2.0 * a + 8.4 * (double)j * a + a*n;
					allatoms[currentatom].z = 1.0 * a + 7.8 * (double)k * a + 0.25 * a * (double)(N/4) + 0.00*a*n;
					allatoms[currentatom].vx = ((double)rand()-RMo2)/RMo2;
					allatoms[currentatom].vy = ((double)rand()-RMo2)/RMo2;
					allatoms[currentatom].vz = ((double)rand()-RMo2)/RMo2;
					allatoms[currentatom].ax = 0.0;
					allatoms[currentatom].ay = 0.0;
					allatoms[currentatom].az = 0.0;
					allatoms[currentatom].ndx = 0.0;
					allatoms[currentatom].ndy = 0.0;
					allatoms[currentatom].ndz = 0.0;
					allatoms[currentatom].potential = 0.0;
					allatoms[currentatom].newpotential = 0.0;
					for(l=0; l<NUMBONDS; l++) allatoms[currentatom].bonds[l] = 0;
					allatoms[currentatom].bonds[0] = -2;
					currentT += 0.5 * elements[allatoms[currentatom].type].mass * (allatoms[currentatom].vx*allatoms[currentatom].vx + allatoms[currentatom].vy*allatoms[currentatom].vy + allatoms[currentatom].vz*allatoms[currentatom].vz);
					netmx += elements[allatoms[currentatom].type].mass * allatoms[currentatom].vx;
					netmy += elements[allatoms[currentatom].type].mass * allatoms[currentatom].vy;
					netmz += elements[allatoms[currentatom].type].mass * allatoms[currentatom].vz;
					currentatom++;

					allatoms[currentatom].type = C;
					allatoms[currentatom].x = 2.0 * a + 6.4 * (double)i * a + a*(n+1);
					allatoms[currentatom].y = 2.0 * a + 8.4 * (double)j * a + a*(n+1);
					allatoms[currentatom].z = 2.0 * a + 7.8 * (double)k * a + 0.25 * a * (double)(N/4) + 0.00*a*n;
					allatoms[currentatom].vx = ((double)rand()-RMo2)/RMo2;
					allatoms[currentatom].vy = ((double)rand()-RMo2)/RMo2;
					allatoms[currentatom].vz = ((double)rand()-RMo2)/RMo2;
					allatoms[currentatom].ax = 0.0;
					allatoms[currentatom].ay = 0.0;
					allatoms[currentatom].az = 0.0;
					allatoms[currentatom].ndx = 0.0;
					allatoms[currentatom].ndy = 0.0;
					allatoms[currentatom].ndz = 0.0;
					allatoms[currentatom].potential = 0.0;
					allatoms[currentatom].newpotential = 0.0;
					for(l=0; l<NUMBONDS; l++) allatoms[currentatom].bonds[l] = 0;
					allatoms[currentatom].bonds[0] = -3;
					allatoms[currentatom].bonds[1] = 1;
					allatoms[currentatom].bonds[2] = 2;
					allatoms[currentatom].bonds[3] = 3;
					currentT += 0.5 * elements[allatoms[currentatom].type].mass * (allatoms[currentatom].vx*allatoms[currentatom].vx + allatoms[currentatom].vy*allatoms[currentatom].vy + allatoms[currentatom].vz*allatoms[currentatom].vz);
					netmx += elements[allatoms[currentatom].type].mass * allatoms[currentatom].vx;
					netmy += elements[allatoms[currentatom].type].mass * allatoms[currentatom].vy;
					netmz += elements[allatoms[currentatom].type].mass * allatoms[currentatom].vz;
					currentatom++;

					allatoms[currentatom].type = H;
					allatoms[currentatom].x = 1.5 * a + 6.4 * (double)i * a + a*(n+1);
					allatoms[currentatom].y = 2.5 * a + 8.4 * (double)j * a + a*(n+1);
					allatoms[currentatom].z = 2.5 * a + 7.8 * (double)k * a + 0.25 * a * (double)(N/4) + 0.00*a*n;
					allatoms[currentatom].vx = ((double)rand()-RMo2)/RMo2;
					allatoms[currentatom].vy = ((double)rand()-RMo2)/RMo2;
					allatoms[currentatom].vz = ((double)rand()-RMo2)/RMo2;
					allatoms[currentatom].ax = 0.0;
					allatoms[currentatom].ay = 0.0;
					allatoms[currentatom].az = 0.0;
					allatoms[currentatom].ndx = 0.0;
					allatoms[currentatom].ndy = 0.0;
					allatoms[currentatom].ndz = 0.0;
					allatoms[currentatom].potential = 0.0;
					allatoms[currentatom].newpotential = 0.0;
					for(l=0; l<NUMBONDS; l++) allatoms[currentatom].bonds[l] = 0;
					allatoms[currentatom].bonds[0] = -1;
					currentT += 0.5 * elements[allatoms[currentatom].type].mass * (allatoms[currentatom].vx*allatoms[currentatom].vx + allatoms[currentatom].vy*allatoms[currentatom].vy + allatoms[currentatom].vz*allatoms[currentatom].vz);
					netmx += elements[allatoms[currentatom].type].mass * allatoms[currentatom].vx;
					netmy += elements[allatoms[currentatom].type].mass * allatoms[currentatom].vy;
					netmz += elements[allatoms[currentatom].type].mass * allatoms[currentatom].vz;
					currentatom++;

					allatoms[currentatom].type = H;
					allatoms[currentatom].x = 2.5 * a + 6.4 * (double)i * a + a*(n+1);
					allatoms[currentatom].y = 1.5 * a + 8.4 * (double)j * a + a*(n+1);
					allatoms[currentatom].z = 2.5 * a + 7.8 * (double)k * a + 0.25 * a * (double)(N/4) + 0.00*a*n;
					allatoms[currentatom].vx = ((double)rand()-RMo2)/RMo2;
					allatoms[currentatom].vy = ((double)rand()-RMo2)/RMo2;
					allatoms[currentatom].vz = ((double)rand()-RMo2)/RMo2;
					allatoms[currentatom].ax = 0.0;
					allatoms[currentatom].ay = 0.0;
					allatoms[currentatom].az = 0.0;
					allatoms[currentatom].ndx = 0.0;
					allatoms[currentatom].ndy = 0.0;
					allatoms[currentatom].ndz = 0.0;
					allatoms[currentatom].potential = 0.0;
					allatoms[currentatom].newpotential = 0.0;
					for(l=0; l<NUMBONDS; l++) allatoms[currentatom].bonds[l] = 0;
					allatoms[currentatom].bonds[0] = -2;
					currentT += 0.5 * elements[allatoms[currentatom].type].mass * (allatoms[currentatom].vx*allatoms[currentatom].vx + allatoms[currentatom].vy*allatoms[currentatom].vy + allatoms[currentatom].vz*allatoms[currentatom].vz);
					netmx += elements[allatoms[currentatom].type].mass * allatoms[currentatom].vx;
					netmy += elements[allatoms[currentatom].type].mass * allatoms[currentatom].vy;
					netmz += elements[allatoms[currentatom].type].mass * allatoms[currentatom].vz;
					currentatom++;
				}

				allatoms[currentatom].type = C;
				allatoms[currentatom].x = 2.5 * a + 6.4 * (double)i * a + a*(N-1);
				allatoms[currentatom].y = 2.5 * a + 8.4 * (double)j * a + a*(N-1);
				allatoms[currentatom].z = 1.5 * a + 7.8 * (double)k * a + 0.25 * a * (double)(N/4) + 0.00*a*(N-1);
				allatoms[currentatom].vx = ((double)rand()-RMo2)/RMo2;
				allatoms[currentatom].vy = ((double)rand()-RMo2)/RMo2;
				allatoms[currentatom].vz = ((double)rand()-RMo2)/RMo2;
				allatoms[currentatom].ax = 0.0;
				allatoms[currentatom].ay = 0.0;
				allatoms[currentatom].az = 0.0;
				allatoms[currentatom].ndx = 0.0;
				allatoms[currentatom].ndy = 0.0;
				allatoms[currentatom].ndz = 0.0;
				allatoms[currentatom].potential = 0.0;
				allatoms[currentatom].newpotential = 0.0;
				for(l=0; l<NUMBONDS; l++) allatoms[currentatom].bonds[l] = 0;
				allatoms[currentatom].bonds[0] = -4;
				allatoms[currentatom].bonds[1] = 1;
				allatoms[currentatom].bonds[2] = 2;
				allatoms[currentatom].bonds[3] = 3;
				currentT += 0.5 * elements[allatoms[currentatom].type].mass * (allatoms[currentatom].vx*allatoms[currentatom].vx + allatoms[currentatom].vy*allatoms[currentatom].vy + allatoms[currentatom].vz*allatoms[currentatom].vz);
				netmx += elements[allatoms[currentatom].type].mass * allatoms[currentatom].vx;
				netmy += elements[allatoms[currentatom].type].mass * allatoms[currentatom].vy;
				netmz += elements[allatoms[currentatom].type].mass * allatoms[currentatom].vz;
				currentatom++;

				allatoms[currentatom].type = H;
				allatoms[currentatom].x = 2.0 * a + 6.4 * (double)i * a + a*(N-1);
				allatoms[currentatom].y = 3.0 * a + 8.4 * (double)j * a + a*(N-1);
				allatoms[currentatom].z = 1.0 * a + 7.8 * (double)k * a + 0.25 * a * (double)(N/4) + 0.00*a*(N-1);
				allatoms[currentatom].vx = ((double)rand()-RMo2)/RMo2;
				allatoms[currentatom].vy = ((double)rand()-RMo2)/RMo2;
				allatoms[currentatom].vz = ((double)rand()-RMo2)/RMo2;
				allatoms[currentatom].ax = 0.0;
				allatoms[currentatom].ay = 0.0;
				allatoms[currentatom].az = 0.0;
				allatoms[currentatom].ndx = 0.0;
				allatoms[currentatom].ndy = 0.0;
				allatoms[currentatom].ndz = 0.0;
				allatoms[currentatom].potential = 0.0;
				allatoms[currentatom].newpotential = 0.0;
				for(l=0; l<NUMBONDS; l++) allatoms[currentatom].bonds[l] = 0;
				allatoms[currentatom].bonds[0] = -1;
				currentT += 0.5 * elements[allatoms[currentatom].type].mass * (allatoms[currentatom].vx*allatoms[currentatom].vx + allatoms[currentatom].vy*allatoms[currentatom].vy + allatoms[currentatom].vz*allatoms[currentatom].vz);
				netmx += elements[allatoms[currentatom].type].mass * allatoms[currentatom].vx;
				netmy += elements[allatoms[currentatom].type].mass * allatoms[currentatom].vy;
				netmz += elements[allatoms[currentatom].type].mass * allatoms[currentatom].vz;
				currentatom++;

				allatoms[currentatom].type = H;
				allatoms[currentatom].x = 3.0 * a + 6.4 * (double)i * a + a*(N-1);
				allatoms[currentatom].y = 2.0 * a + 8.4 * (double)j * a + a*(N-1);
				allatoms[currentatom].z = 1.0 * a + 7.8 * (double)k * a + 0.25 * a * (double)(N/4) + 0.00*a*(N-1);
				allatoms[currentatom].vx = ((double)rand()-RMo2)/RMo2;
				allatoms[currentatom].vy = ((double)rand()-RMo2)/RMo2;
				allatoms[currentatom].vz = ((double)rand()-RMo2)/RMo2;
				allatoms[currentatom].ax = 0.0;
				allatoms[currentatom].ay = 0.0;
				allatoms[currentatom].az = 0.0;
				allatoms[currentatom].ndx = 0.0;
				allatoms[currentatom].ndy = 0.0;
				allatoms[currentatom].ndz = 0.0;
				allatoms[currentatom].potential = 0.0;
				allatoms[currentatom].newpotential = 0.0;
				for(l=0; l<NUMBONDS; l++) allatoms[currentatom].bonds[l] = 0;
				allatoms[currentatom].bonds[0] = -2;
				currentT += 0.5 * elements[allatoms[currentatom].type].mass * (allatoms[currentatom].vx*allatoms[currentatom].vx + allatoms[currentatom].vy*allatoms[currentatom].vy + allatoms[currentatom].vz*allatoms[currentatom].vz);
				netmx += elements[allatoms[currentatom].type].mass * allatoms[currentatom].vx;
				netmy += elements[allatoms[currentatom].type].mass * allatoms[currentatom].vy;
				netmz += elements[allatoms[currentatom].type].mass * allatoms[currentatom].vz;
				currentatom++;

				allatoms[currentatom].type = H;
				allatoms[currentatom].x = 3.0 * a + 6.4 * (double)i * a + a*(N-1);
				allatoms[currentatom].y = 3.0 * a + 8.4 * (double)j * a + a*(N-1);
				allatoms[currentatom].z = 2.0 * a + 7.8 * (double)k * a + 0.25 * a * (double)(N/4) + 0.00*a*(N-1);
				allatoms[currentatom].vx = ((double)rand()-RMo2)/RMo2;
				allatoms[currentatom].vy = ((double)rand()-RMo2)/RMo2;
				allatoms[currentatom].vz = ((double)rand()-RMo2)/RMo2;
				allatoms[currentatom].ax = 0.0;
				allatoms[currentatom].ay = 0.0;
				allatoms[currentatom].az = 0.0;
				allatoms[currentatom].ndx = 0.0;
				allatoms[currentatom].ndy = 0.0;
				allatoms[currentatom].ndz = 0.0;
				allatoms[currentatom].potential = 0.0;
				allatoms[currentatom].newpotential = 0.0;
				for(l=0; l<NUMBONDS; l++) allatoms[currentatom].bonds[l] = 0;
				allatoms[currentatom].bonds[0] = -3;
				currentT += 0.5 * elements[allatoms[currentatom].type].mass * (allatoms[currentatom].vx*allatoms[currentatom].vx + allatoms[currentatom].vy*allatoms[currentatom].vy + allatoms[currentatom].vz*allatoms[currentatom].vz);
				netmx += elements[allatoms[currentatom].type].mass * allatoms[currentatom].vx;
				netmy += elements[allatoms[currentatom].type].mass * allatoms[currentatom].vy;
				netmz += elements[allatoms[currentatom].type].mass * allatoms[currentatom].vz;
				currentatom++;
			}
		}
	}

	currentT /= (double)numatoms;
	currentT /= 1.5 * kB;

	netmx /= (double)numatoms;
	netmy /= (double)numatoms;
	netmz /= (double)numatoms;

	vMul = sqrt(TEMP/currentT);

	for(i=0; i<numatoms; i++)
	{
		while(allatoms[i].x > tempheader.bigX) allatoms[i].x -= tempheader.bigX;
		while(allatoms[i].x < 0.0) allatoms[i].x += tempheader.bigX;
		while(allatoms[i].y > tempheader.bigY) allatoms[i].y -= tempheader.bigY;
		while(allatoms[i].y < 0.0) allatoms[i].y += tempheader.bigY;
		while(allatoms[i].z > tempheader.bigZ) allatoms[i].z -= tempheader.bigZ;
		while(allatoms[i].z < 0.0) allatoms[i].z += tempheader.bigZ;

		allatoms[i].vx = (allatoms[i].vx - netmx/elements[allatoms[i].type].mass) * vMul;
		allatoms[i].vy = (allatoms[i].vy - netmy/elements[allatoms[i].type].mass) * vMul;
		allatoms[i].vz = (allatoms[i].vz - netmz/elements[allatoms[i].type].mass) * vMul;
	}

	fwrite(&tempheader, sizeof(Headerinfo), 1, file);
	fwrite(allatoms, sizeof(Atom), numatoms, file);

	free(allatoms);
	fclose(file);

	return 1;
}
