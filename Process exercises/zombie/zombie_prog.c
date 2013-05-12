// zombie_prog creates a zombie process

#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>

int main( int argc, char** argv )
{
	pid_t pid;

	pid = fork();

	if( pid != 0 )
	{
		sleep(5);
	}

	system( "ps" );

	return 0;
}
	
