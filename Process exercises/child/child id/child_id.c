// child_id takes an integer argument and creates N many child processes
// 	that each print their pids.

#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main( int argc, char** argv )
{
	if( argc == 1 )
	{
		printf( "Error: No argument passed\n" );
		return -1;
	}

	pid_t 	pid;
	pid_t	done;
	int 	i = 0;	
	int		N = atoi( argv[ 1 ] );
	int		child_flag = 0;
	int		realpid;
	
	while( i < N && child_flag == 0 )
	{
		i++;
		pid = fork();
		if( pid == 0 )
			child_flag = 1;
	}

	if( pid == 0 )
	{
		realpid = getpid();
		printf( "Child: %d, ID: %d\n", i, realpid );
	}
	else
	{
		while( done != 0 )
		{
			done = waitpid( pid, (int *) 0, WNOHANG );
		}
	}
}	 
