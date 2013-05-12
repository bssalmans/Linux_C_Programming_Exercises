#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<dirent.h>
#include<string.h>
#include<sys/stat.h>
#include<stdlib.h>

int main(int arg, char **arr){

   int size = sizeof(arr);
   FILE* sources[size];
   char single;
   int i;

   for(i=1;i<=size;i++){
        sources[i-1]=fopen(arr[i], "r");
   }

   if(arg<2){ // display file 
	printf("Not Enough Arguments! Exiting.");
	exit(0);
   }

   else if(arg>=2){ // concat files and display
	for(i=0;i<size;i++){
	   while(sources[i]!=NULL){
	     single = getc(sources[i]);
             if(single==EOF){
                break;
             }
             else{
                printf("%c",single);
             }
 	   }
	}
   }  


}
