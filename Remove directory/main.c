// remove_directory takes a directory argument and removes all files
// 	contained within it and removes the directory.

#include<unistd.h>
#include<stdio.h>

int main( int argc, char** argv )
{
	if( argc < 2 )
	{
		fprintf( stderr, "Not enough arguments\n" );
		return -1;
	}

	remove_directory( argv[1] );

	return 0;

}
