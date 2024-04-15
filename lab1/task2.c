//
// Created by anton on 2/9/23.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/wait.h>

int line=1;
int column=0;
int wordCounter=0;
int *sm;

void strColLine(long s, long index,char *text,char *searchString,int cp);
int main(int argc, char **argv){
    sm = mmap(NULL,sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS,-1,0);

    char *filename =argv[1];
    char *searchString =argv[2];

    FILE *inputFile;


    if( ( inputFile = fopen( filename, "r" ) ) == NULL ) {
        fprintf( stderr, "Error- Unable to open %s\n", filename );
        exit( -1 );
    }

    fseek(inputFile,0,SEEK_END);
    long s = ftell(inputFile);

    rewind(inputFile);



    char strArray[s];
    fread(&strArray,s,1,inputFile);


    for (int i = 0; i < 4; i++) {
        pid_t pid = fork();
        if (pid == 0){

            strColLine(s/4,((s/4)*i),strArray,searchString,getpid());

            exit(0);
        }
    }
    for(int i=0; i < 4; i++){
        pid_t pid = wait(NULL);
    }

    printf("_________________________________________________________\n\n");
    printf("Amount Of The Word \"%s\" in \"%s\" Is: [%d]\n",searchString,filename,*sm);
    printf("\nEnd Of Program\n");

    fclose(inputFile);
    inputFile=NULL;
    return 0;
}


void strColLine(long s, long index,char *text,char *searchString,int cp){

    for (long i = index; i < s+index; ++i) {

        column++;


        if (text[i] == '\n'){
            line++;
            column=0;
        }


        if (strncmp(searchString,&text[i],strlen(searchString))==0){
            wordCounter++;

            printf("CHILD %d FOUND THE WORD \"%s\" AT LINE: %d AND COLUMN: %d\n",cp,searchString,line,column);
            printf("\n");
        }
    }
    *sm += wordCounter;
}