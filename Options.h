//
// Created by Alex Grupas on 4/30/20.
//

#ifndef PROJECT2_OPTIONS_H
#define PROJECT2_OPTIONS_H

#include <stdio.h> //printf
#include <unistd.h> //getopt, pid,
#include <stdlib.h> //exit, atoi

// Flag Declarations
extern int helpFlag;
extern int maxChildFlag;
extern int numChildAtOneTimeFlag;
extern int numToTestFlag;
extern int incrementFlag;

static void initFlags();
void setFlags();
void printHelpMessage();


#endif //PROJECT2_OPTIONS_H
