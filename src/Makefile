CC = gcc
CFLAGS = -std=gnu99#-Wall -Wextra

all: cw

cw: menu.o processopts.o
	$(CC) $(CFLAGS) -o $@ $^

menu.o: menu.c processopts.h
	$(CC) $(CFLAGS) -c $^

processopts.o: processopts.c processopts.h
	$(CC) $(CFLAGS) -c $^

clear:
	rm *.o
	rm *.gch
