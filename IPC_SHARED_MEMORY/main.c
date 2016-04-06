#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void welcome()
{
    printf("Hi,\nThis Program is a Simulation of IPC using shared memory, example of how to initialize array of integers in a parent process, ");
    printf("passing it to child process to execute the sum of array values, ");
    printf("then pass the sum value back to parent process.\n");
    printf("This Simulation is intended to work perfectly on kernel architecture:\n");
    printf("Linux kali 4.0.0-kali1-amd64 #1 SMP Debian 4.0.4-1+kali2 (2015-06-03) x86_64 GNU/Linux \n");
    printf("Undesired Output may occur while running on different kernel architecture!");
    printf("\n*WARNING* Shared Memory Segment creation with unique key makes it impossible to exceed its first given memory range! ");
    printf("If you want to extend memory range, change the segment key or reboot your machine.\n\n");
}

int main()
{
    welcome();

    //start creating shared memory segment
    int *SEGMENT;

    key_t  SEGEMENT_KEY = 3030;

    printf("Enter Segment Size: ");
    int S;
    scanf("%d",&S);

    //the segment buffer will hold values ,then sum of values, then size of segment
    //example if S = 5, then we will create segment of size 7 where the first 5 are values, #6 is the sum of values, #7 is the segment size which is 7
    int SEGMENT_ID;
    if((SEGMENT_ID = shmget(SEGEMENT_KEY, S*sizeof(int)+2, IPC_CREAT | 0666)) < 0)
    {
        fprintf(stderr,"\nERROR CANNOT CREATE SHARED MEMORY SEGMENT!\n");
        exit(1);
    }

    SEGMENT = (int *)shmat(SEGMENT_ID, 0, 0);
    SEGMENT[S+1] = S;
    //done creating shared memory segment

    //start input buffer values
    printf("Enter Numbers Values: \n");
    int i;
    for(i = 0; i < S; i++)
    {
        scanf("%d", SEGMENT+i);
    }

    printf("\nWritting to memory succesful...\n");
    //done input buffer values

    //fork child process
    pid_t PROCESS_ID;
    PROCESS_ID = fork();
    if (PROCESS_ID < 0)
    {
        fprintf(stderr, "FORK CHILD PROCESS FAILED");
        return 1;
    }

    //parent process sleep and child process execute on shared memory segment
    sleep(1);

    //child process
    if (PROCESS_ID == 0)
    {
        printf("\nI am Luke Skywalker\n");
        //initialize the index[S] element for sum buffer values
        SEGMENT[S] = 0;
        for (i = 0; i < S; i++)
        {
            SEGMENT[S] += SEGMENT[i];
        }
        printf("Sum of values in shared memory segment: %d", SEGMENT[S]);
        printf("\nChild Process Done...\n");
    }

    //parent process
    else
    {
        sleep(1);
        printf("\nI am your Father\n");
        printf("Sum of values in shared memory segment: %d\n", SEGMENT[S]);
        printf("Parent Process Done...\n");
    }

    //deattach shared memory segment
    shmdt((void *) SEGMENT);

    return 0;
}
