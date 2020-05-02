//
// Created by Alex Grupas on 4/30/20.
//

#include "Options.h"

int helpFlag;
int maxChildFlag;
int numChildAtOneTimeFlag;
int numToTestFlag;
int incrementFlag;

static void initFlags()
{
    helpFlag = 0;
    maxChildFlag = 0;
    numChildAtOneTimeFlag = 0;
    numToTestFlag = 0;
    incrementFlag = 0;
}

void setFlags(int argc, char** argv)
{
    initFlags();

    int opt;
    while((opt = getopt(argc, argv, "hn:"))  != -1)
    {
        switch (opt) {
            case 'h':
                printHelpMessage();
                exit(0);

            case 'n':
                maxChildFlag = atoi(optarg);
                printf("%d\n", maxChildFlag);
                break;

            case '?':
                printf("Make sure to print arguments along with the flags\n");
                printHelpMessage();
                exit(0);
        }
    }
}

void printHelpMessage()
{
    printf("This is the help message for now\n");
}