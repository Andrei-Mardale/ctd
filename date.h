#include <string.h>

#ifndef _DATE_
#define _DATE_

#define ULDIFF 32 //upper - lower case difference ('A' - 'a')

typedef struct {
	unsigned char seconds;
	unsigned char minutes;
	unsigned char hour;
	unsigned char day;
	unsigned char month;
	int year;
} TDate;

TDate * parse (char *s);

#endif
