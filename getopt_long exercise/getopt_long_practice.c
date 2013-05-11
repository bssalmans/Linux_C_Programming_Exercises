// This program uses getopt_long to parse command line arguments in bash

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define _GNU_SOURCE
#include <getopt.h>

int main( int argc, char *argv[] )
{
	int aFlag = 0;
	int bFlag = 0;
	int opt;
	struct option longopts[] = { { "c", 1, NULL, 'c' }, { 0, 0, 0, 0 } };
	while( ( opt = getopt_long( argc, argv, "ab::c::", longopts, NULL ) ) != -1 ) 
	{
		switch( opt ) 
		{
			case 'a':
				if( bFlag == 1 )
				{
					printf( "Error: Option -b already invoked, cannot invoke -a.\n" );
					break;
				}
				printf( "Option %c\n", opt );
				aFlag = 1;
				break;
			case 'b':
				if( aFlag == 1 )
				{
					printf( "Error: Option -a already invoked, cannot invoke -b.\n" );
                    break;
                }
				printf( "Option %c\n", optarg );
				bFlag = 1;
				break;
			case 'c':
				printf( "Option %c\n", opt );
				break;
		}
	}
	for( ; optind < argc; optind++ )
	{
		if( *argv[optind] == ' ' )
		{
			printf( "Error: No command argument.\n" );
			exit(1);
		}
		else
			printf( "Argmuent: %s\n", argv[optind] );
	}
	exit(0);
}
