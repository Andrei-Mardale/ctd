#include "date.h"

static int checkDate (char *s, TDate *date);
static int checkHour (char *s, TDate *date);
static int checkLimits (TDate *date);
TDate * parse (char *s);

