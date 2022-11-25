.PHONY = all clean
CC = gcc
FLAGS = -Wall -Werror -g

all: mytee

mytee: mytee.o
	$(CC) mytee.o $(FLAGS) -o mytee

mytee.o: mytee.c
	$(CC) $(FLAGS) -c mytee.c

clean:
	rm -f *.a *.o *.so *.gch mytee