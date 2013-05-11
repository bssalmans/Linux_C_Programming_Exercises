// end_of_file tests for the end of the file while reading with file
// 	descriptors.

#include <unistd.h>
#include <sys/types.h>

bool eof( int fd )
{
	off_t	end_offset;
	off_t	n_offest = 0;
	size_t	nread;
	char 	current[5];

	nread = read( fd, current, 1 );
 
	while( nread > 0 )
	{
		n_offset++;
		nread = read( fd, current, 1 );
	}
			
	if( n_offset == 0 )
	{
		return 0;
	}
	else
	{
		end_offset = lseek( fd, -n_offset, SEEK_END );
		return 1;
	}
}	 

