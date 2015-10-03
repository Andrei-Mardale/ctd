#include <stdio.h>
#include <sys/stat.h>
#include "date.h"

#ifndef _TSTATE_
#define _TSTATE_

#define STOREPATH "~/.ctd/date"
#define FOLDERPATH "~/.ctd"

//function that will return pointer to the next function that shall be executed
struct state;
typedef void (*func) (struct state *s);

typedef struct state {
	func next;
	int argc;
	char **argv;
} State;
	

void runStateMachine (int argc, char *argv[]);

#endif

