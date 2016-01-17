#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include "atoms.h"
#include "header.h"

double max(double a, double b) {
  if(a > b) {
    return a;
  } else {
    return b;
  }
}

int main(int argc, char** argv)
{
	Headerinfo tempheader;
	Atom tempatom;
	FILE* file1;
	FILE* file2;
	int i,j,k;
	char buffer[255];
	double sizemul;
	double cutoff;
	double curX, curY, curZ;

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

		sprintf(buffer, "%s.pov", argv[k]);

		file2 = fopen(buffer, "w");

		sizemul = max(tempheader.bigX, max(tempheader.bigY, tempheader.bigZ));
		if(sizemul <= 0.0)
		{
			fprintf(file2, "crash HARD and die\n");
			fclose(file2);
			fclose(file1);
			return 5;
		}
		sizemul = 1/sizemul;
		cutoff = tempheader.mincutoff * sizemul;

		curX = tempheader.bigX*sizemul;
		curY = tempheader.bigY*sizemul;
		curZ = tempheader.bigZ*sizemul;

		fprintf(file2, "#include \"colors.inc\"\n\n");
		fprintf(file2, "camera {\n\tlocation <%f, %f, %f>\n\tlook_at <%f, %f, %f>\n}\n\n", 0.1+0.5*curX, 0.7+0.5*curY, -(0.8+0.5*curZ), 0.5*curX, 0.5*curY, 0.5*curZ);
		fprintf(file2, "light_source { <10, 10, -10>, White }\n\n");
		fprintf(file2, "background { color Black }\n\n");

		fprintf(file2, "cylinder { <0.0, -0.025, -0.025>, <0.0, -0.025, -0.025> + <%f, 0.0, 0.0>, 0.01 pigment { Green } }\n\n", 1.0E-9*sizemul);
		fprintf(file2, "cylinder { <0.0, -0.025, -0.025>, <0.0, -0.025, -0.025> + <%f, 0.0, 0.0>, 0.011 pigment { GreenCopper } }\n\n", 1.0E-10*sizemul);

		fprintf(file2, "cylinder { <0.0, 0.0, 0.0>, <%f, 0.0, 0.0>, 0.002 pigment { Red } }\n", curX);
		fprintf(file2, "cylinder { <0.0, 0.0, 0.0>, <0.0, %f, 0.0>, 0.002 pigment { Red } }\n", curY);
		fprintf(file2, "cylinder { <0.0, 0.0, 0.0>, <0.0, 0.0, %f>, 0.002 pigment { Red } }\n", curZ);

		fprintf(file2, "cylinder { <%f, 0.0, 0.0>, <%f, 0.0, %f>, 0.002 pigment { Red } }\n", curX, curX, curZ);
		fprintf(file2, "cylinder { <%f, 0.0, 0.0>, <%f, %f, 0.0>, 0.002 pigment { Red } }\n", curX, curX, curY);

		fprintf(file2, "cylinder { <0.0, %f, 0.0>, <0.0, %f, %f>, 0.002 pigment { Red } }\n", curY, curY, curZ);
		fprintf(file2, "cylinder { <0.0, %f, 0.0>, <%f, %f, 0.0>, 0.002 pigment { Red } }\n", curY, curX, curY);

		fprintf(file2, "cylinder { <0.0, 0.0, %f>, <%f, 0.0, %f>, 0.002 pigment { Red } }\n", curZ, curX, curZ);
		fprintf(file2, "cylinder { <0.0, 0.0, %f>, <0.0, %f, %f>, 0.002 pigment { Red } }\n", curZ, curY, curZ);

		fprintf(file2, "cylinder { <%f, %f, %f>, <0.0, %f, %f>, 0.002 pigment { Red } }\n", curX, curY, curZ, curY, curZ);
		fprintf(file2, "cylinder { <%f, %f, %f>, <%f, 0.0, %f>, 0.002 pigment { Red } }\n", curX, curY, curZ, curX, curZ);
		fprintf(file2, "cylinder { <%f, %f, %f>, <%f, %f, 0.0>, 0.002 pigment { Red } }\n", curX, curY, curZ, curX, curY);
		fprintf(file2, "\n\n");

		for(i=0; i< (sizeof(elements)/sizeof(elements[0])); i++)
		{
			fprintf(file2, "#macro %s(center)\n", elements[i].symbol);
			fprintf(file2, "\tsphere { center, %f pigment { %s } }\n", elements[i].radius*sizemul, elements[i].pigment);
			fprintf(file2, "#end\n");
		}
		fprintf(file2, "\n");

		for(i=0; i<tempheader.numatoms; i++)
		{
			j = (int)fread(&tempatom, sizeof(Atom), 1, file1);

			if(j < 1)
			{
				printf("error reading atom %d from infile\n\n", i);
				return 3;
			}

			curX = sizemul * tempatom.x;
			curY = sizemul * tempatom.y;
			curZ = sizemul * tempatom.z;
			
			fprintf(file2, "%s( <%f,%f,%f> )\n", elements[tempatom.type].symbol, curX, curY, curZ);
		}

		fclose(file2);
		fclose(file1);
	}

	return 1;
}
