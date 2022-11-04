CC = gcc
CFLAGS = -g -Wall -lm -std=c99
build:
	$(CC) -o quadtree main.c queue.c $(CFLAGS)
clean:
	rm -f quadtree