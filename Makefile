all: hl

hl: holy_land.o
	gcc holy_land.o -g -o hl

holy_land.o: holy_land.c holy_land.h
	gcc holy_land.c -g -c
