// file_attributes uses libc functionds and structures to
// 	list the attributes of a file or the files containec in a 
// 	directory.

#include<sys/stat.h>
#include<unistd.h>
#include<pwd.h>
#include<grp.h>
#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
int main( int argc, char** argv )
{
	//Check for argument file
	if( argc < 2 )
	{
		printf( "Error: No file or directory specified\n" );
		return 1;
	}
	
	struct	stat statbuf;			//Declare stat structure
	mode_t	modes;					//Declare mode_t to store modes

	lstat( argv[1], &statbuf );
	modes = statbuf.st_mode;

	printf("File information:\n" ); 

	switch( modes & S_IFMT )		//Mask the modes for file type fields, case for file type
	{
		case	S_IFREG:	printf(	"     regular file\n" );
							break;
		case	S_IFDIR:	printf( "     directory\n" );
							break;
		case	S_IFLNK:	printf( "     symbolic link\n" );
							break;
		default:			printf( "     unknown file type\n" );
	}

	printf( "     Inode #: %ld\n", (long) statbuf.st_ino );		//Inode number

	printf( "     Number of links to file: %ld\n", (long) statbuf.st_nlink ); //Link count

	printf( "     File size: %ld bytes\n", (long) statbuf.st_size ); 		  //File size

	printf( "     Number of blocks used: %ld\n", (long) statbuf.st_blocks );  //Blocks used

	printf( "     Last access made: %s", ctime( &statbuf.st_atime ) );	  //Last access

	printf( "     Last change to file: %s", ctime( &statbuf.st_mtime ) );	  //Last modification

	printf( "     Last status change: %s", ctime( &statbuf.st_ctime ) );    //Last change

	printf( "     ID number of device housing file: %ld\n", (long) statbuf.st_dev ); //Device ID

	//Get user name
	uid_t UID = statbuf.st_uid;
	char username[25];
	struct passwd *usernm = getpwuid( UID );
	strncpy( username, usernm->pw_name, 24 );
	username[24] = '\0';

	printf( "     User name: %s\n", username );     //User name

	//Get group name
	gid_t GID =	statbuf.st_gid;
	char groupname[25];
	struct group *group = getgrgid( GID );
	strncpy( groupname, group->gr_name, 24 );
	groupname[24] = '\0';

	printf( "     Group name: %s\n", groupname );   //Group name
	
	int permissions = access( argv[1], R_OK|W_OK|X_OK );

	printf( "     Permissions: %d\n", permissions );	

	exit(EXIT_SUCCESS);
}
