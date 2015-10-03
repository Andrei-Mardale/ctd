#include "fstate.h"


void runStateMachine (int argc, char *argv[]) {
	TState current = &checkArgc;
	while (current != NULL) {
		current = (*checkArgc) (argc, argv);
	}
}

//will check if argc is greater than 1
static TState checkArgc (int argc, char *argv[]) {
	if (argc == 1) return &printDiff;
	return &checkArgv1;
}

//check if date is stored in file
static TState printDiff (int argc, char *argv[]) {	
	FILE *in = fopen(STOREPATH, "r");
	if (in == NULL) {
		printf("No date stored. Try to store one by calling \"ctd [date]\"\n");
		return NULL;
	}
	
	return NULL;
}

//checks if argv[1] is "-c"
static TState checkArgv1 (int argc, char *argv[]) {
	if (!strcmp(argv[1], "-c") return &checkArgc3c;
	return &checkArgc3;
}

//checks if argument count is 3, when first argument is NOT -c
TState checkArgc3 (int argc, char *argv[]) {
	if (argc == 3) return &print2Diff;
	
	return &checkArgc2;
}

//prints a live countdown relative to stored date
TState printCountdownStored (int argc, char *argv[]); //TODO

//prints a live countdown relative to given date
TState printCountdown (int argc, char *argv[]); //TODO

//check if argument count is 3, when first argument is -c
TState checkArgc3c (int argc, char *argv[]) {
	if (argc == 3) return &printCountdown;
	return &printCountdownStored;
}

//store given date
TState storeDate (int argc, char *argv[]);

//check if arugment count is 2
TState checkArgc2 (int argc, char *argv[]) {
	if (argc == 2) return &storeDate;
	printf("Incorrect argument number\n");
	return NULL;
}

//prints differnce between the 2 given dates
TState print2Diff (int argc, char *argv[]); //TODO
