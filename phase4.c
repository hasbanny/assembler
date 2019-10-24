#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "sicengine.c"
#include "commands.h"

int main(){
SICInit(); //initialize simulator
opHash = calloc(29, sizeof(list)); //allocating space for hashtable
symHash = calloc(501, sizeof(list)); //allocating space for hashtable

char line[100]; //array for input str
char cmd[10]; //array for command
char prm1[10]; //array for param1
char prm2[10]; //array for param2

int n=0; //num of params found
int len;

printf("Welcome \n");

while(strcmp(cmd, "exit")){
    printf("Enter a command: ");

    //read in string from keyboard
    fgets(line, 100-1, stdin);

    //replacing \n with the null terminator
    if (line[strlen(line)-1] == '\n'){
       line[strlen(line)-1] = '\0';
    }

    //getting the length of string
    len = strlen(line);

    //breaking line for command/arguments
    breakUp(line, cmd, prm1, prm2, &n);

    com_cmp(cmd, &n, prm1, prm2);
    }

return 0;
}


