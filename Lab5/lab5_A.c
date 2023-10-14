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

#define BUF_SZ 5 //declaring buffer size as 5

struct CircularBuffer
{
    int buffer[BUF_SZ];
    int in; //in pointer
    int out; //out pointer
};

//for sharing n and d from parent to child
struct ParentToChild
{
    int n;  //number of elements
    int d;  //common difference
};

#define SIZE sizeof(struct CircularBuffer)  //declaring size of our datatype

//helper functions for circular buffer
void initializeBuffer(struct CircularBuffer *buffer)
{
    buffer->in = 0;
    buffer->out = 0;
}
void insertIntoBuffer(struct CircularBuffer *buffer, int kthElement)
{
    buffer->buffer[buffer->in] = kthElement;
    buffer->in = (buffer->in + 1) % BUF_SZ;
}
int popFromBuffer(struct CircularBuffer *buffer)
{
    int data = buffer->buffer[buffer->out];
    buffer->out = (buffer->out + 1) % BUF_SZ;
    return data;
}

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

    int pipeForData[2]; //making a pipe to transfer n,d from parent to child
    pipe(pipeForData); //make pipe

    //file name for shared memory
    char *sharedMemory = "kk5395_lab5_A";
    //create shared memory segment
    int sharedMemoryID = shm_open(sharedMemory, O_CREAT | O_RDWR, 0666);
    //check if shared memory created successfully
    if(sharedMemoryID == -1)
    {
        perror("shm_open");
        return 1;
    }

    //allocate the size to shared memory
    if(ftruncate(sharedMemoryID, SIZE) == -1)
    {
        perror("ftruncate");
        return 1;
    }
    
    //map the shared memory into address space
    struct CircularBuffer *sharedMemoryPtr = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, sharedMemoryID, 0);
    //check if mapped successfully
    if(sharedMemoryPtr == MAP_FAILED)
    {
        perror("mmap");
        return 1;
    }
    //set in and out pointers of buffer to starting index
    initializeBuffer(sharedMemoryPtr);

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
        close(pipeForData[1]); //write end of pipe closed
        struct ParentToChild dataFromParent;

        //read data from pipe
        read(pipeForData[0], &dataFromParent, sizeof(struct ParentToChild));

        //generate elements
        for(int k = 0 ; k < dataFromParent.n ; ++k)
        {
            int kthElement = k*dataFromParent.d;
            insertIntoBuffer(sharedMemoryPtr, kthElement);
            usleep((rand() % (int)(1e6)));    //random time(0-1s) between generating elements
        }
        close(pipeForData[0]);
        munmap(sharedMemoryPtr, SIZE);
    }
    //routine for parent process
    else
    {
        close(pipeForData[0]); //read end of pipe closed
        
        struct ParentToChild dataToChild;
        dataToChild.n = n;
        dataToChild.d = d;

        //write to the pipe
        write(pipeForData[1], &dataToChild, sizeof(struct ParentToChild));

        int elementsPrinted = 0;
        printf("Parent Printing...\n");
        //we know only n elements will be printed
        while(elementsPrinted < n)
        {
            //print element if generated and buffer not empty
            if(sharedMemoryPtr->in != sharedMemoryPtr->out)
            {
                int kthElement = popFromBuffer(sharedMemoryPtr);
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
        }
        wait(NULL);
        //clean pipe and shared memory
        close(pipeForData[1]);
        munmap(sharedMemoryPtr, SIZE);
        shm_unlink(sharedMemory);
    }
    return 0;
}
