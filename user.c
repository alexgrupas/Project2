//
// Created by Alex Grupas on 5/3/20.
//
#include <stdio.h>

int main(int argc, char** argv)
{

    key_t key = ftok("test", 20);
    int shmid = shmget(key,1024,0666|IPC_CREAT);
    if (shmid == -1) {
        perror("Shared memory");
        return 0;
    }
    char *str = (char*) shmat(shmid,(void*)0,0);

    //detach from shared memory
    shmdt(str);

    //print shared memory
    printf("%s\n", str);

    //clear the shared memory..
    shmctl(shmid,IPC_RMID,NULL);
    return 0;
}
