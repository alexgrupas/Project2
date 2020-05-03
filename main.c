#include "Options.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <signal.h>

void quit(char*);

typedef struct {
    int id;
    int index;  //key_t key;
    int clock_seconds;
    int clock_nanoseconds;
} shared_memory;

int main(int argc, char** argv) {
    setFlags();

    key_t key = 1234;
    shared_memory *shmptr;
    int shmid;
    if((shmid = shmget(key, 1024, 0666 | IPC_CREAT)) < 0)
        quit("shmget");

    if ((shmptr = shmat(shmid, NULL, 0)) == (void *) -1)
        quit("shmat");

    shmptr->clock_seconds = 100;

    //detach shmptr
    if((shmdt(shmptr)) == (void *) -1)
        quit("shmdt");

    //Create child and exec user
    pid_t childpid;
    childpid = fork();
    if(childpid == -1)
        quit("fork");
    if(childpid == 0)
    {
        execlp("./user", NULL);
        exit(1);
    }

    wait(NULL);

    return 0;
}

void quit(char* str)
{
    perror(str);
    exit(1);
}