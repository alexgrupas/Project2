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
    while((opt = getopt(argc, argv, "h"))  != -1)
    {
        switch (opt) {
            case 'h':
                printHelpMessage();
                break;

            case '?':
                printf("Question mark case reached switch in setFlags function\n");
        }
    }
}

void printHelpMessage()
{
    printf("This is the help message for now\n");
}