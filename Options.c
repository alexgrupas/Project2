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
    while((opt = getopt(argc, argv, "hn:s:b:i:o:"))  != -1)
    {
        switch (opt) {
            case 'h':
                printHelpMessage();
                exit(0);

            case 'n':
                maxChildFlag = atoi(optarg);
                break;

            case 's':
                numChildAtOneTimeFlag = atoi(optarg);
                break;

            case 'b':
                numToTestFlag = atoi(optarg);
                break;

            case 'i':
                incrementFlag = atoi(optarg);
                break;

            case 'o':
                outputFileName = strdup(optarg);
                break;

            case '?':
                if(optopt == 'n' || optopt == 's' || optopt == 'b' || optopt == 'i') {
                    fprintf(stderr, "Option -%c requires an argument. \n", optopt);
                    exit(1);
                }
                break;
        }
    }

    if(maxChildFlag == 0)
        maxChildFlag = 4;
    if(numChildAtOneTimeFlag == 0)
        numChildAtOneTimeFlag = 2;
    if(numToTestFlag == 0)
        numToTestFlag = 101;
    if(incrementFlag == 0)
        incrementFlag = 4;
    if(outputFileName == NULL)
        outputFileName = "output.log";


    //For testing
    printf("%d\n", maxChildFlag);
    printf("%d\n", numChildAtOneTimeFlag);
    printf("%d\n", numToTestFlag);
    printf("%d\n", incrementFlag);
}

void printHelpMessage()
{
    printf("This is the help message for now\n");
    exit(1);
}