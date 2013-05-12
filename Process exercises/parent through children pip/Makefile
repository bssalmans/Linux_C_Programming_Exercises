exec: child_id_pipe.o
	gcc -o child_id_pipe child_id_pipe.o

child_id.o: child_id_pipe.c
	gcc -c child_id_pipe.c

clean:
	rm -f child_id_pipe.o
	rm -f child_id_pipe

