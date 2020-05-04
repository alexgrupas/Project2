#include "Options.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <signal.h>

//handle signals... ctrl-c and alarm
void alarmSigHandler(int);
void interruptSigHandler(int);

//function definitions
void quit(char*);
void cleanup();


//shared memory data structure
typedef struct {
    int childID[20];
    int clock_seconds;
    int clock_nanoseconds;
} shared_memory;


//

shared_memory *shmptr;
int shmid;
pid_t childpid;


int main(int argc, char** argv) {
    //handle command line arguments
    setFlags();

    //create some shared memory and attach to it
    key_t key = 1234;
    if((shmid = shmget(key, sizeof(shmptr), 0666 | IPC_CREAT)) < 0)
        quit("main: shmget");

    if ((shmptr = shmat(shmid, NULL, 0)) == (void *) -1)
        quit("shmat");

    shmptr->clock_seconds = 0;
    shmptr->clock_nanoseconds = 0;

    //set alarm and signal handlers
    //setup alarm
    signal(SIGALRM, alarmSigHandler);
    signal(SIGINT, interruptSigHandler);
    alarm(2);

    //set child process id's in shared memory
    int inc;
    for(inc = 0; inc < maxChildFlag; ++inc)
    {
        shmptr->childID[inc] = inc + 1;
    }

    int i = 1;
    int incrementedNumber = 0;
    int currentChildProcesses = 0;
    while(1) {
        //increment the clock
        shmptr->clock_nanoseconds += 10000;
        if(shmptr->clock_nanoseconds > 1000000000)
        {
            shmptr->clock_nanoseconds = 0;
            shmptr->clock_seconds += 1;
        }
        //check if we have created the max number of child processes
        if(i == maxChildFlag)
            break;

        //check if we have the correct number of currently running child processes
        if(currentChildProcesses != numChildAtOneTimeFlag)
        {
            childpid = fork();
            if(childpid == -1)
                quit("fork");

            if(childpid == 0) { //we are in the child process
                //compute number to check for primality
                incrementedNumber = numToTestFlag + (i - 1 * incrementFlag);

                execl("./user", "./user", i, incrementedNumber, (char *) 0);
                currentChildProcesses += 1;
                i += 1;
                }
            }
            int k;
            for(k = 0; k < maxChildFlag; ++k)
            {
                if(shmptr->childID[k] != (k+1))
                {
                    currentChildProcesses--;
                    printf("child %d childID: %d\n", k+1, shmptr->childID[k]);
                }
            }
        }


    //detach shmptr
    if((shmdt(shmptr)) == -1)
        quit("shmdt");

    //destroy shared mem
    if((shmctl(shmid, IPC_RMID, NULL)) == -1)
        quit("shmctl");

    return 0;
}

void quit(char* str)
{
    perror(str);
    exit(1);
}

void alarmSigHandler(int sig)
{
    int i;

    //kill child processes
    for(i = 0; i < 1; ++i) {
        if(childpid != 0) {
            kill(childpid, SIGQUIT);
        }
    }

    wait(NULL);

    //detach shmptr
    if((shmdt(shmptr)) == -1)
        quit("shmdt");

    //clear shared memory
    if((shmctl(shmid, IPC_RMID, NULL)) == -1)
        quit("shmctl");

    //free any allocated memory
    //free(shmptr);

    exit(0);
}

void interruptSigHandler(int sig)
{
    int i;

    //kill child processes
    for(i = 0; i < 1; ++i) {
        if(childpid != 0) {
            kill(childpid, SIGINT);
        }
    }

    wait(NULL);

    //detach shmptr
    if((shmdt(shmptr)) == -1)
        quit("shmdt");

    //clear shared memory
    if((shmctl(shmid, IPC_RMID, NULL)) == -1)
        quit("shmctl");

    //free any allocated memory
    //free(shmptr);

    exit(0);
}

void cleanup()
{
    //detach shmptr
    if((shmdt(shmptr)) == -1)
        quit("shmdt");

    //clear shared memory
    if((shmctl(shmid, IPC_RMID, NULL)) == -1)
        quit("shmctl");
}