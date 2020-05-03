//
// Created by Alex Grupas on 5/3/20.
//
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdlib.h>

void quit(char*);

int main(int argc, char** argv)
{
    int shmid;
    key_t key;
    char *shm, *s;

    key = 1234;

    if ((shmid = shmget(key, 1024, 0666)) < 0)
        quit("shmget");

    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1)
        quit("shmat");

    //Now read what the server put in the memory.
    for (s = shm; *s != '\0'; s++)
        putchar(*s);
    putchar('\n');

    /*
     *Change the first character of the
     *segment to '*', indicating we have read
     *the segment.
     */
    *shm = '*';

    return 0;
}

void quit(char* str)
{
    perror(str);
    exit(1);
}