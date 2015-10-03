#include "fstate.h"

//check if argument count is 1
void checkArgc (State *s);

//prints difference between stored date and present
void printDiff (State *s);

//checks if argv[1] is "-c"
void checkArgv1 (State *s);

//checks if argument count is 3, when first argument is NOT -c
void checkArgc3 (State *s);

//check if argument count is 3, when first argument is -c
void checkArgc3c (State *s);

//check if arugment count is 2 when first argument is NOT -c
void checkArgc2 (State *s);

//check if arugment count is 2 when first argument is -c
void checkArgc2c (State *s);

//store given date
void storeDate (State *s); //TODO

//prints differnce between the 2 given dates
void print2Diff (State *s); //TODO

//prints a live countdown relative to stored date
void printCountdownStored (State *s); //TODO

//prints a live countdown relative to given date
void printCountdown (State *s); //TODO

//this simulates the begaviour of tha state machine
void runStateMachine (int argc, char *argv[]) {
	State s = {&checkArgc, argc, argv};
	
	while (s.next != NULL) {
		(*s.next)(&s);
	}
}

//returns string which represents path to storage directory
char * makeDirPath () {
	char *s = getenv("HOME");
	strcat (s, "/.ctd/");
	
	return s;
}

//returns string which represents path to storage file
char * makeFilePath () {
	char *s = makeDirPath();
	strcat(s, "date");
	
	return s;
}

void checkArgc (State *s) {
	if (s->argc == 1) {
		s->next = &printDiff;
	} else {
		s->next = &checkArgv1;
	}
}

//prints date difference to output
void printDateDiff (TDateDiff *diff) {
	printf("%d w %d d %d h %d m %d s\n",
		diff->week,
		diff->day,
		diff->hour,
		diff->minute,
		diff->second);
}

//prints difference between stored date and present
void printDiff (State *s) {	
	s->next = NULL;
	FILE *in = fopen(makeFilePath(), "rb");
	if (in == NULL) {
		printf("No date stored. Try to store one by calling \"ctd [date]\"\n");
		return NULL;
	}
	
	double jstored;
	fread(&jstored, sizeof(double), 1, in); 
	
	TDate *present = currentDate();
	
	double jpresent = toJulian(present);
	
	TDateDiff *diff = tdiff(jstored, jpresent);
	
	printDateDiff(diff);
	
	free(present);
	free(diff);
	fclose(in);
}

//checks if argv[1] is "-c"
void checkArgv1 (State *s) {
	if (!strcmp(s->argv[1], "-c")) {
		s->next = &checkArgc3c;
	} else {
		s->next = &checkArgc3;
	}
}

//checks if argument count is 3, when first argument is NOT -c
void checkArgc3 (State *s) {
	if (s->argc == 3) {
		s->next = &print2Diff;
	} else {	
		s->next = &checkArgc2;
	}
}

//check if argument count is 3, when first argument is -c
void checkArgc3c (State *s) {
	if (s->argc == 3) {
		s->next = &printCountdown;
	} else {
		s->next = &checkArgc2c;
	}
}

//check if arugment count is 2 when first argument is NOT -c
void checkArgc2 (State *s) {
	if (s->argc == 2) {
		s->next = &storeDate;
	} else {
		printf("Incorrect argument number\n");
		s->next = NULL;
	}
}

//check if arugment count is 2 when first argument is -c
void checkArgc2c (State *s) {
	if (s->argc == 2) {
		s->next = &printCountdownStored;
	} else {
		printf("Incorrect argument number\n");
		s->next = NULL;
	}
}

//store given date
void storeDate (State *s) {
	s->next = NULL;
	//check if folder exists
	struct stat fStat;
	
	char *dirName = makeDirPath();
	if (stat(dirName, &fStat) == -1) {
		if (mkdir(dirName, 0700) == -1) {
			printf("Could not create directory to store data. Check privileges\n");
			return;
		}
	}
	
	TDate *date = parse(s->argv[1]);
	if (!date) {
		printf("Could not parse date. Check formatting\n");
		return;
	}
	
	
	FILE *out = fopen(strcat(dirName, "date"), "wb");
	if (!out) {
		printf("Could not open file to store data. Check privileges\n");
		return;
	}
	
	double jvalue = toJulian(date);
	fwrite(&jvalue, sizeof(double), 1, out);
	
	free(date);
	fclose(out);
}
	
	
//prints differnce between the 2 given dates
void print2Diff (State *s) {
	s->next = NULL;
	
	TDate *date1 = parse(s->argv[1]);
	TDate *date2 = parse(s->argv[2]);
	
	if (!date1 || !date2) {
		printf("Could not parse dates. Check formatting\n");
		return;
	}
	
	TDateDiff *diff = tdiff(toJulian(date2), toJulian(date1));
	
	printDateDiff(diff);
	
	free(diff);
	free(date1);
	free(date2);
}

//prints a live countdown relative to stored date
void printCountdownStored (State *s) {
	s->next = NULL;
	printf("TODO\n"); //TODO
}

//prints a live countdown relative to given date
void printCountdown (State *s) {
	s->next = NULL;
	printf("TODO\n");
} //TODO
