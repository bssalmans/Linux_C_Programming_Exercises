#include<sys/stat.h>
#include<pwd.h>
#include<grp.h>
#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<dirent.h>
#include<unistd.h>
#include<errno.h>
int main( int argc, char* argv[] )
//int myls( int argc, char* argv[] )
{
/**************************************************************************************/
/*                             Variable Declaration                                   */
/**************************************************************************************/

//--Program variables--
	DIR*	dp; 			/* Pointer to directory being listed */
	char 	dirbuf[255]; 	/* Buffer for directory name */
	struct 	stat statbuf; 	/* Structure for file/dir statistics */
	struct 	dirent *entry; 	/* Structure for directory listing */
	int 	lflag = 0; 		/* Flag for attribute listing (0 = off, 1 = on ) */
	int 	flagargs; 		/* Variable for optin handling */
	int 	statcheck; 		/* Varibale to check stat call success */
	int		inflag = 0;     /* Flag to keep track of chwds */

//--Entry attribute variables--
	char 	type; 			/* Variable to hold file/dir/link ("-", "d", or "l") */
	uid_t 	UID; 			/* Variable to hold user ID number */
	gid_t 	GID; 			/* Variable to hold group ID number */
	struct 	passwd *usrnm; 	/* Structure to hold password information (for username) */
	struct 	group *grpnm; 	/* Structure to hold group information (for groupname) */
	char 	username[25]; 	/* Buffer to hols username */
	char 	groupname[25]; 	/* Buffer to hold groupname */


/**************************************************************************************/
/*                                Option Handling                                     */
/**************************************************************************************/

//--Handle options ( 'l' - list with attribute )--

	while( ( flagargs = getopt( argc, argv, "l::" ) ) != -1 )
	{	
		switch( flagargs )
		{
			case 'l':
				lflag = 1;
				break;
			case '?':
				return -1;
			default:
				break;
		}
	}


/**************************************************************************************/
/*                                   No Option                                        */
/**************************************************************************************/

//--Handle no argument (non-attribute listing)--
	if(  lflag == 0 )
	{
	
	//--Check for directory agrument--
		if( argc == 1 )
		{
		
		//--No argument, get current directory--
			getcwd( dirbuf, 254 );
		}
		else
		{
		
		//--Argument, put argument into directory buffer--
			strcpy( dirbuf, argv[ optind ] );
		}
	
	//--Check if directory can be opened--		
		if( ( dp = opendir( dirbuf ) ) == NULL )
		{
			fprintf( stderr, "Cannot open directory: %s\n", dirbuf );
			return;
		}
		
	//--List directory entries--
		while( ( entry = readdir( dp ) ) != NULL )
		{	
			if( strcmp( "..", entry->d_name ) == 0 || strcmp( ".", entry->d_name ) == 0 )
			{
				continue;
			}
			fprintf( stdout, "%s  ", entry->d_name );
		}
		
		printf( "\n" );
		return 1;
	}


/************************************************************************************/
/*                                    'l' Option                                    */
/************************************************************************************/

//--List files and attributes (ls -l)--
	else if( lflag == 1 )
	{
	
	//--Check argument type--
		if( argv[ optind ] != NULL )
		{
		
		//--Get command line argument--
			statcheck = lstat( argv[ optind ], &statbuf );
			if( statcheck == -1 )
			{
				perror( "myls: Error" );
				return -1;
			}

		//--Parse command line argument
			switch( statbuf.st_mode & S_IFMT )
			{
				case S_IFDIR: type = 'd';
							strcpy( dirbuf, argv[ optind ] ); 
							break;
				case S_IFREG: type = '-';
                            break;
			}
		}
	
	//--No command line argument--
		else
		{
		
		//--Get current directory--
			getcwd( dirbuf, 254 );
			type = 'd';
		}
		
	//--If directory did not open--
		if( type == 'd' && ( dp = opendir( dirbuf ) ) == NULL )
        {
        	perror( "myls: Error" );
        	return 1;
        }
	
	//--If directory do--
		if( type == 'd' )
		{
			closedir( dp );
			dp = opendir( dirbuf );
			chdir( dirbuf );		
		
		//--Read directory entries and attributes--
			while( ( entry = readdir( dp ) ) != NULL )
			{	
			
			//--Check for .. and .--
				if( strcmp( "..", entry->d_name ) == 0 || strcmp( ".", entry->d_name ) == 0 )
        		{
           			continue;
        		}
					
			//--Place entry into statbuffer--
				statcheck = lstat( entry->d_name, &statbuf );
			
			//--Check if lstat call worked--
				if( statcheck == -1 )
				{
					perror( "myls: Error" );
					return -1;
				}

			//--Handle file type--
				switch( statbuf.st_mode & S_IFMT )
				{
					case S_IFREG: type = '-'; break;
					case S_IFDIR: type = 'd'; break;
					case S_IFLNK: type = 'l'; break;
					default: type = 'u'; 
				}					
			
			//--Handle user name--
				UID = statbuf.st_uid;
				usrnm = getpwuid( UID );
				strncpy( username, usrnm->pw_name, 24 );
				username[24] = '\0';

			//--Handle group name--
				GID = statbuf.st_gid;
				grpnm = getgrgid( GID );
				strncpy( groupname, grpnm->gr_name, 24 );
				groupname[24] = '\0';

			//--Handle permissions--
				char urp = ( statbuf.st_mode & S_IRUSR ) ? 'r' : '-';
				char uwp = ( statbuf.st_mode & S_IWUSR ) ? 'w' : '-';
				char uxp = ( statbuf.st_mode & S_IXUSR ) ? 'x' : '-';
				char grp = ( statbuf.st_mode & S_IRGRP ) ? 'r' : '-';
				char gwp = ( statbuf.st_mode & S_IWGRP ) ? 'w' : '-';
				char gxp = ( statbuf.st_mode & S_IXGRP ) ? 'x' : '-';
				char orp = ( statbuf.st_mode & S_IROTH ) ? 'r' : '-';
				char owp = ( statbuf.st_mode & S_IWOTH ) ? 'w' : '-';
				char oxp = ( statbuf.st_mode & S_IXOTH ) ? 'x' : '-';
						
			//--Handle time--
				char timebuf[255];
				strcpy( timebuf, ctime( &statbuf.st_atime ) );
				timebuf[ strlen( timebuf ) - 9 ] = '\0';
				int offset = 4;
				int i = 0;
				for( i = 0; timebuf[ i + offset ] != '\0'; i++ )
				{
					timebuf[ i ] = timebuf[ i + offset ];
				}
				timebuf[ i ] = '\0';
	
			//--Print to standard output--
				fprintf( stdout, "%c%c%c%c%c%c%c%c%c%c %ld %s %s %5ld %s %s\n", type, urp, 
						uwp, uxp, grp, gwp, gxp, orp, owp, oxp, statbuf.st_nlink,
						username, groupname, (long) statbuf.st_size, timebuf, 
						entry->d_name );
			}
			chdir( ".." );
		}
		else
		{	
		
		//--Place entry into statbuffer--
        	statcheck = lstat( argv[ optind ], &statbuf );

        //--Check if lstat call worked--
            if( statcheck != 0 )
            {
        	    perror( "mysl: Error" );
				return -1;
            }

        //--Handle file type--
            switch( statbuf.st_mode & S_IFMT )
            {
                case S_IFREG: type = '-'; break;
                case S_IFLNK: type = 'l'; break;
                default: type = 'u';
            }

        //--Handle user name--
            UID = statbuf.st_uid;
            usrnm = getpwuid( UID );
            strncpy( username, usrnm->pw_name, 24 );
            username[24] = '\0';

        //--Handle group name--
            GID = statbuf.st_gid;
            grpnm = getgrgid( GID );
            strncpy( groupname, grpnm->gr_name, 24 );
            groupname[24] = '\0';

        //--Handle permissions--
            char urp = ( statbuf.st_mode & S_IRUSR ) ? 'r' : '-';
            char uwp = ( statbuf.st_mode & S_IWUSR ) ? 'w' : '-';
            char uxp = ( statbuf.st_mode & S_IXUSR ) ? 'x' : '-';
            char grp = ( statbuf.st_mode & S_IRGRP ) ? 'r' : '-';
            char gwp = ( statbuf.st_mode & S_IWGRP ) ? 'w' : '-';
            char gxp = ( statbuf.st_mode & S_IXGRP ) ? 'x' : '-';
            char orp = ( statbuf.st_mode & S_IROTH ) ? 'r' : '-';
            char owp = ( statbuf.st_mode & S_IWOTH ) ? 'w' : '-';
            char oxp = ( statbuf.st_mode & S_IXOTH ) ? 'x' : '-';
         
		//--Handle time--
            char timebuf[255];
            strcpy( timebuf, ctime( &statbuf.st_atime ) );
            timebuf[ strlen( timebuf ) - 9 ] = '\0';
            int offset = 4;
            int i = 0;
            for( i = 0; timebuf[ i + offset ] != '\0'; i++ )
            {
                timebuf[ i ] = timebuf[ i + offset ];
            }
            timebuf[ i ] = '\0';
         
		//--Print to standard output--
            fprintf( stdout, "%c%c%c%c%c%c%c%c%c%c %ld %s %s %5ld %s %s\n", type, urp,
                  uwp, uxp, grp, gwp, gxp, orp, owp, oxp, statbuf.st_nlink,
                  username, groupname, (long) statbuf.st_size, timebuf,
                  argv[ optind ] );
					
		}
	}	 
} 

