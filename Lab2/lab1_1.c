#include<stdio.h>
#include<stdlib.h>
#include<time.h>

void print_statement()
{	
	srand(time(NULL));	//seeding rand(), so that random number every time
	printf("Hello world! This is CS6233 Fall 2023.\n");
	printf("%i\n", rand()%1000);
	printf("\n");
}

int main()
{
	print_statement();
	return 0;
}
