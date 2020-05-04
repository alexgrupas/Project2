//
// Created by Alex Grupas on 5/3/20.
//
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>


//handle signals
void quitSigHandler(int);
void interruptSigHandler(int);

//function definitions
void quit(char*);
void detach_and_quit();

typedef struct {
    int childID[20];
    int clock_seconds;
    int clock_nanoseconds;
} shared_memory;

//need this to be global for signal functions
shared_memory *shmptr;
int ID;

int main(int argc, char** argv)
{
    ID = atoi(argv[1]);
    //register signals
    signal(SIGQUIT, quitSigHandler);
    signal(SIGINT, interruptSigHandler);

    //get shared memory and attach it
    int shmid;
    key_t key = 1234;
    if ((shmid = shmget(key, sizeof(shmptr), 0666)) < 0)
        quit("user: shmget");

    if ((shmptr = shmat(shmid, NULL, 0)) == (void *) -1)
        quit("user: shmat");


    //check clock for start time and compute end time
    int clockStartNano = shmptr->clock_nanoseconds;
    int clockStartSec = shmptr->clock_seconds;
    int clockStopNano, clockStopSec;
    if(clockStartNano > 999000000)
    {
        clockStopNano = clockStartNano - 999000000;
        clockStopSec = clockStartSec + 1;
    } else {
        clockStopNano = clockStartNano + 1000000;
        clockStopSec = clockStartSec;
    }

    int numberToCheck = atoi(argv[2]);
    printf("%d\n", numberToCheck);
    int not_prime = 0;
    int i;
    for(i = 2; i <= numberToCheck / 2; ++i)
    {
        if(i % 7 == 0)
        {
            if((shmptr->clock_nanoseconds > clockStopNano) && (shmptr->clock_seconds >= clockStopNano))
            {
                shmptr->childID[ID] = -1;
                detach_and_quit();
            }
        }
        if(numberToCheck % i == 0) {
            not_prime = 1;
            shmptr->childID[ID] = -1 * numberToCheck;
            detach_and_quit();
            break;
        }
    }

    //If we got here then the number should be prime
    shmptr->childID[ID] = numberToCheck;

    //detach shmptr
    if((shmdt(shmptr)) == -1)
        quit("shmdt");

    return 0;
}

void quit(char* str)
{
    perror(str);
    exit(1);
}

void detach_and_quit()
{
    if((shmdt(shmptr)) == -1)
        quit("shmdt");
    exit(0);
}

void quitSigHandler(int sig)
{
    shmptr->childID[ID] = -1;
    if((shmdt(shmptr)) == -1)
        quit("shmdt");

    //printf("quitSigHandler\n\n");
    exit(0);
}

void interruptSigHandler(int sig)
{
    shmptr->childID[ID] = -1;
    if((shmdt(shmptr)) == -1)
        quit("shmdt");

    //printf("interruptSigHandler\n\n");
    exit(0);
}