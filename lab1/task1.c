//
// Created by anton on 2/1/23.
//
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
char* commandHistory[10];
extern int errno;
void pushCommandToHistoryArray(char* commands);


int main(){
    int arr_size = sizeof(commandHistory)/ sizeof(char*);

    for (int i = 0; i < arr_size; ++i) {
        commandHistory[i]=NULL;
    }

        while(1){

            char str[20];
            const char *strArray[10];
            int inputN;
            printf("\nosh>> ");
            fgets(str,20,stdin);
            size_t sLen = strlen(str);

            if(str[sLen-1] == '\n'){
                str[sLen-1] = '\0';
            }


            if(str[0]=='!'){
                if(str[1]=='!'){
                    strncpy(str,commandHistory[0],strlen(commandHistory[0]));

                }
                else if(isdigit(str[1])) {
                    inputN = str[1];
                    int newInt = inputN -49;
                    strncpy(str,commandHistory[newInt], strlen(commandHistory[newInt]));
                }
                else{
                    printf("Error: Most insert number\n");
                }

            }



            if(strncmp(str,"exit",4)==0){
                printf("<--[Shutting down]-->");
                break;
            }


            if(strncmp(str,"history",7)==0){
                printf("Command History: \n");
                for (int i = arr_size; i >= 0; --i) {
                    if(commandHistory[i]!=NULL){
                        printf("%d: -> %s\n",i+1,commandHistory[i]);
                    }
                }
                continue;
            }
            pushCommandToHistoryArray(strdup(str));

            int pid = fork();

            if (pid==-1){
                printf("There is an error while calling\n");
                return 1;
            }

            //check if we are in child process
            if(pid == 0){


                char *token = strtok(str," ");

                int count=0;
                while (token!=NULL){
                    strArray[count]=token;

                    token = strtok(NULL," ");
                    count++;
                }

                char *args[] = {strArray[0],strArray[1], NULL};

                execvp(args[0], args);


                exit(0);
            }
            //else we are in the parent process
            else{

                if(strstr(str,"&") == NULL){
                    wait(NULL);
                }
                int returnStatus;
                waitpid(pid, &returnStatus,0);
                if(returnStatus==0){
                    printf("The child process terminated normally\n");
                }
                if(returnStatus==1){
                    printf("The child precess terminated with an error\n");
                }
                printf("Parent\n");
                printf("Some post processing goes here\n");
            }

        }
    for (int i = 0; i < arr_size; ++i) {
        free(commandHistory[i]);
        commandHistory[i]=NULL;
    }
    return 0;
}



void pushCommandToHistoryArray(char* commands) {

    commandHistory[9] = NULL;

    for (int i = 8; i >= 0; --i) {
        commandHistory[i + 1] = commandHistory[i];
    }
    commandHistory[0] = commands;

}

