#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#include<time.h>

int main(int argc,char *argv[])
{	
	//Check if proper number of arguements passed
	if(argc != 3)
	{
		printf("Too many or less arguements passed.");
		return 1;
	}

	//Number of elements(n)
	int n = atoi(argv[1]);
	//Common Difference
	int d = atoi(argv[2]);

	pid_t childId = fork();

	//if current process is child
	if(childId == 0)
	{	//seeding according to time
		srand(time(NULL));
		//Generate random number (0-99)
		int m = rand() % 100;

		printf("Child Process prints %d elements of AP with first element %d and common difference %d.\n", n, m, d);

		for(int ith = 0 ; ith < n ; ++ith)
		{
			printf("%d ", m + ith*d);
			fflush(stdout);
		}
		printf("\n");
		fflush(stdout);

		exit(m);
	}

	//if current process is the parent
	else if(childId > 0)
	{	int exitStatus;
		pid_t exitChildId = wait(&exitStatus);
		//check which child exited
		if(exitChildId == childId)
		{
			if(WIFEXITED(exitStatus))
			{
				int mFromChild = WEXITSTATUS(exitStatus);

				printf("Parent prints next 5 elements of AP with first element %d and common difference %d.\n", mFromChild, d);
				fflush(stdout);
				for(int ith = n ; ith < n+5 ; ++ith)
				{
					printf("%d ", mFromChild + ith * d);
					fflush(stdout);
				}
				printf("\n");
				fflush(stdout);
			}
		}
	}
	//if error
	else
		printf("Unsuccessful fork");

	return 0;

}

