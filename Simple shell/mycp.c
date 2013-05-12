#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>
#include<stdio.h>
#include<dirent.h>
#include<string.h>
#include<sys/stat.h>

int recursive_copy( char* from, char* to )
{
//--Recursive call variables--
	DIR*    dp1;            /* Pointer for copy from directory */
    DIR*    dp2;            /* Pointer for copy to directory */
    struct  dirent* entry;  /* Structure for directory listing */
    struct  stat statbuf;   /* Structure for file status information */
    int     infile;         /* File descriptor for file to copy */
    int     outfile;        /* File descriptor for file to wrtie */
    char    readbuf;        /* Buffer for read operation */
	int		numread;		/* To track number of chars read from infile */
	int		changed;		/* Directory change flag */
	char*	directory;

//--Check for valid 'from' directory--
    if( ( dp1 = opendir( from ) ) == NULL ) 
    {
    	perror( "mycp: Error" );
        return -1;
    }
    if( ( dp2 = opendir( to ) ) == NULL )
    {
	//--Make directory--
		mkdir( to, S_IRWXU );
		//--Check for valid 'to' directory--
		if( ( dp2 = opendir( to ) ) == NULL )
		{
        	perror( "mycp: Error" );
        	return -1;
		}
    }
	 
//--Copy files--
    chdir( from );

    while( ( entry = readdir( dp1 ) ) != NULL )
    {
    //--Get status of entry
		stat( entry->d_name, &statbuf );

	//--Check for directory--
	 	if( S_ISDIR( statbuf.st_mode ) )
        {
		//--Check for "." and ".."
			if( strcmp( ".", entry->d_name ) == 0 ||
                strcmp( "..", entry->d_name ) == 0 )
            {
                continue;
            }

            else
            {
                recursive_copy( entry->d_name, to );
            }
        }

	//--Regular file copy-- 
		else
		{
		//--Open copy from file descriptor--
			infile = open( entry->d_name, O_RDONLY );
			
		//--Check descriptor open success--
			if( infile == -1 )
            {
				perror( "mycp: Error" );
                return -1;
            }
		
		//--Change to copy to directory--
			chdir( ".." );
			changed = chdir( to );

		//--Open copy to descriptor--
			outfile = open( entry->d_name, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR );
			
		//--Check outfile open success--
			if( outfile == -1 )
            {
        	    perror( "myp: Error" );
                return -1;
            }
			
		//--Copy from--
			while( numread = read( infile, &readbuf, 1 ) == 1 )
            {
                write( outfile, &readbuf, 1);
            }
	
			if( numread == -1 )
			{
				perror( "mycp: Error" );
			}
		
		//--Return to from directory--
			close( outfile );
			close( infile );
			chdir( ".." );
			chdir( from );
		}
	}	
}


int main( int argc, char* argv[] )
//  int mycp( int argc, char** argv )
{
/******************************************************************************
 *                            Variable Declaration                            *
 ******************************************************************************/                            
//--Program variables--
	int		infile;			/* File descriptor for file to copy */
	int		outfile;		/* File descriptor for file to wrtie */
	char	readbuf; 		/* Buffer for read operation */
	int 	flagargs;		/* Varialbe for option handling */
	int		Rflag = 0;		/* Flag for recursive directory copy */
	int 	recurse_success;/* Status of recurse call success */

/******************************************************************************
 *                               Option Handling                              *
 ******************************************************************************/                               
//--Handle options ( 'R' - Recursive directory copy )--
	while( ( flagargs = getopt( argc, argv, "R:" ) ) != -1 )
	{
		switch( flagargs )
		{
			case 'R': Rflag = 1; break;
			case '?': return -1;
			default: break;
		}
	}

/******************************************************************************
 *                                No Option                                   *
 ******************************************************************************/

	if( Rflag == 0 )
	{
	//--Check number or arguments--
		if( argc < 3 )
		{
			fprintf( stderr, "mycp: Error: Insufficient number of arguments\n" );
			return -1;
		}                              

	//--Open file to copy--
		infile = open( argv[1], O_RDONLY );

	//--Check file open success--
		if( infile == -1 )
		{
			perror( "mycp: Error" );
			return -1;
		}	

	//--Open file to copy to--
		outfile = open( argv[2], O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR );

	//--Check file open success--
		if( outfile == -1 )
		{
			perror( "mycp: Error" );
			return -1;
		}
	
	//--Copy file--
		while( read( infile, &readbuf, 1 ) == 1 )
		{
			write( outfile, &readbuf, 1 );
		}
	
		return 0;
	}

/******************************************************************************
 *                              'R' Option                                    *
 ******************************************************************************/
	else if( Rflag == 1 )
	{
	//--Check number of arguments--
		if( argc < 4 )
		{
			fprintf( stderr, "mysp: Error: Insufficient number of arguments\n" );
			return -1;
		}
		int ind = optind - 1;
		recurse_success = recursive_copy( argv[ ind ], argv[ ind + 1 ] );
		if( recurse_success == 0 )
		{
			return 0;
		}
		else
		{
			return -1;
		}
	}
}
