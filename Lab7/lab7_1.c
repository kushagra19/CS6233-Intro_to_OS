//Initializing header files required
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<time.h>

//macro declaration
#define NUM_THREADS 8
#define NUM_TRIALS 1e5

//shared variables
int nHits = 0;
pthread_mutex_t mutex;

//Worker thread routine
void* WorkerThread(void* pVar)
{
    int *numStudents = (int*) pVar;
    //generating a random seed for thread
    unsigned int randState = (unsigned int)time(NULL) + pthread_self();
    //doing NUM_TRIALS trials of the experiment
    for(int trial = 0 ; trial < NUM_TRIALS ; ++trial)
    {
        int birthday[*numStudents];
        //generating random birthdays(0-364) for numStudents
        for(int i = 0 ; i < *numStudents ; ++i)
        {
            birthday[i] = rand_r(&randState) % 365;
        }

        //perform experiment
        int flag = 0; //To check whether experiment succeeded
        for(int i = 0 ; i < *numStudents ; ++i)
        {
            for(int j = i+1 ; j < *numStudents ; ++j)
            {   //only if experiment was not successful before
                if(birthday[i] == birthday[j])
                {
                    flag = 1;
                    pthread_mutex_lock(&mutex);
                    nHits += 1; //increment the variable storing successful experiments
                    pthread_mutex_unlock(&mutex);
                    break;
                }    
            }
            //if experiment successful
            if(flag)
                break;
        }
    }
    pthread_exit(NULL);
}

//main routine
int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        printf("Usage: %s <number of students>\n", argv[0]);
        return 1;
    }

    int numStudents = atoi(argv[1]);

    //check for negative number of students
    if(numStudents <= 0)
    {
        printf("Number of students must be positive.\n");
        return 1;
    }

    //seed at the start of the experiment
    srand((unsigned int)time(NULL));

    //initialize threads of size NUM_THREADS
    pthread_t threads[NUM_THREADS];
    //initialize mutex for all threads
    pthread_mutex_init(&mutex, NULL);

    //create threads
    for(int i = 0 ; i < NUM_THREADS ; ++i)
    {
        pthread_create(&threads[i], NULL, WorkerThread, &numStudents);
    }

    //wait for threads to finish execution 
    for(int i = 0 ; i < NUM_THREADS ; ++i)
    {
        pthread_join(threads[i], NULL);
    }

    //deinitialize mutex
    pthread_mutex_destroy(&mutex);

    //calculate probability of two students having the same birthday
    double p = (double)nHits / (NUM_THREADS*NUM_TRIALS);

    printf("Probability of two students having same birthday in a class of %d students is %.2f%%.\n", numStudents, p*100);


}