#include "Options.h"
#include <sys/ipc.h>
#include <sys/shm.h>



int main(int argc, char** argv) {
    setFlags();

    key_t key = ftok("test", 20);
    int shmid = shmget(key,1024,0666|IPC_CREAT);
    if (shmid == -1) {
        perror("Shared memory");
        return 0;
    }
    char *str = (char*) shmat(shmid,(void*)0,0);
    str = "This is a test";
    printf("Data written in memory: %s\n",str);
    //detach from shared memory
    shmdt(str);

    //clear the shared memory..
    shmctl(shmid,IPC_RMID,NULL);


    pid_t childpid;
    childpid = fork();
    if(childpid == -1)
        perror("Failed to create child process\n");
    if(childpid == 0)
    {
        execlp("./user", NULL);
        exit(1);
    }
    return 0;
}