all : project
project: DataStructure.h function.o main.o 
	cc main.o function.o -o project -lncurses
main.o : DataStructure.h main.c
	cc -c main.c
function.o : DataStructure.h function.c
	cc -c function.c
clean:
	rm *.o