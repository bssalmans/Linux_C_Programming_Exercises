// This program assumes a text file "foo.txt" and appends an 'x' to the end

#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>

int main( int argc, char** argv )
{
	FILE* fp = fopen( "foo.txt", "a+" );
	char x;
	fscanf( fp, "%c", &x );
	printf( "%c\n\n", x );
		
	printf( "Writing 'x' to file.\n\n" );

	fputc( 'x', fp );

	int fclose( FILE *fp );
}
