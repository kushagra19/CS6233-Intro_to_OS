# Producer-Consumer Program with Shared Memory

This program demonstrates inter-process communication between a producer and a consumer using shared memory. The producer generates elements of an arithmetic sequence, and the consumer prints them from the shared buffer.

## Prerequisites

- This program is written in C and assumes a Unix-like environment.
- Ensure that the necessary libraries are available (e.g., `unistd.h`, `sys/mman.h`, `sys/stat.h`).

## Building the Program

Compile the producer and consumer programs separately:

```bash
gcc lab9_producer.c -o lab9_producer -lrt
gcc lab9_consumer.c -o lab9_consumer -lrt
```

## Running the Program
-Producer:
Run the producer program with the desired parameters (n and d):

```bash
./lab9_producer <n> <d>
```
-Consumer:
Run the consumer program in a separate terminal:
```bash
./lab9_consumer <n> <d> 
```
Note: Ensure that you use the same values of n and d for both the producer and consumer.
