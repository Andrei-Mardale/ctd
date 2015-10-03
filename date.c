#include "date.h"

/*return values:
	1-12 - actual month
	13 - sometihg wrong */
static unsigned char retrieveMonth (char *s) {
	//make everything lower case
	for (char *aux = s; *aux != '\0'; ++aux) {
		if (!(*aux >= 'a' && *aux <= 'z')) {
			*aux += ULDIFF;
		}
	}
	
	if (!strcmp(s, "january")) return 1;
	if (!strcmp(s, "february")) return 2;
	if (!strcmp(s, "march")) return 3;
	if (!strcmp(s, "april")) return 4;
	if (!strcmp(s, "may")) return 5;
	if (!strcmp(s, "june")) return 6;
	if (!strcmp(s, "july")) return 7;
	if (!strcmp(s, "august")) return 8;
	if (!strcmp(s, "september")) return 9;
	if (!strcmp(s, "october")) return 10;
	if (!strcmp(s, "novemer")) return 11;
	if (!strcmp(s, "december")) return 12;
	
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
	if (*s < '0' || *s > '9') {
		date->month = retrieveMonth(s);
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
	if (!aux) return 1;
	
	date->minute = (unsigned char) atoi(aux);
	
	aux = strtok(NULL, ":");
	if (!aux) return 1;
	
	date->second = (unsigned char) atoi(aux);
	
	return 0;
}

static int absVal (int x) {
	return (x < 0) ? -x : x;
}

/*return values
	0 - everything ok
	1 - wrong limits 
checks if given date is valid */
static int checkLimits (TDate *date) {
	if (date->hour > 23) return 1;
	if (date->minute > 59 || date->second > 59) return 1;

	if (date->month < 1 || date->month > 12) return 1;
	
	if (date->day > 31) {
		return 1;
	} else if (date->day > 30 && (date->month % 2) == 0) {
		return 1;
	} else if (date->month == 2) {
		if (absVal(date->year) % 4 == 1 && date->day > 29) {
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

TDate * currentDate () {
	time_t rawtime; //no of second
	struct tm *actualTime; //structure from which TDate will be built
	
	//get raw time - no of second from 1/01/1970 (most probably)
	time(&rawtime);
	
	//get local time from 
	actualTime = localtime(&rawtime);
	
	TDate *present = (TDate *) malloc(sizeof(TDate));
	if (!present) return NULL;
	
	//convert values
	present->year = 1900 + actualTime->tm_year;
	present->month = 1 + actualTime->tm_mon;
	present->day = actualTime->tm_mday;
	present->hour = actualTime->tm_hour;
	present->minute = actualTime->tm_min;
	present->second = actualTime->tm_sec;
	
	return present;
}

double toJulian (TDate *date) {
	int a = (14 - date->month) / 12;
	int y = date->year + 4800 + a;
	int m = date->month + 12 * a - 3;
	
	double jvalue = date->day + (153 * m + 2) / 5;
	jvalue += 365 * y + y / 4 - y / 100 + y / 400 - 32045;
	jvalue += ((double) date->hour - 2) / 24;
	jvalue += (double) date->minute / 1440;
	jvalue += (double) date->second / 86400;
	
	return jvalue;
}

TDateDiff * tdiff (double jv1, double jv2) {
	TDateDiff * diff = calloc(1, sizeof(TDateDiff));
	if (!diff) return NULL;
	
	//calculate difference
	jv1 -= jv2;
	
	//retrive number of second from fraction part
	diff->second = (int) ((jv1 - (int) jv1) * 86400);
	
	//number of days
	diff->day = (int) jv1;
	
	//calculate number of weeks and keep number of remaining days
	diff->week = diff->day / 7;
	diff->day %= 7;	
	
	//find number of hours
	diff->hour = diff->second / 3600;
	
	//eliminate hours, and keep remaining second, from that find minute
	diff->second %= 3600;
	diff->minute = diff->second / 60;
	
	//eliminate minute and then keep remaining second
	diff->second %= 60;
	
	return diff;
}
			
	
	

