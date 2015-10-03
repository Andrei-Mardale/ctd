#include "fstate.h"

//check if argument count is 1
void checkArgc (State *s);

//prints difference between stored date and present
void printDiff (State *s);

//checks if argv[1] is "-c" or "-s"
void checkArgv1 (State *s);

//checks if argument when first argument is NOT special parameter
void checkArgc3 (State *s);

//check if argument count when first argument is -c
void checkArgc3c (State *s);

//check if argument count when first argument is -s
void checkArgc3s (State *s);

//store given date
void storeDate (State *s);

//prints differnce between the 2 given dates
void print2Diff (State *s);

//prints a live countdown relative to stored date
void printCountdownStored (State *s);

//prints a live countdown relative to given date
void printCountdown (State *s);

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
	printf("%d w %d d %d h %d m %d s",
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
		printf("No date stored. Try to store one by calling \"ctd -s [date]\"\n");
		return NULL;
	}
	
	double jstored;
	fread(&jstored, sizeof(double), 1, in); 
	fclose(in);
	
	TDate *present = currentDate(NULL);
	
	double jpresent = toJulian(present);
	
	TDateDiff *diff = tdiff(jstored, jpresent);
	
	printDateDiff(diff);
	printf("\n");
	
	free(present);
	free(diff);

}

void checkArgv1 (State *s) {
	if (!strcmp(s->argv[1], "-c")) {
		s->next = &checkArgc3c;
	} else if (!strcmp(s->argv[1], "-s")) {
		s->next = &checkArgc3s;
	} else {
		s->next = &checkArgc3;
	}
}

void checkArgc3 (State *s) {
	if (s->argc == 3) {
		s->next = &print2Diff;
	} else {	
		s->next = NULL;
	}
}

void checkArgc3c (State *s) {
	if (s->argc == 3) {
		s->next = &printCountdown;
	} else if (s->argc == 2) {
		s->next = &printCountdownStored;
	} else {
		s->next = NULL;
	}
}

void checkArgc3s (State *s) {
	if (s->argc == 3) {
		s->next = &storeDate;
	} else {
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
	
	TDate *date = parse(s->argv[2]);
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
	
	fclose(out);
	free(date);	
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
	printf("\n");
	
	free(diff);
	free(date1);
	free(date2);
}

//prints difference between jvalue2 and jvalue1 every second
void countdown (double jvalue1, double jvalue2) {
	TDateDiff *diff = tdiff(jvalue2, jvalue1);
	while (
		diff->second || 
		diff->minute || 
		diff->hour || 
		diff->day || 
		diff->week) {
		
		printf(CLEARLINE);
		printDateDiff(diff);		
		fflush(stdout);
		decreaseDiff(diff);
		sleep(1);
	}
}
		
		
//prints a live countdown relative to stored date
void printCountdownStored (State *s) {
	s->next = NULL;
	
	FILE *in = fopen(makeFilePath(), "rb");
	if (!in) {
		printf("No date stored. Try to store one by calling \"ctd -s [date]\"\n");
		return;
	}
	
	double jdate;
	fread(&jdate, sizeof(double), 1, in);
	fclose(in); 
	
	TDate *now = currentDate(NULL);
	double jnow = toJulian(now);
	
	countdown (jnow, jdate);	
}

//prints a live countdown relative to given date
void printCountdown (State *s) {
	s->next = NULL;
	
	TDate *date = parse(s->argv[2]);
	if (!date) {
		printf("Could not parse date. Check formatting\n");
		return;
	}
	
	TDate *now = currentDate(NULL);
	
	countdown (toJulian(now), toJulian(date));	
}
