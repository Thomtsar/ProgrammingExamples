/*
 * csp1.c
 *
 *  Created on: May 16, 2017
 *      Author: thomt
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <windows.h>
#include <time.h>

int calculateCost(int x, int y);
void setPuzzle(char *fileName);
int solution();
void change(int x, int y,int problem);
int ** pathFinder(int x,int y, int **findPath);
int simulatedAnneling();
void printBlack();

struct myhitori{
	int value;
	int white; // if it is white or not
	int block;
	int cost; // which is the cost of this block
};

typedef struct myhitori hitori;

hitori **puzzle;
int N;
int block = 0;

int solution()
{
	int i,j;
	for(i=0; i<N; i++)
	{
		for(j=0; j<N; j++)
		{
			if(puzzle[i][j].cost > 0)
				return 0;
		}
	}
	return 1;
}


int ** pathFinder(int x,int y, int **findPath)
{
	int i;

	if(puzzle[x][y].white ==1) findPath[x][y] =1;



	for(i=y+1; i<N; i++)
	{
		if(puzzle[x][i].white==0 || findPath[x][i] == 1) break;
		findPath[x][i] = 1;
		findPath = pathFinder(x,i,findPath);
	}


	for(i=x+1; i<N; i++)
	{
		if(puzzle[i][y].white == 0 || findPath[i][y] == 1) break;

		findPath[i][y] = 1;
		findPath = pathFinder(i,y,findPath);
	}

	for(i=x-1; i>=0; i--)
	{
		if(puzzle[i][y].white == 0 || findPath[i][y] == 1) break;

		findPath[i][y] = 1;
		findPath = pathFinder(i,y,findPath);

	}

	for(i=y-1; i>= 0 ; i--)
	{
		if(puzzle[x][i].white==0 || findPath[x][i] == 1) break;

		findPath[x][i] = 1;
		findPath = pathFinder(x,i,findPath);
	}

	return findPath;
}

int calculateCost(int x, int y)
{
	int i,j;
	int **findPath;


	i= x;
	j= y;

	puzzle[x][y].cost = 0;

	if(puzzle[x][y].white == 0)
	{
		findPath = malloc(N*sizeof(int *));

		for(i=0; i<N; i++)
		{
				findPath[i] = (int *) malloc(N*sizeof(int));
				for(j=0; j<N; j++)
				{
					findPath[i][j] = 0;
				}
		}

		for(i=0; i<N; i++)
		{
			for(j=0; j<N; j++)
			{
				if(puzzle[i][j].white == 1)
				{

					break;
				}

			}
			if(j<N) break;
		}

		findPath = pathFinder(i,j,findPath);


			if ( (y+1 < N && findPath[x][y+1]==0 && puzzle[x][y+1].white==1)
					|| (y-1>=0 &&findPath[x][y-1]==0 && puzzle[x][y-1].white==1)
					|| (x+1 <N && findPath[x+1][y]==0 && puzzle[x+1][y].white==1)
					|| (x-1 >= 0 && findPath[x-1][y]==0  && puzzle[x-1][y].white==1) )
				puzzle[x][y].cost ++;



			if( (x+1 < N && puzzle[x+1][y].white ==0)
					|| (y+1 < N && puzzle[x][y+1].white ==0)
					|| (x-1 >= 0 && puzzle[x-1][y].white ==0 )
					|| (y-1 >= 0 && puzzle[x][y-1].white ==0 ) )
				puzzle[x][y].cost ++ ;

			if(puzzle[x][y].cost >= 1) return 2;


			return 0;
	}
	i++;
	while(i<N)
	{
		if(puzzle[x][y].value == puzzle[i][j].value && puzzle[i][j].white == 1)
		{
			puzzle[x][y].cost ++;
			return 1;
		}
		i++;
	}
	i=x;

	j++;
	while(j<N)
	{
			if(puzzle[x][y].value == puzzle[i][j].value  && puzzle[i][j].white == 1)
			{
				puzzle[x][y].cost ++;
				return 1;
			}
			j++;
	}
	j=y;

	j--;
	while(j>=0)
	{
		if(puzzle[x][y].value == puzzle[i][j].value  && puzzle[i][j].white == 1)
		{
			puzzle[x][y].cost ++;
			return 1;
		}
		j--;
	}
	j=y;

	i--;
	while(i>=0)
	{
		if(puzzle[x][y].value == puzzle[i][j].value  && puzzle[i][j].white == 1)
		{
			puzzle[x][y].cost ++;
			return 1;
		}
		i--;
	}
	return 0;
}

void change(int x, int y,int problem)
{
	if(problem==1)
	{
		puzzle[x][y].white = 0;
	}
	else if(problem ==2)
	{
		puzzle[x][y].white = 1;
	}

	calculateCost(x,y);
}

void setPuzzle(char *fileName)
{
	int i,j;
	char *c;
	char temp;
	int k =1;

	div_t n;
	FILE *file;



	n = div(N,10);
	while(n.quot != 0)
	{
		n=div(n.quot,10);
		k++;
	}
	c=(char *)malloc( (k+1)*sizeof(char) );


	printf("File name is %s \n \n",fileName);
	file = fopen(fileName,"r");

	if(file == NULL)
	{
		printf("Couldn't load the array from the file");
		exit(0);
	}


	for(i=0; i<N; i++)
	{
			for(j=0; j<N; j++)
			{
				k=0;
				temp = getc(file);
				while(isdigit(temp) != 0)
				{
					c[k] = temp;
					k++;
					temp = getc(file);
				}
				c[k] = '\0';



				puzzle[i][j].value = atoi(c);
				puzzle[i][j].white = 1;
				puzzle[i][j].cost = 0;
				puzzle[i][j].block = 0;

				if(puzzle[i][j].value <1  || puzzle[i][j].value>N)
				{
					printf("The variable %d at (%d,%d) is bigger than the array length %d",
							puzzle[i][j].value,i,j,N);
					exit(ERROR);
				}


				printf("%d ",puzzle[i][j].value);
			}
			printf("\n");
	}
	printf("\n \n");
	for(i=0; i<N; i++)
	{
		for(j=0; j<N; j++)
		{
			calculateCost(i,j);
		}

	}



	fclose(file);

}

void printBlack()
{
	int i,j;
	printf("--------------------------------\n");
	for(i =0; i<N; i++)
	{

		for(j=0; j<N; j++)
		{
			if(puzzle[i][j].white==0)
				printf(" X |");
			else
				printf(" %d |",puzzle[i][j].value);
		}
		printf(" \n");
		printf("--------------------------------\n");

	}



}

int simulatedAnneling()
{


	int newx,newy;
	int x,y;
	int DE;
	int k=0;
	int failure = 0;
	double T = 10.0;
	double p;



	srand(time(NULL));

	x = rand() % N;
	y = rand() % N;

	newx = rand() % N;
	newy = rand() % N;

	while(T != 0)
	{
		failure =0;
		k=0;

		while(k<2*N && failure < 6*N )
		{

			change(x,y,calculateCost(x,y));

			while(newx == x && newy ==y )
			{
				newx = rand() % N;
				newy = rand() % N;
			}
			calculateCost(newx,newy);
			DE = puzzle[newx][newy].cost - puzzle[x][y].cost ;

			if(DE > 0 )
			{
				failure = 0;
				x =newx;
				y= newy;

			}
			else if(DE <= 0)
			{
				p=1/(1+exp(DE/T));
				if(p > ((rand()%100) / 100.0))
				{
					failure = 0;
					x=newx;
					y=newy;

				}
				else
				{
					failure++;
				}

			}

			if(solution() || failure == 6*N)
			{
				printf("The algorithm just ended. \n");
				printf("It ended at (%d,%d)  with temperature %.4f and %d cost \n \n", x,y,T,puzzle[x][y].cost);
				return 1;
			}

			k++;

		}

		T = 0.999*T;

	}

	return 0;


}



int main(int argc, char *argv[])
{

	int i;


	N = atoi(argv[1]);


	puzzle = malloc(N *sizeof(hitori*));
	for(i=0; i<N; i++)
	{
		puzzle[i] = (hitori*)malloc(N*sizeof(hitori));
	}

	setPuzzle(argv[2]);


	simulatedAnneling();

	printBlack();

	return 1;

}
