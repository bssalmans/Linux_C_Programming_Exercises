// directory_scan takes a directory argument and lists the files contained in
// 	and any sub-directories using libc functions and structures

#include<sys/types.h>
#include<sys/stat.h>
#include<dirent.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main()
{
	int		ordflag = 0;
	DIR*	dp;
	struct 	stat statbuf;
	char 	dirbuf[255]; 
	int		times[20];
	struct 	dirent* entry;
	int		n = 0;
	int		nument = 0;
	int 	temp;
	int		countup;
	getcwd( dirbuf, 254 );
	dp = opendir( dirbuf );
	while( ( entry = readdir( dp ) ) != NULL )
	{
		lstat( entry->d_name, &statbuf );
		times[ nument ] = statbuf.st_atime;
		nument++;
	}
	closedir( dp ); 	
	int 	i;
	int 	j;
	for( i = nument; i > 0; i-- )
	{
		for( j = 1; j <= i; j++ )
		{
			if( times[ j-1 ] > times[ j ] )
			{
				temp = times[ j-1 ];
				times[ j-1 ] = times[ j ];
				times[ j ] = temp;
			}
		}
	}

	while( countup != nument )
	{	
		dp = opendir( dirbuf );
		while( ( entry = readdir( dp ) ) != NULL )
		{
			lstat( entry->d_name,  &statbuf );
			if( statbuf.st_atime == times[ countup ] )
			{
				printf( "%s\n", entry->d_name );
				countup++;
			}
			else
			{
				continue;
			}
		}
	}

	return( 0 );
}

