exe: remove_directory.o main.o
	gcc -o remove_directory remove_directory.o main.o

remove_directory.o: remove_directory.c
	gcc -c remove_directory.c

main.o: main.c
	gcc -c main.c

clean:
	rm -f remove_directory.o
	rm -f remove_directory
	rm -f main.o
