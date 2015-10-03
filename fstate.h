#include <stdio.h>
#include <sys/stat.h>
#include "date.h"

#ifndef _TSTATE_
#define _TSTATE_

#define STOREPATH ~/.ctd/date
#define STOREFOLDER ~/.ctd/
//function that will return pointer to the next function that shall be executed
typedef struct state {
	struct state * (*f) (int argc, char *argv[]); 
} *TState;

void runStateMachine (int argc, char *argv[]);

#endif

