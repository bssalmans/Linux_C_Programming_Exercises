// seek_overwrite is an experiment to see if a file can be read or written from anywhere
// 	if opened with the O_APPEND open time flag.

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

int main()
{
	size_t	written;	//Number of written chars
	off_t	cursor;		//Cursor position
	char	buf[4] = { 'a', 'b', 'c' };	//Char buffer for read/write
	int 	nread;		//Number of chars read
	int		fp = open( "words.txt", O_RDWR|O_APPEND );	//File descriptor

	//Place cursor 5 bits before EOF
	cursor = lseek( fp, -5, SEEK_END );

	//Check succes of lseek
	if( cursor < 0 )
	{
		printf( "Error on seek\n" );
	}
	//If successful
	else
	{	
		printf( "Cursor at %d\n", cursor );	//Current cursor position
		nread = read( fp, buf, 1 );			//Attempt to read
		
		//Check success of read
		if( nread == 1 )
		{
			printf( "You can read!\n" );
		}
		else
		{
			printf( "You illerate!\n" );
		}

		written = write( fp, buf, 3 );		//Write to file
		
		//Check success of write
		if( written == 3 )
		{
			printf( "You can do it!\n" );
		}
		else
		{
			printf( "You can't do it!\n" );
		}
	}
	
	close( fp );

	return 0;
}
