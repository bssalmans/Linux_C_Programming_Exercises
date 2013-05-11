#include<stdlib.h>
#include<fcntl.h>


int main()
{
	char	buf[] = "x";	// Character buffer for write

	// Open "hole_file" to create hole
	int fp = open( "hole_file", O_WRONLY|O_CREAT|O_TRUNC, 400 );
	
	write( fp, buf, 1 );		// Write one char to "hole_fle"
	lseek( fp, 8388608, SEEK_SET ); 	// Move file offest 1KB
	write( fp, buf, 1 );		// Write one chat to "hole_file"

	close( fp );			// Close "hole_file"

	return 0;
}
