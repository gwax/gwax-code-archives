#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main()
{
	int lhand;
	int lfingers;
	int rhand;
	int aside;

	int hexagram[6];

	int i,j;
	unsigned int seed;

	printf("Preparing PRNG...\n");
	printf("\tseed...");
	seed = (unsigned int)time(NULL);
	printf("%d...\n", seed);
	srand(seed);
	printf("done\n");
	printf("\n");

	printf("Generating I Ching...\n");
	for(i=0; i<6; i++)
	{
		printf("Casting Yarrow Stalks for yao line %d...\n", i+1);
		lfingers = 0;
		rhand = 0;
		aside = 0;

		printf("\tThe number of Great Expansion is fifty.\n");
		lhand = 50;

		printf("\tOf which forty-nine are used.\n");
		lhand--;

		for(j=0; j<3; j++)
		{
			if(j==0)
				printf("\tDivide them into two, symbolizing the two primary forces.\n");
			rhand += (int)((double)lhand*(double)rand()/(double)RAND_MAX)+1;
			lhand -= rhand;

			if(j==0)
				printf("\tSuspend one, symbolizing the three supreme powers.\n");
			rhand--;
			lfingers++;

			if(j==0)
				printf("\tManipulate by four, symbolizing the four seasons.\n");
			while(lhand > 4)
			{
				lhand -= 4;
				aside += 4;
			}

			if(j==0)
				printf("\tReturn the remainders, symbolizing the intercalary month.\n");
			lfingers += lhand;
			lhand = 0;

			if(j==0)
				printf("\tIn five years there is another intercalation.\n");
			while(rhand > 4)
			{
				rhand -= 4;
				aside += 4;
			}
			lfingers += rhand;
			rhand = 0;
			printf("\t\t(%d changes completed)\n", 3*i+j+1);
			lfingers = 0;

			if(j==0)
				printf("\tAfterward the process is repeated.\n");
			if(j<2)
			{
				lhand += aside;
				aside = 0;
			}
		}
		printf("\n\t%d piles remain\n", aside/4);
		hexagram[i] = aside/4;
		printf("yao line %d is a ", i+1);
		switch(hexagram[i])
		{
		case 6:
			printf("Greater Yin");
			break;
		case 7:
			printf("Lesser Yang");
			break;
		case 8:
			printf("Lesser Yin");
			break;
		case 9:
			printf("Greater Yang");
			break;
		default:
			printf("WTF?");
			break;
		}
		printf(" line\n\n");
	}

	printf("Casting of Yarrow Stalks complete.\n\n");

	printf("\tHexagram:\n");
	for(i=5; i>=0; i--)
	{
		switch(hexagram[i])
		{
		case 6:
			printf("\t-- X --\n");
			break;
		case 7:
			printf("\t-------\n");
			break;
		case 8:
			printf("\t--   --\n");
			break;
		case 9:
			printf("\t---O---\n");
			break;
		default:
			printf("\t???????\n");
			break;
		}
	}
}

