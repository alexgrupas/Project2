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


//global variables

shared_memory *shmptr;
int shmid;
pid_t childpid;
int childDead[20];
int pidArr[20];
char cid[12];
char incrementedNum[25];
FILE *file;
int  childrenCreated = 0;

int main(int argc, char** argv) {
    //handle command line arguments
    setFlags();

    //create log file...erase it if it already exist...then close
    file = fopen(outputFileName, "w+");
    fclose(file);

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
        childDead[inc] = 0;
        shmptr->childID[inc] = inc + 1;
    }

    int incrementedNumber = 0;
    int totalDeadChildren = 0;
    //launch numChildAtOneTime many child processes
    int j;
    for(j  =  0; j < numChildAtOneTimeFlag; ++j)
    {
        childpid = fork();
        if(childpid == -1)
            quit("fork");

        if(childpid > 0)
        {
            pidArr[j] = childpid;
            ++childrenCreated;
            file = fopen(outputFileName, "a");
            fprintf(file, "Launched child %d at %d s and %d ns.\n", shmptr->childID[j], shmptr->clock_seconds, shmptr->clock_nanoseconds);
            fclose(file);
        }

        if(childpid == 0) {
            incrementedNumber = numToTestFlag + (j * incrementFlag);
            sprintf(cid, "%d", j+1);
            sprintf(incrementedNum, "%d", incrementedNumber);
            if((execlp("./user", "./user", cid, incrementedNum, (char *)0)) == -1)
                quit("execlp");
        }
    }

    while(1) {
        //increment the clock
        shmptr->clock_nanoseconds += 10; //change this back to 10000
        if(shmptr->clock_nanoseconds > 1000000000)
        {
            shmptr->clock_nanoseconds = 0;
            shmptr->clock_seconds += 1;
        }
        //check to see if any child process has terminated
        int k;
        for(k = 0; k < maxChildFlag; ++k)
        {
            if((shmptr->childID[k] != (k+1)) && (childDead[k] == 0))
            {
                childDead[k] = 1;
                totalDeadChildren += 1;
                pidArr[k] = -1;

                file = fopen(outputFileName, "a");
                fprintf(file, "Child %d was found to be terminated at %d s and %d ns\n", k+1, shmptr->clock_seconds, shmptr->clock_nanoseconds);
                fclose(file);
                if(totalDeadChildren >= maxChildFlag)
                {
                    continue;
                } else {
                    childpid = fork();
                    if(childpid == -1)
                        quit("fork");

                    if(childpid > 0)
                    {
                        pidArr[childrenCreated] = childpid;
                        ++childrenCreated;

                        file = fopen(outputFileName, "a");
                        fprintf(file, "Launched child %d at %d s and %d ns.\n", childrenCreated, shmptr->clock_seconds, shmptr->clock_nanoseconds);
                        fclose(file);
                    }
                    if(childpid == 0) {
                        incrementedNumber = numToTestFlag + (totalDeadChildren * incrementFlag);
                        sprintf(cid, "%d", totalDeadChildren + 1);
                        sprintf(incrementedNum, "%d", incrementedNumber);
                        if((execl("./user", "./user", cid, incrementedNum, (char *)0)) == -1)
                            quit("execlp");
                    }
                }
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
    int i, num;

    //kill child processes
    for(i = 0; i < maxChildFlag; ++i) {
        if(pidArr[i] != -1) {
            kill(pidArr[i], SIGQUIT);
        }
    }

    wait(NULL);
    file = fopen(outputFileName, "a");
    for(i = 0; i < maxChildFlag; ++i)
    {
        num = numToTestFlag + (i * incrementFlag);
        if(shmptr->childID[i] == -1)
        fprintf(file, "Child %d checked %d and didn't have time to finish checking for primality.\n", i+1, num);
        else if(shmptr->childID[i] > 0)
        {
            fprintf(file, "Child %d checked %d and found it to be prime.\n", i+1, num);
        } else {
            fprintf(file, "Child %d checked %d and found it to be not prime.\n", i+1, num);

        }
    }
    fclose(file);

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