#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

#define READ 0
#define WRITE 1

void welcome()
{
    printf("Hi,\nThis Program is a Simulation of IPC using pipes, example of how to initialize array of integers in a parent process, ");
    printf("passing it to child process to execute the sum of array values, ");
    printf("then pass the sum value back to parent process.\n");
    printf("This Simulation is intended to work perfectly on kernel architecture:\n");
    printf("Linux kali 4.0.0-kali1-amd64 #1 SMP Debian 4.0.4-1+kali2 (2015-06-03) x86_64 GNU/Linux \n");
    printf("Undesired Output may occur while running on different kernel architecture!");
}

int main()
{
    welcome();

    int i;

    //create buffer in memory
    int n;
    printf("\n\nEnter Buffer Size: ");
    scanf("%d", &n);
    int buffer[n];

    //create 2 pipes to read and write
    int PIPE_1_FD[2];
    int PIPE_2_FD[2];
    if (pipe(PIPE_1_FD) == -1 || pipe(PIPE_2_FD) == -1)
    {
        fprintf(stderr,"PIPE CREATION FAILED");
        return 1;
    }

    //fork child process
    pid_t PROCESS_ID;
    PROCESS_ID = fork();

    if (PROCESS_ID < 0)
    {
        fprintf(stderr, "FORK CHILD PROCESS FAILED");
        return 1;
    }

    // parent process
    if (PROCESS_ID > 0)
    {
        //enter buffer values
        printf("Enter Buffer Values:\n");
        for(i = 0; i < n;i++)
        {
            scanf("%d", &buffer[i]);
        }

        //close "READ end" of PIPE_1 permenantly for parent process because PIPE_1 is used only as the writer of parent process
        close(PIPE_1_FD[READ]);

        //start writing to "WRITE end" of PIPE_1
        for(i = 0 ; i < n; i++)
        {
            sleep(1);
            write(PIPE_1_FD[WRITE],&buffer[i],sizeof(int));
            printf("Parent Process(%d) writes single byte buffer value : %d\n", getpid(), buffer[i]);
            //write a single byte buffer value then parent process sleep and child process start executing on this single byte
        }
        close(PIPE_1_FD[WRITE]);
        //finish writing buffer values to "WRITE end" of PIPE_1 so must close it

        sleep(1);

        //close "WRITE end" of PIPE_2 permenantly for parent process because PIPE_2 is used only as the reader of parent process
        close(PIPE_2_FD[WRITE]);

        //start reading single value from PIPE_2 "READ end"
        int READ_VALUE = 0;
        read(PIPE_2_FD[READ],&READ_VALUE,sizeof(READ_VALUE));
        printf("Parent Process(%d) read sum buffer values : %d\n", getpid(), READ_VALUE);
        close(PIPE_2_FD[READ]);
        //finish reading single value from PIPE_2 "READ end", so must close it
    }

    //parent process
    else
    {
        //close "WRITE end" of PIPE_1 permenantly for child process because PIPE_1 is used only as the reader of child process
        close(PIPE_1_FD[WRITE]);

        //start reading single buffer value one byte by one from PIPE_1 "READ end", and sum all buffer values into "SUM"
        int SUM = 0;
        for(i = 0; i < n; i++)
        {
            read(PIPE_1_FD[READ],&buffer[i],sizeof(int));
            printf("Child Process(%d) read single byte buffer value : %d\n\n", getpid(), buffer[i]);
            SUM += buffer[i];
        }
        close(PIPE_1_FD[READ]);
        //finish reading buffer values from PIPE_1 "READ end" so must close it

        //close "READ end" of PIPE_2 permenantly for child process because PIPE_2 is used only as the writer of child process
        close(PIPE_2_FD[READ]);

        //start wrting SUM value to "READ end" of PIPE_2
        write(PIPE_2_FD[WRITE],&SUM,sizeof(SUM));
        printf("Child Process(%d) writes sum value : %d\n\n", getpid(), SUM);
        close(PIPE_2_FD[WRITE]);
        //finish wrting SUM value to "READ end" of PIPE_2, so must close it
    }

    return 0;
}
