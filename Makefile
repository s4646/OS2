.PHONY = all clean
CC = gcc
FLAGS = -Wall -Werror -g

all: mytee mync shell

mytee: mytee.o
	$(CC) mytee.o $(FLAGS) -o mytee

mytee.o: mytee.c
	$(CC) $(FLAGS) -c mytee.c

mync: mync.o
	$(CC) mync.o $(FLAGS) -o mync

mync.o: mync.c
	$(CC) $(FLAGS) -c mync.c

shell: shell.o dir.o copy.o dir.h copy.h
	$(CC) shell.o dir.o copy.o $(FLAGS) -o shell

shell.o: shell.c
	$(CC) $(FLAGS) -c shell.c

dir.o: dir.c dir.h
	$(CC) $(FLAGS) -c dir.c

copy.o: copy.c copy.h
	$(CC) $(FLAGS) -c copy.c

clean:
	rm -f *.a *.o *.so *.gch mytee mync shell