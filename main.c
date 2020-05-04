#include "Options.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <signal.h>

//handle alarms
void alarmSigHandler(int);

//function definitions
void quit(char*);


//shared memory data structure
typedef struct {
    int id;
    int index;  //key_t key;
    int clock_seconds;
    int clock_nanoseconds;
} shared_memory;

shared_memory *shmptr;
int shmid;
pid_t childpid;


int main(int argc, char** argv) {
    //handle command line arguments
    setFlags();

    //setup alarm handlers
    signal(SIGALRM, alarmSigHandler);


    key_t key = 1234;

    if((shmid = shmget(key, 1024, 0666 | IPC_CREAT)) < 0)
        quit("shmget");

    if ((shmptr = shmat(shmid, NULL, 0)) == (void *) -1)
        quit("shmat");

    shmptr->clock_seconds = 0;
    shmptr->clock_nanoseconds = 0;


    //Create child and exec user
    childpid = fork();
    if(childpid == -1)
        quit("fork");
    if(childpid == 0)
    {
        execlp("./user", NULL);
        exit(1);
    }

    alarm(2);

    while(1)
    {
        if(shmptr->clock_nanoseconds == 1)
            break;
        shmptr->clock_nanoseconds = shmptr->clock_nanoseconds + 1;
        if(shmptr->clock_nanoseconds > 1000000)
        {

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

void alarmSiHandler(int sig)
{
    int i;

    //clear shared memory
    if((shmctl(shmid, IPC_RMID, NULL)) == -1)
        quit("shmctl");

    //kill child processes
    for(i = 0; i < 1; ++i) {
        if(childpid != 0) {
            kill(childpid, SIGQUIT);
        }
    }

    //free any allocated memory
    free(shmptr);
}