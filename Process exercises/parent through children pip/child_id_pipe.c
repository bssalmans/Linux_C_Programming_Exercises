// child_id_pipe takes an integer argument, then creates N children and
// 	pipes a message from the parent through each child in succession.
// 	The message is printed by the last child.

#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>

int main( int argc, char** argv )
{
    if( argc == 1 )
    {
        printf( "Error: No argument passed\n" );
        return -1;
    }

    pid_t   pid; 
    int     i = 0, j;
    int     N = atoi( argv[ 1 ] );
    int     child_flag = 0;
	int		**proc_pipes;
	int 	length;
	char	read_buf[50];
	
	proc_pipes = ( int ** ) malloc( N * sizeof( int* ) );

	for( j = 0; j < N; j++ )
	{
		proc_pipes[j] = ( int* ) malloc( 2 * sizeof(int) );
		if( pipe( proc_pipes[j] ) < 0 ) return 1;
	}

	write( proc_pipes[0][1], "\nPass it on", 50 );

    while( i < N )
    {
        i++;
		if( ( pid = fork() ) < 0) { printf( "Failed to fork.\n" ); return 1; }
		else if( pid == 0 )
		{
			if( i == N )
			{
				read( proc_pipes[i - 1][0], read_buf, 11 );
				printf( "Child: %d, ID: %d", i, getpid() );
				puts( read_buf );
				return 0;
			}
			else
			{
				length = read( proc_pipes[i - 1][0], read_buf, 11);
				printf( "Child: %d, ID: %d\n", i, getpid() );
				write( proc_pipes[i][1], read_buf, length );
				return 0;
			}
		}
    }
	
	while( pid = wait( NULL ) ){
		if( errno == ECHILD ) break;
    }
	for( j = 0; j < N; j++) free( proc_pipes[j] );
	free( proc_pipes );
	printf( "Parent ID: %d\n", getpid() );
	return 0;
}

