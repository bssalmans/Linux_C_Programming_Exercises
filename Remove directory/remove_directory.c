#include<unistd.h>
#include<stdio.h>
#include<dirent.h>
#include<string.h>
#include<sys/stat.h>
#include<stdlib.h>

int remove_directory( char *dir )
{
	DIR*	dp;
	struct	dirent	*entry;
	struct	stat	statbuf;

	if( ( dp = opendir( dir ) ) == NULL )
	{
		fprintf( stderr, "Directory %s cannot be opened\n", dir );
		return -1;
	}

	chdir( dir );
	
	while( ( entry = readdir( dp ) ) != NULL )
	{
		lstat( entry->d_name, &statbuf );
		if( S_ISDIR( statbuf.st_mode ) )
		{
			if( strcmp( ".", entry->d_name ) == 0 || 
				strcmp( "..", entry->d_name ) == 0 )
			{
				continue;
			}
			remove_directory( entry->d_name );
		}
		else if( S_ISREG( statbuf.st_mode ) )
		{
			remove( entry->d_name );
		}
		else if( S_ISLNK( statbuf.st_mode ) )
		{
			unlink( entry->d_name );
		}
		else
		{
			fprintf( stderr, "There is an error reading directory %s\n", dir );
		}
	}
	chdir( ".." );
	rmdir( dir );
	return 0;
}
