#include <stdio.h>
#include "date.h"
#include <time.h>

int main (int argc, char *argv[]) {
	TDate *date = parse(argv[1]);

	return 0;
}
