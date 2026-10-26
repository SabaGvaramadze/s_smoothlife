CC = gcc
FLAGS = -g -Wall -Wextra
LIBS = -lm -lc

smoothlife : smoothlife.c
	$(CC) $(FLAGS) smoothlife.c -o smoothlife $(LIBS)
