.PHONY = all clean
CC = gcc
FLAGS = -Wall -Werror -g

all: mytee mync myshell

mytee: mytee.o
	$(CC) mytee.o $(FLAGS) -o mytee

mytee.o: mytee.c
	$(CC) $(FLAGS) -c mytee.c

mync: mync.o
	$(CC) mync.o $(FLAGS) -o mync

mync.o: mync.c
	$(CC) $(FLAGS) -c mync.c

myshell: myshell.o dir.o copy.o execute.o dir.h copy.h execute.h
	$(CC) myshell.o dir.o copy.o execute.o $(FLAGS) -o myshell

myshell.o: myshell.c
	$(CC) $(FLAGS) -c myshell.c

dir.o: dir.c dir.h
	$(CC) $(FLAGS) -c dir.c

copy.o: copy.c copy.h
	$(CC) $(FLAGS) -c copy.c
	
execute.o: execute.c execute.h
	$(CC) $(FLAGS) -c execute.c

clean:
	rm -f *.a *.o *.so *.gch mytee mync myshell