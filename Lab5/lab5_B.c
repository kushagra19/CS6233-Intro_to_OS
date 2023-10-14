//Initalizing header files
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/mman.h>
#include<sys/stat.h>
#include<sys/shm.h>
#include<sys/wait.h>
#include<unistd.h>
#include<time.h>

//for sharing n and d from parent to child
struct ParentToChild
{
    int n;  //number of elements
    int d;  //common difference
};

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        fprintf(stderr, "Usage: %s <n> <d>\n", argv[0]); //check number of params passed
        return 1;
    }

    int n = atoi(argv[1]); //number of element in the AP
    int d = atoi(argv[2]); //common difference of the AP

    if(n < 1)
    {
        printf("Improper value of n.");
        return 1;
    }

    int pipeForNandD[2]; //making a pipe to transfer n,d from parent to child
    pipe(pipeForNandD); //make pipe

    int pipeForkthElement[2]; //making pipe to communicate kth elements
    pipe(pipeForkthElement);

    //create child process
    pid_t childPID = fork();
    //check if child created successfulyy
    if(childPID < 0)
    {
        perror("fork");
        return 1;
    }

    //routine for child process
    if(childPID == 0)
    {   
        close(pipeForNandD[1]); //write end of pipe n,d closed
        close(pipeForkthElement[0]); //read end of kth element closed
        struct ParentToChild dataFromParent;

        //read data from pipe
        read(pipeForNandD[0], &dataFromParent, sizeof(struct ParentToChild));

        //generate elements
        for(int k = 0 ; k < dataFromParent.n ; ++k)
        {
            int kthElement = k*dataFromParent.d;
            //write to pipe
            write(pipeForkthElement[1], &kthElement, sizeof(kthElement));
            usleep((rand() % (int)(1e6)));    //random time(0-1s) between generating elements
        }
        //pipe cleanup
        close(pipeForNandD[0]);
        close(pipeForkthElement[1]);
    }
    //routine for parent process
    else
    {
        close(pipeForNandD[0]); //read end of pipe n,d closed
        close(pipeForkthElement[1]); //write element of pipe kth element closed

        struct ParentToChild dataToChild;
        dataToChild.n = n;
        dataToChild.d = d;

        //write to the pipe
        write(pipeForNandD[1], &dataToChild, sizeof(struct ParentToChild));

        printf("Parent Printing...\n");
        int kthElement, elementsPrinted = 0;
        while(1)
        {  //read elements as and when generated
            ssize_t status = read(pipeForkthElement[0],&kthElement, sizeof(kthElement));

            //if child closed write end of the pipe then break i.e nothing to read
            if(status == 0)
                break;

            //print sequence elements
            if(elementsPrinted == 0)
                printf("%dst element of the sequence is %d.\n", elementsPrinted+1, kthElement);
            else if(elementsPrinted == 1)
                printf("%dnd element of the sequence is %d.\n", elementsPrinted+1, kthElement);
            else if(elementsPrinted == 2)
                printf("%drd element of the sequence is %d.\n", elementsPrinted+1, kthElement);
            else
                printf("%dth element of the sequence is %d.\n", elementsPrinted+1, kthElement);
            fflush(stdout);
            elementsPrinted += 1;
        }
        wait(NULL);
        //clean pipe and shared memory
        close(pipeForNandD[1]);
        close(pipeForkthElement[0]);
    }
    return 0;
}
