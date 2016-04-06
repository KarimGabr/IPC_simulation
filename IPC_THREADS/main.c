#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>

#define MAX_THREADS 5
#define BUFFER_LENGTH 10

void welcome()
{
    printf("Hi,\nThis Program is a Simulation of IPC using threads, example of how to find maximum of array of integers using multithreading.\n");
    printf("This Simulation is intended to work perfectly on kernel architecture:\n");
    printf("Linux kali 4.0.0-kali1-amd64 #1 SMP Debian 4.0.4-1+kali2 (2015-06-03) x86_64 GNU/Linux \n");
    printf("Undesired Output may occur while running on different kernel architecture!");
    printf("If you encountered a problem in your IDE during compilation, please compile in terminal using this command:\n gcc -pthread -o main main.c\n");
    printf("Then run the executable with command: ./main\n");

}

typedef struct input_struct
{
    int* A;
    int LEFT;
    int RIGHT;
    int* RESULT;
} INPUT_DATA;

void MAX(void *ptr)
{
    pthread_t ID = pthread_self();
    printf("Thread %d, yes commander\n", ID);
    INPUT_DATA* DATA;
    DATA = (INPUT_DATA*)ptr;
    int i;
    int m = INT_MIN;
    for(i = DATA->RIGHT; i <= DATA->LEFT; i++)
    {
        if(DATA->A[i] > m) m = DATA->A[i];
    }
    *(DATA->RESULT) = m;
    printf("Thread %d, roger that and moving out\n", ID);
    pthread_exit(0);
}

int main()
{
    welcome();
    int i;
    int BUFFER[BUFFER_LENGTH];

    printf("Enter Array values : \n");
    for(i = 0; i < BUFFER_LENGTH; i++)	scanf("%d", &BUFFER[i]);

    pthread_t THREADS[MAX_THREADS];

    for(i = 0; i < MAX_THREADS; i++) THREADS[i] = 0;

    int RESULTS[MAX_THREADS];
    INPUT_DATA INPUTS[MAX_THREADS];

    int PART = BUFFER_LENGTH / 5;
    int REMAINDER = BUFFER_LENGTH % 5;
    int START = 0;

    for(i = 0; i < MAX_THREADS && START < BUFFER_LENGTH; i++)
    {
        INPUTS[i].A = BUFFER;
        INPUTS[i].RIGHT = START;
        INPUTS[i].LEFT = START + PART - 1;
        START = INPUTS[i].LEFT + 1;
        INPUTS[i].RESULT = &RESULTS[i];
        if(i + 1 == MAX_THREADS || START > (i * PART + REMAINDER))
        {
            INPUTS[i].LEFT += REMAINDER;
            START += REMAINDER;
        }
        pthread_create(&THREADS[i], NULL, (void*)MAX, (void*)&INPUTS[i]);
    }

    int MAXIMUM = INT_MIN;
    for(i = 0; i < MAX_THREADS; i++)
    {
        if(THREADS[i] != 0)
        {
            pthread_join(THREADS[i], NULL);
            if(RESULTS[i] > MAXIMUM) MAXIMUM = RESULTS[i];
        }
    }

    printf("\n\nMaximum: %d\n", MAXIMUM);
}
