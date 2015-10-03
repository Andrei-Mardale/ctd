#include <string.h>
#include <stdlib.h>
#include <time.h>

#ifndef _DATE_
#define _DATE_

#define ULDIFF 32 //upper - lower case difference ('A' - 'a')

//date type as it will be parsed
typedef struct {
	unsigned char second;
	unsigned char minute;
	unsigned char hour;
	unsigned char day;
	unsigned char month;
	int year;
} TDate;

//date difference as it will be obtained from julian day value difference
typedef struct {
	int week;
	int day;
	int hour;
	int minute;
	int second;
} TDateDiff;

//will parse input and retrieve date value
TDate * parse (char *s);

//gets current system date
TDate * currentDate (TDate *update);

//converts date to julian calendar value
double toJulian (TDate *date);

//calculates jv1 - jv2 and returns TDateDiff type
TDateDiff * tdiff (double jv1, double jv2);

//decrease difference by one second
void decreaseDiff (TDateDiff *diff);

#endif
