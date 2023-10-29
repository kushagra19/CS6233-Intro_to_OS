# Birthday Probability Estimation

This C program (lab7_1.c) uses a multi-threaded Monte Carlo simulation to estimate the probability of two students in a class having the same birthday. It demonstrates the use of pthreads and a shared mutex for synchronization.

## Usage

To compile the program, use the following command:

```bash
gcc lab7_1.c -o lab7_1.c -pthread
```

To run the program, specify the number of students in the class as a command-line argument. For example, to estimate the probability for a class of 23 students:

```bash
./lab7_1 23
```

## Program Details

### Files

- `lab7_1.c`: The main program that performs the Monte Carlo simulation and calculates the probability.
- `README.md`: This file, providing an overview of the program.

### Dependencies

- The program uses the `pthread` library for multi-threading and synchronization.
- It also uses the `semaphore` library.

### Key Components

- `NUM_THREADS` and `NUM_TRIALS` macros define the number of threads and trials for the simulation.
- `nHits` is a shared variable to store the number of successful experiments.
- The `mutex` is a pthread mutex used for synchronization.
- `WorkerThread` is a thread routine that performs the birthday experiment.
- The main routine initializes the threads, performs the simulation, calculates the probability, and prints the result.

