#include <stdio.h>


void waitingTime(int proc[],int n, const int burst_time[],int wait_time[]);
void turnaroundTime(int proc[], int n, const int burst_time[], const int wait_time[],int tat[]);
void avgTime(int proc[], int n, int burst_time[]);



int main(){
    int proc[]={1, 2, 3};
    int n = sizeof(proc)/sizeof(proc[0]);
    int burst_time[] = {10, 5, 8};
    avgTime(proc,n,burst_time);

    return 0;
}

void waitingTime(int proc[],int n, const int burst_time[],int wait_time[]){
    wait_time[0]=0;
    for (int i = 1; i < n; i++) {
        wait_time[i] = burst_time[i]+ wait_time[i-1];
    }
}

void turnaroundTime( int proc[], int n, const int burst_time[], const int wait_time[],int tat[]){
    for (int i = 0; i < n; i++) {
        tat[i] = burst_time[i] + wait_time[i];
    }
}

void avgTime(int proc[], int n, int burst_time[]){
    int wait_time[n], tat[n], total_waitTime = 0, total_tat = 0;
    waitingTime(proc, n , burst_time, wait_time);
    turnaroundTime(proc,n, burst_time, wait_time, tat);

    printf("Process\tBurst Time\tWaiting Time\tTurnaround Time\n");
    for (int i = 0; i < n; ++i){
        printf("%d\t%d\t\t%d\t\t%d\n", proc[i], burst_time[i], wait_time[i], tat[i]);
        total_waitTime = total_waitTime + wait_time[i];
        total_tat = total_tat + tat[i];
    }

    printf("\n#################[AVERAGE]#################\n");
    printf("Waiting Time: %.2f\n", (float)total_waitTime / n);
    printf("Turnaround Time: %.2f\n", (float)total_tat / n);
}