// foo_system implements the system() clib functionality using an execl()
// 	and wait()

#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>

int foo_system( const char* command )
{
	pid_t	pid;
	pid = fork();
	if( pid == 0 )
	{
		execl( "/bin/sh", "sh", "-c", command, (char*) 0  );
	}
	else
	{
		wait( NULL );
	}

	return 0;
}

int main( int argc, char** argv )
{
	char* command = { NULL };
	int	n =	0;
	
	foo_system( "ls -l" );
	
	return 0;
} 

