#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>

int main( int argc, char** argv )
{
	int	gchild_id;
	int	child_id;
	int	parent_id;
	int pid;

	if( ( pid = fork() ) == 0 )
	{
		if( ( pid = fork() ) > 0 ) exit( 0 );
		gchild_id = getpid();
		child_id = getppid();
		printf( "Child: %d\nGrandchild: %d\n",
					 child_id, gchild_id );
		system( "ps -l" );
	}
	else if( pid > 0 )
	{
		wait( NULL );
		parent_id = getpid();
		printf( "Parent: %d\n", parent_id );
		system( "ps -l" );
	}
	return 0;
}
