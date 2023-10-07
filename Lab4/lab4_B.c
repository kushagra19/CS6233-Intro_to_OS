#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>

int main()
{
	//Store parent process id before fork()
	pid_t parentProcessId = getpid();
	//Create Child using Fork
	pid_t childId = fork();
	
	//if child process
	if(childId == 0)
	{
		printf("My Parent ID: %d \n", parentProcessId);
		fflush(stdout);
	}
	//if fork causes Error
	else if(childId == -1)
	{
		printf("Fork Unsuccessful \n");
		fflush(stdout);
	}
	
	//wait for the child process
	wait(NULL);

	return 0;
}
