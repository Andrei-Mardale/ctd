CC = gcc -c
LD = gcc
CFLAGS = -std=c99 -Wall -Wextra -Wpedantic -g

ctd: ctd.o date.o
	$(LD) $(CFLAGS) $^ -o $@
	
ctd.o: ctd.c date.c date.h
	$(CC) $(CFLAGS) $< -o $@
	
date.o: date.c date.h
	$(CC) $(CFLAGS) $< -o $@
	
clean:
	rm -f *.o
	rm -f *~
	rm -f ctd
