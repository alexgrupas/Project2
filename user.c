//
// Created by Alex Grupas on 5/3/20.
//
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdlib.h>

void quit(char*);

typedef struct {
    int id;
    int index;  //key_t key;
    int clock_seconds;
    int clock_nanoseconds;
} shared_memory;

int main(int argc, char** argv)
{
    int shmid;
    int clockStart;
    key_t key;
    shared_memory *shmptr;

    key = 1234;

    if ((shmid = shmget(key, 1024, 0666)) < 0)
        quit("shmget");

    if ((shmptr = shmat(shmid, NULL, 0)) == (void *) -1)
        quit("user: shmat");

    printf("seconds: %d\n", shmptr->clock_seconds);
    clockStart = shmptr->clock_seconds;
    while(1) {
        if (shmptr->clock_seconds > (clockStart + 1000000000))
        {
            printf("seconds: %d\n", shmptr->clock_seconds);
            printf("clock start: %d\n", clockStart);
            break;
        }

    }
    //detach shmptr
    shmptr->clock_nanoseconds = 1;
    if((shmdt(shmptr)) == -1)
        quit("shmdt");


    return 0;
}

void quit(char* str)
{
    perror(str);
    exit(1);
}