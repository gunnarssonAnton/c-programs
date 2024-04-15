//
// Created by anton on 2/9/23.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


typedef unsigned long ULONG;

ULONG fact(ULONG num);

int main(int argc, char* args[]){
    int input = atoi(args[1]);

    //Check if input is less than Zero
    if(input < 0){
        printf("Invalid Input, Enter A Positive Number!\n");
        exit(-1);
    }

    ULONG sumArray[input];

    int pid = fork();

    if(pid==0){
        //Child Process
            for (int i = 1; i <= input; ++i) {
                sumArray[i-1] = fact(i);
            }

            printf("\n\nThe sum of the series is\n");
            for (int i = 0; i < input; ++i) {
                printf("%lu ",sumArray[i]);
            }
        printf("\n");
        exit(0);

    } else{
        //Parent Process
        wait(NULL);
    }

    return 0;
}
/**
 * prints the factorial of input and return the sum of all factorial number.
 * @param num
 * @return the sum of all factorial numbers.
 */
ULONG fact(ULONG num){
    ULONG res=1;
    ULONG s=0;
    for (ULONG i = 1; i <= num; ++i) {
        res *= i;
        s+=res;
        printf("%lu ",res);
    }
    printf("\n");

    return s;
}