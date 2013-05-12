// mysh1 s a simple shell program with "ls", "cp" and "cat" functionality built
// 	in.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>

#define MAX  250

typedef enum {false, true} bool;

void removeChar(char str[], char garbage) 
{
    char *src; 
    char *dst;

    for (src = dst = str; *src != '\0'; src++) {
        *dst = *src;
        if (*dst != garbage) dst++;
    }

    *dst = '\0';
}

void printCmd(char *print[])
{
  int index = 0;
  while(print[index] != NULL){
       printf("%s ",print[index]);
       index++;
  }
  printf("\n");
}

void parseCmd(char *cmdLine, char *args[], char *in[], char *out[], char *pipe[], char *var[])
{
   bool terminate = false;
   char *arg;
   int i = 0;

   if(args[0] != NULL)
   {
      i = 1;
   }
   
   arg = strtok(cmdLine, " ");
   
   while(arg != NULL && terminate != true)
   {
      if (strchr(arg, '<') != NULL)
      {
         if (strlen(arg) > 1)
         {
            removeChar(arg, '<');
            args[i] = arg;
            in[0] = arg;
         }
         else
         {
            arg = strtok(NULL, " ");
            args[i] = arg;
            in[0] = arg;
         }
      }
      else if (strchr(arg, '>') != NULL)
      {
         if(strlen(arg) > 1)
         {
            removeChar(arg, '>');
            args[i] = arg;
            out[0] = arg;
         }
         else
         {
            arg = strtok(NULL, " ");
            args[i] = arg;
            out[0] = arg;
         }
         
      }
      else if (strchr(arg, '|') != NULL)
      {
         terminate = true;
         int i = 0;
         arg = strtok(NULL, " ");
         while(arg != NULL)
         {
            pipe[i] = arg;
            i++;
            arg = strtok(NULL, " ");
         }
         break;
      }
      else if (strchr(arg, '$') != NULL)
      {
         terminate = true;
         int i = 0;
         removeChar(arg, '$');
         removeChar(arg, '(');
         removeChar(arg, ')');
         while(arg != NULL)
         {
            var[i] = arg;
            i++;
            arg = strtok(NULL, " ");
         }
         break;
      }
      else
      {
         args[i] = arg;
         i++;
      }
      arg = strtok(NULL, " ");
   }
   args[i] = '\0';
}

void getCmd(char str[])
{
    fgets(str, MAX, stdin);
    str[strlen(str)-1] = '\0';
}

void exeCmd(char *cmdArgv[], char *in[], char *out[], char *pl[], char *var[])
{
   pid_t pid1;
   pid_t pid2;
   int fd;
   size_t t;
   bool flag = false;
   char *buffer;
   buffer = (char*)malloc(sizeof(char)*MAX); 

   int status1;
   int status2;

   int pip[2];

   if(pipe(pip))
   {
      printf("Pipe failed!");
      exit(-1);
   }

   pid1 = fork();

   if(pid1 < 0)
   {
      printf("Fork error occured");
      exit(-1);
   }

   if (pid1 == 0)
   {
      if (out[0] != NULL)
      {
         fd = open(out[0], O_WRONLY|O_CREAT|O_TRUNC, 0600);
         if (fd < 0)
         {  
             printf("File failed to oprn");
             exit(-1);
         }
         dup2(fd, 1);
         close(fd);
      }
      if (in[0] != NULL)
      {
         fd = open(in[0], O_RDONLY, 0);
          
         if(fd < 0)
         {
             printf("File failed to open");
             exit(-1);
         }
 
         dup2(fd, 0);
         close(fd);
      }
      if (pl[0] != NULL)
      {
         dup2(pip[1], 1);
         close(pip[0]);
         close(pip[2]);

      }
      if (var[0] != NULL)
      {
         flag = true;
         dup2(pip[1], 1);
         close(pip[1]);
         close(pip[0]);
      }

      if(flag == false)
      {
         execvp(*cmdArgv, cmdArgv);
      }
      else
      {
         execvp(*var, var);
      }
   }
   else
   {
      if(var[0] != NULL)
      {
         while(read(pip[0], buffer, MAX) < 0)
         {
            printf("Pipe failed!");
            exit(-1);
         }

         close(pip[0]);

         char *temp = cmdArgv[0];

         strcat(temp, " ");
         strcat(temp, buffer);
        
         int i;      
  
         int size = strlen(temp);

         for(i = 0; i < size; i++)
         {
            if(temp[i] == '\n')
            {
               temp[i] = ' ';
            }
         }
  
         cmdArgv[0] = NULL;
         in[0] = NULL;
         out[0] = NULL;
         pl[0] = NULL;
         var[0] = NULL;
    
         parseCmd(temp, cmdArgv, in, out, pl, var);
      
         exeCmd(cmdArgv, in, out, pl, var); 
      }

      else if(pl[0] != NULL)
      {
         waitpid(pid1, &status1, 0);

         pid2 = fork();

         if(pid2 < 0)
         {
             printf("Fork failed!");
             exit(-1);

         }

         else if (pid2 == 0)
         {
            dup2(pip[0], 0);
            close(pip[0]);
            close(pip[1]);
            execvp(*pl, pl);
         }
         else
         {
            close(pip[0]);
            close(pip[1]);
         }
      }
      else
         waitpid(pid1, &status1, 0);      
   }
}

int main(int argc, char* argv[])
{
   char *inputString;
   inputString = (char*)malloc(sizeof(char)*MAX);

   char *args[MAX];
   char *inFile[MAX];
   char *outFile[MAX];
   char *pipe[MAX];
   char *var[MAX];

   while(true)
   {
      args[0] = '\0';
      inFile[0] = '\0';
      outFile[0] = '\0';
      pipe[0] = '\0';
      var[0] = '\0';
   
      printf("mysh ");
 
      getCmd(inputString);
 
      if(strcmp(inputString, "exit") == 0)
         exit(0);

      parseCmd(inputString, args, inFile, outFile, pipe, var);
    
      exeCmd(args, inFile, outFile, pipe, var);

   }
    return 0;
} 
