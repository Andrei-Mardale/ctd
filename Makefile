CC = gcc -c
LD = gcc
CFLAGS = -std=c99 -Wall -Wextra -Wpedantic -g

install:
	make ctd
	sudo mv ctd /usr/bin/ctd
	make clean
	
uninstall:
	sudo rm -f /usr/bin/ctd
	rm -f ~/.ctd/date
	rmdir -f ~/.ctd

build: ctd

ctd: ctd.o date.o fstate.o
	$(LD) $(CFLAGS) $^ -o $@
	
ctd.o: ctd.c date.c fstate.c date.h fstate.h
	$(CC) $(CFLAGS) $< -o $@
	
date.o: date.c date.h
	$(CC) $(CFLAGS) $< -o $@
	
fstate.o: fstate.c date.c date.h fstate.h
	$(CC) $(CFLAGS) $< -o $@
	
clean:
	rm -f *.o
	rm -f *~
	rm -f ctd
