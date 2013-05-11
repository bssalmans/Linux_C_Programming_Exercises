// avg_weight assumes a text file in the following format
// 	<string> <char>:<int>:<float>:<float>:<char> 
// 	where
// 	<last name> <first initial>:<weight>:<?>:<height>:<sex>
// 	and calculates and prints the average weight and average height

#include<stdio.h>
#include<stdlib.h>

int main()
{
    double   weight;
    double   height;
    double   ttl_wgt = 0;
    double   ttl_hgt = 0;
    double   last_height;
    double   last_weight;
    int      count = 0;
    FILE     *fp;
 
    fp = fopen( "foo.txt", "r" );

    while( 0 == 0 )
    {	
		if( feof( fp ) )
		{
			break;
		}
        fscanf( fp, "%*[^:]:%*f:%lf:%lf:%*c", &weight, &height );
        if( last_weight != weight && last_height != height )
        {
            ttl_wgt += weight;
            ttl_hgt += height;
            if( weight != 0 && height != 0 )
            {
                count += 1;
            }
        }
 
        last_weight = weight;
        last_height = height;
    }

    ttl_wgt /= count;
    ttl_hgt /= count;
    printf( "The average weight: %10.2f lb\n", ttl_wgt );
    printf( "The average height: %10.2f ft\n", ttl_hgt );
    fclose( fp );
    exit( 0 );
}

