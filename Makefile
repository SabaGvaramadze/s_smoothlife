CC = gcc
FLAGS = -g -Wall -Wextra

smoothlife : smoothlife.c
	$(CC) smoothlife.c -o smoothlife $(FLAGS)
