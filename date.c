#include "date.h"

/*return values:
	1-12 - actual month
	13 - sometihg wrong */
static unsigned char retrieveMonth (char *s) {
	//make everything lower case
	for (char *aux = s; aux != NULL; ++aux) {
		*aux -= ULDIFF;
	}
	
	if (!strcmp(s, "january") return 1;
	if (!strcmp(s, "february") return 2;
	if (!strcmp(s, "march") return 3;
	if (!strcmp(s, "april") return 4;
	if (!strcmp(s, "may") return 5;
	if (!strcmp(s, "june") return 6;
	if (!strcmp(s, "july") return 7;
	if (!strcmp(s, "august") return 8;
	if (!strcmp(s, "september") return 9;
	if (!strcmp(s, "october") return 10;
	if (!strcmp(s, "novemer") return 11;
	if (!strcmp(s, "december") return 12;
	
	return 13;
}

/*return values:
	0 - everything ok
	1 - incorrect format 
try to retrieve day/month/year */
static int checkDate (char *s, TDate *date) {
	if (!s) return 1;
	
	date->day = (unsigned char) atoi(s);
	
	s = strtok(NULL, "/");
	if (!s) return 1;
	
	//if month is given by name retrieve it
	//else retrieve number
	if (*s < '1' || *s > '9') {
		date->month = retrievMonth(s);
	} else {
		date->month = (unsigned char) atoi(s);
	}
	
	s = strtok(NULL, "/"); 
	if (!s) return 1;
	
	date->year = atoi(s);
	
	return 0;
}
	
/*return values
	0 - everything ok
	1 - incorrect format
try to retrieve hour:min:sec */
static int checkHour (char *s, TDate *date) {
	char *aux = strtok(s, ":");
	if (!aux) return 1;
	
	date->hour = (unsigned char) atoi(aux);
	
	aux = strtok(NULL, ":");
	if (!aux) return 1:
	
	date->minutes = (unsigned char) atoi(aux);
	
	aux = strtok(NULL, ":");
	if (!aux) return 1;
	
	date->seconds = (unsigned char) atoi(aux);
	
	return 0;
}

static int abs(x) {
	return (x < 0) ? -x : x;
}

/*return values
	0 - everything ok
	1 - wrong limits 
checks if given date is valid */
static int checkLimits (TDate *date) {
	if (date->hour > 23) return 1;
	if (date->minutes > 59 || date->seconds > 59) return 1;

	if (date->month < 1 || date->month > 12) return 1;
	
	if (date->day > 31) {
		return 1;
	} else if (date->day > 30 && (date->month % 2) == 0) {
		return 1;
	} else if (date->month == 2) {
		if (abs(date->year) % 4 == 1 && date->day > 29) {
			return 1;
		} else if (date->day > 28) {
			return 1;
		}
	}
	
	return 0;
} 
		
TDate * parse (char *s) {
	char *aux = strtok(s, "/"), *hour = NULL;	
	
	if (!aux) return NULL;
	
	//check if first field is of format hour:min:sec
	//if so then keep it for later parsing
	if (strchr(aux, ':') != NULL) {
		hour = aux;
		aux = strtok(NULL, "/");
	}
	
	TDate *date = (TDate *) calloc(1, sizeof(TDate));
	if (!date) return NULL;
	
	if (checkDate(aux, date) == 1) {
		free(date);
		return NULL;
	}
	
	if (hour != NULL && checkHour(hour, date) == 1) {
		free(date);
		return NULL;
	}
	
	if (checkLimits(date) == 1) {
		free(date);
		return NULL;
	}
	
	return date;
}
	
			
	
	

