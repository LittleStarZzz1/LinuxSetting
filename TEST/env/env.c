#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 

int main(int argc, char* argv[], char* env[]) 
{ 
    int i = 0; 
    //for (; i < argc; ++i) 
    //{ 
    //  printf("argv[%d] = %s\n", i, argv[i]); 
    //}
    
    //for (; env[i] != NULL; ++i) 
    //{ 
    //  printf("env[%d] = %s\n", i, env[i]); 
    //}
    
    extern char** environ;
    for (; environ[i] != NULL; ++i)
    {
        printf("environ[%d] = %s\n", i, environ[i]);
    }


    return 0;

}
