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

#define SIZE sizeof(struct CircularBuffer)  //declaring size of our datatype

//global variable declaration so n,d are stored in data section
int n = 1;
int d = 1;

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

    n = atoi(argv[1]); //number of element in the AP
    d = atoi(argv[2]); //common difference of the AP

    if(n < 1)
    {
        printf("Improper value of n.");
        return 1;
    }

    //file name for shared memory
    char *sharedMemory = "kk5395_lab9";
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

    // Print the start address of the shared buffer
    printf("Producer: Shared Buffer Start Address: %p\n", (void *)sharedMemoryPtr->buffer);
    printf("Producer: n Address: %p\n", (void *)&n);

    close(sharedMemoryID);
    //generate elements
    for(int k = 0 ; k < n ; ++k)
    {
        int kthElement = k*d;
        insertIntoBuffer(sharedMemoryPtr, kthElement);
        usleep((rand() % (int)(1e6)));    //random time(0-1s) between generating elements
    }
    munmap(sharedMemoryPtr, SIZE);
    return 0;
}
