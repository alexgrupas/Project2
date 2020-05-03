#include "Options.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <signal.h>

void quit(char*);


int main(int argc, char** argv) {
    setFlags();

    key_t key = 1234;
    char *shm, *s;
    int shmid;
    char c;
    if(shmid = shmget(key, 1024, 0666 | IPC_CREAT) < 0)
        quit("shmget");


    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1)
        quit("shmat");

    s = shm;
    for (c = 'a'; c <= 'z'; c++)
        *s++ = c;

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


    while (*shm != '*')
        sleep(1);

    return 0;
}

void quit(char* str)
{
    perror(str);
    exit(1);
}