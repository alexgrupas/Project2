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

}

void printHelpMessage()
{
    printf("Usage: -n [int] -s [int] -b [int] -i [int] -o [output file name]\n");
    printf("\"-n\" Maximum number of child processes to be created by oss.\n");
    printf("\"-s\" Maximum number of child processes to exist at one  timie.\n");
    printf("\"-b\" First number to check for primality.\n");
    printf("\"-i\" Increment the number we check for primality by this much during each child process creation.\n");
    printf("\"-o\" Output file name.\n");
    printf("If these options are not given by the user then the defaults will be as follows:\n");
    printf("n = 4\ns = 2\nb = 101\ni = 4\no = output.log\n");

}