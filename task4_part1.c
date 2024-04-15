#include <stdio.h>
#include <limits.h>
#include <stdbool.h>

typedef struct Process Process;
struct Process{
int pid;
int burstTime;
int arrivalTime;
};
void findTurnAroundTime(Process proc[], int n, int waitTime[], int tat[]);
void findWaitingTime(Process proc[], int n, int waitTime[]);
void findAvgTime(Process proc[],int n);



int main(){
    Process proc[]={
            {1,5,1},
            {2,3,1},
            {3,6,2},
            {4,5,3}
    };
    int n = sizeof(proc)/sizeof(proc[0]);
    findAvgTime(proc,n);

    return 0;
}

void findTurnAroundTime(Process proc[], int n, int waitTime[], int tat[]){
    for (int i = 0; i < n; i++) {
        tat[i] = proc[i].burstTime + waitTime[i];
    }
}

void findWaitingTime(Process proc[], int n, int waitTime[]){
    int roundTime[n];

    for (int i = 0; i < n; i++) {
        roundTime[i]=proc[i].burstTime;
    }
    int complete = 0, t=0, minm = INT_MAX;
    int shortest = 0, finish_time;
    bool check = false;

    while(complete != n){
        for (int j = 0; j < n; j++) {

            if(proc[j].arrivalTime <= t && roundTime[j] < minm && roundTime[j] > 0){
                minm = roundTime[j];
                shortest = j;
                check = true;
            }

            if(check == false){
                t++;
                continue;
            }

            roundTime[shortest]--;
            minm = roundTime[shortest];

            if(minm == 0){
                minm = INT_MAX;
            }
            if(roundTime[shortest]==0){
                complete++;
                check = false;
                finish_time = t + 1;
                waitTime[shortest] = finish_time - proc[shortest].burstTime - proc[shortest].arrivalTime;

                if(waitTime[shortest]<0){
                    waitTime[shortest]=0;

                }
            }
            t++;
        }
    }


}

void findAvgTime(Process proc[],int n){
    int waitTime[n],tat[n], total_waitTime = 0, total_tat = 0;
    findWaitingTime(proc,n,waitTime);
    findTurnAroundTime(proc,n,waitTime,tat);

    printf("Process\tBurst Time\tWaiting Time\tTurnaround Time\n");
    for (int i = 0; i < n; ++i) {
        total_waitTime = total_waitTime + waitTime[i];
        total_tat = total_tat + tat[i];
        printf("%d\t%d\t\t%d\t\t%d\n", proc[i].pid, proc[i].burstTime, waitTime[i], tat[i]);
    }

    printf("\n#################[AVERAGE]#################\n");
    printf("Waiting Time: %.2f\n", (float)total_waitTime / n);
    printf("Turnaround Time: %.2f\n", (float)total_tat / n);
}