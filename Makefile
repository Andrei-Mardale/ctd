CC = gcc -c
LD = gcc
CFLAGS = -std=c99 -Wall -Wextra -Wpedantic

ctd: ctd.o date.o
	$(LD) $(CFLAGS) $^ -o $@
	
ctd.o: ctd.c date.h
	$(LD) $(CFLAGS) $< -o $@
	
date.o: date.o date.h
	$(LD) $(CFLAGS) $< -o $@
	
clean:
	rm -f *.o
	rm -f *~
	rm -f ctd
