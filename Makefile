all: hl

hl: hl_loader.o hl_frontend.o
	gcc hl_loader.o hl_frontend.o -g -o hl

hl_loader.o: hl_loader.c holy_land.h
	gcc hl_loader.c -g -c

hl_frontend.o: hl_frontend.c holy_land.h
	gcc hl_frontend.c -g -c

clean:
	rm -rf *.o hello

check:
	-valgrind -v --tool=memcheck --leak-check=yes --track-origins=yes ./hl
