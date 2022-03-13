/*
Name: Daniela Chavez
BlazerId: dchavez
Project #: 3
To compile: 'make' or 'gcc -o search search.c'
To run: ./search -s <file size in bytes > -f <string pattern> -e <"unix command">
(command line options can be in any order)
*/

#include <stdio.h> 
#include <stdlib.h>
#include <dirent.h> 
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <getopt.h>
#include <unistd.h>
#include <sys/wait.h>


typedef void MYFUNC(char *filePath,char *name, int nestingCount);
static MYFUNC myfunc;
static int flags = 0;
static int flagF = 0;
static int flagS = 0;
static int flage = 0;
static int sizeArgument = 0;
static char *substring = NULL;
static char *eArgument = NULL;

/*Fuction that traverses the file hierarchy recursively*/
void printDirectories(char *dirname, MYFUNC* func, int nestingCount){
  struct dirent *dirent; 
  DIR *parentDir; 
  char new_dir[1000];

//assigns directory to parentdir and opens it
  parentDir = opendir(dirname);
  if (parentDir == NULL) { 
    printf ("Error opening directory '%s'\n", dirname); 
    exit (-1);
  } 

//reads the given directory 
  while((dirent = readdir(parentDir)) != NULL){
    if (strcmp(dirent->d_name, ".") != 0 && strcmp(dirent->d_name, "..") != 0){
        strcpy(new_dir, dirname);
        strcat(new_dir, "/");
        strcat(new_dir, dirent->d_name);
        func(new_dir,dirent->d_name, nestingCount);
    
     if(dirent ->d_type == DT_DIR){
       nestingCount++;
        printDirectories(new_dir, func, nestingCount);
        nestingCount--;
   }}
  }
  closedir (parentDir);
}

/*function that will use fork, exec, and wait to execute UNIX command*/
void unixCommand(char *path){
  
  //concatenating -e argument and path to line
  char line[50];
  strcpy(line,"");
  strcat(line, eArgument);
  strcat(line," ");
  strcat(line, path);

  //tokenizing line and storing in args array
  char *token;
  char *args[100];
  char **next = args;
  token = strtok(line, " \n");

   while (token != NULL) {
        *next++ = token;
        token = strtok(NULL, " \n");
    }
    *next = (char *)NULL;

   pid_t pid;
    int status;

    pid = fork();
    if(pid == 0) {
      execvp(args[0], args);
      printf("if you see this statement then execv failed ;-(\n");
    perror("execv");
    exit(-1);
    }else if (pid > 0){
      //printf("wait for the child process to terminate\n");
      wait(&status);
      if(WIFEXITED(status)) {
       // printf("Child process exited with the status = %d\n", WEXITSTATUS(status));
      } else{
        printf("child process did not terminate normally!\n");
      }
    }else{
      perror("fork");
      exit(EXIT_FAILURE);
    }

}

/*function that prints directory hierarchy according to
command line options given */
void myfunc(char* filePath,char *name, int nestingCount){
  struct stat stbuf;
  char buffer[100];
  stat(filePath, &stbuf);
  sprintf(buffer, "%d", (int)stbuf.st_size);;
  
  if(flagF == 1 && flagS == 1 && flags == 1 && flage == 1){
    if(strstr(name,substring) && ((int)stbuf.st_size >= sizeArgument)){
      printf("\n%s%s%s(%s)Bytes\n"," ", name, " ", buffer);
      if(S_ISREG(stbuf.st_mode)){
        unixCommand(filePath);
      }
    }
  }else if(flagF != 1 && flagS == 1 && flags == 1 && flage == 1){
     if(((int)stbuf.st_size >= sizeArgument)){
      printf("\n%s%s%s(%s)Bytes\n"," ", name, " ", buffer);
      if(S_ISREG(stbuf.st_mode)){
        unixCommand(filePath);
      }
    }
  }else if(flagF == 1 && flagS != 1 && flags == 1 && flage == 1){
     if((strstr(name,substring) && (int)stbuf.st_size >= sizeArgument)){
      printf("\n%s%s\n"," ", name);
      if(S_ISREG(stbuf.st_mode)){
        unixCommand(filePath);
      }
    }
  }else if(flagF == 1 && flagS == 1 && flags != 1 && flage == 1){
     if(strstr(name,substring)){
      printf("\n%s%s%s(%s)Bytes\n"," ", name, " ", buffer);
      if(S_ISREG(stbuf.st_mode)){
        unixCommand(filePath);
      }
     }
  }else if(flagF == 1 && flagS == 1 && flags == 1 && flage != 1){
      if(strstr(name,substring) && ((int)stbuf.st_size >= sizeArgument)){
      printf("\n%s%s%s(%s)Bytes\n"," ", name, " ", buffer);
    }
  }else if(flagF == 1 && flagS != 1 && flags != 1 && flage != 1){
      if(strstr(name,substring)){
      printf("\n%s%s\n"," ", name);
    }
  }else if(flagF != 1 && flagS == 1 && flags != 1 && flage != 1){
    printf("\n%s%s%s(%s)Bytes\n"," ", name, " ", buffer);
  }else if(flagF != 1 && flagS != 1 && flags == 1 && flage != 1){
    if((int)stbuf.st_size >= sizeArgument){
      printf("\n%s%s\n"," ", name);
    }
  }else if(flagF != 1 && flagS != 1 && flags != 1 && flage == 1){
      printf("\n%s%s\n"," ", name);
      if(S_ISREG(stbuf.st_mode)){
        unixCommand(filePath);
      }
  }else if(flagF == 1 && flagS == 1 && flags != 1 && flage != 1){
      if(strstr(name,substring)){
      printf("\n%s%s%s(%s)Bytes\n"," ", name, " ", buffer);
     }
  }else if(flagF == 1 && flagS != 1 && flags == 1 && flage != 1){
      if((strstr(name,substring) && (int)stbuf.st_size >= sizeArgument)){
      printf("\n%s%s\n"," ", name);
    }
  }else if(flagF == 1 && flagS != 1 && flags != 1 && flage == 1){
      if(strstr(name,substring)){
      printf("\n%s%s\n"," ", name);
      if(S_ISREG(stbuf.st_mode)){
        unixCommand(filePath);
      }
    }
  }else if(flagF != 1 && flagS == 1 && flags == 1 && flage != 1){
      if((int)stbuf.st_size >= sizeArgument){
      printf("\n%s%s%s(%s)Bytes\n"," ", name, " ", buffer);
    }
  }else if(flagF != 1 && flagS == 1 && flags != 1 && flage == 1){
      printf("\n%s%s%s(%s)Bytes\n"," ", name, " ", buffer);
      if(S_ISREG(stbuf.st_mode)){
        unixCommand(filePath);
      }
  }else if(flagF != 1 && flagS != 1 && flags == 1 && flage == 1){
     if((int)stbuf.st_size >= sizeArgument){
      printf("\n%s%s\n"," ", name);
      if(S_ISREG(stbuf.st_mode)){
        unixCommand(filePath);
      }
    }
  }else{
    printf("\n%s%s\n"," ", name);
  }
}


int main (int argc, char **argv) { 

  //getopt to process command line options
  int c;
  while((c = getopt(argc, argv, "s:f:e:S")) != -1){
    switch (c){
      case 'f':
      flagF = 1;
      substring = optarg;
      break;
      case 'S':
      flagS = 1;
      break;
      case 's':
      flags = 1;
      sizeArgument = atoi(optarg);
      break;
      case 'e':
      flage = 1;
      eArgument = optarg;
      break;
      case '?':
      printf("unknown option %c\n", optopt);
      exit(-1);
      break;
      default:
      break;

     }
  }
//if program is executed without any arguments
  if (argc < 2) { 
    printf ("Current Directory:\n"); 
    printDirectories(".", myfunc, 0);
    exit(-1);
  }

/* checks to see if directory to traverse was given in command line arguments
if not it will use current directory */
  int i;
  struct stat buf;
  char *ptr; 
  for (i = 1; i < argc; i++) { 
        printf("%s: ", argv[i]); 
        if (lstat(argv[i], &buf) < 0) { 
            ptr = (".");
            continue; 
        }
        if(S_ISDIR(buf.st_mode)){
          ptr = argv[i];
        }}


  printDirectories(ptr, myfunc, 0); //takes in printer function
  
  return 0; 
} 