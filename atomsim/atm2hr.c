#include <errno.h>
#include <stdio.h>
#include "atoms.h"
#include "header.h"

int main(int argc, char** argv)
{
	Headerinfo tempheader;
	Atom tempatom;
	FILE* file1;
	FILE* file2;
	int i,j,k;
	char buffer[255];

	if(argc < 2)
	{
		printf("no infile specified\n\n");
		return 0;
	}

	for(k=1; k<argc; k++)
	{
		file1 = fopen(argv[k], "rb");

		if(file1 == NULL)
		{
			printf("failed to open infile %s for reading\n\n", argv[k]);
			return errno;
		}
		
		printf("using infile %s\n\n", argv[k]);

		j = (int)fread(&tempheader, sizeof(Headerinfo), 1, file1);

		if(j < 1)
		{
			printf("error reading header from infile\n\n");
			return 0;
		}

		sprintf(buffer, "%s.hr", argv[k]);

		file2 = fopen(buffer, "w");

		fprintf(file2, "iterations remaining = %d\n", tempheader.iterations);
		fprintf(file2, "minimum cutoff distance = %e\n", tempheader.mincutoff);
		fprintf(file2, "delta time = %e\n", tempheader.dt);
		fprintf(file2, "system x size = %e\n", tempheader.bigX);
		fprintf(file2, "system y size = %e\n", tempheader.bigY);
		fprintf(file2, "system z size = %e\n", tempheader.bigZ);
		fprintf(file2, "number of atoms = %d\n", tempheader.numatoms);
		fprintf(file2, "\n");

		for(i=0; i<tempheader.numatoms; i++)
		{
			j = (int)fread(&tempatom, sizeof(Atom), 1, file1);

			if(j < 1)
			{
				printf("error reading atom %d from infile\n\n", i);
				return 0;
			}

			fprintf(file2, "Atom #%d\n", i);
			fprintf(file2, "type = %s\n", elements[tempatom.type].symbol);
			fprintf(file2, "position = <%e, %e, %e>\n", tempatom.x, tempatom.y, tempatom.z);
			fprintf(file2, "velocity = <%e, %e, %e>\n", tempatom.vx, tempatom.vy, tempatom.vz);
			fprintf(file2, "potential = %e\n", tempatom.potential);
			fprintf(file2, "\n");
		}

		fclose(file2);
		fclose(file1);
	}

	return 1;
}
