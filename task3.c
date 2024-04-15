#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define BOAT_CAPACITY 4

int hackers = 0;
int serfs = 0;
pthread_mutex_t mutex;
pthread_barrier_t barrier;

sem_t hacker_sem;
sem_t serf_sem;


void board(char* name)
{
    printf("%s is boarding\n",name);
    pthread_barrier_wait(&barrier);
    sleep(1);
}

void rowboat(int noHackers,int noSerfs){
    printf("Rowboat takes of with %d hackers and %d serfs\n",noHackers,noSerfs);
    pthread_mutex_unlock(&mutex);
}

void* hacker_thread(void* arg)
{
    char*type = "hacker";
    int isCaptainNow = 0;

    pthread_mutex_lock(&mutex);
    hackers++;

    if (hackers >= BOAT_CAPACITY){
        for (int i = 0; i < BOAT_CAPACITY; ++i) {
            sem_post(&hacker_sem);
        }
        hackers = 0;
        isCaptainNow = 1;

    } else if (hackers == 2 && serfs >= 2) {

        for (int i = 0; i < 2; ++i) {
            sem_post(&hacker_sem);
            sem_post(&serf_sem);
        }
        serfs = serfs - 2;
        hackers = 0;
        isCaptainNow = 1;

    } else {
        pthread_mutex_unlock(&mutex);
    }
    sem_wait(&hacker_sem);
    board(type);

    if(isCaptainNow==1){
        rowboat(hackers,serfs);
    }
}

void* serf_thread(void* arg)
{

    char *type ="Serf";
    int isCaptainNow = 0;
    pthread_mutex_lock(&mutex);
    serfs++;

    if (serfs >= BOAT_CAPACITY) {

        for (int i = 0; i < BOAT_CAPACITY; ++i) {
           sem_post(&serf_sem);
        }
        serfs=0;
        isCaptainNow = 1;
    } else if (serfs == 2 && hackers >= 2) {
        for (int i = 0; i < 2; ++i) {
            sem_post(&serf_sem);
            sem_post(&hacker_sem);
        }
        hackers = hackers - 2;
        serfs=0;
        isCaptainNow = 1;
    } else {
        pthread_mutex_unlock(&mutex);
    }

    sem_wait(&serf_sem);

    board(type);


    if (isCaptainNow == 1) {
        rowboat(hackers,serfs);
    }

}

int main()
{
    printf("####################[Task 3]####################\n\n");
    pthread_attr_t attr;
    pthread_t hacker_threads[4];
    pthread_t serf_threads[4];

    pthread_mutex_init(&mutex, NULL);
    pthread_barrier_init(&barrier,NULL,BOAT_CAPACITY);
    sem_init(&hacker_sem, 0, 0);
    sem_init(&serf_sem, 0, 0);

    for (int i = 0; i < 4; i++) {
        pthread_create(&hacker_threads[i], &attr, hacker_thread, NULL);
        printf("Hacker:%d created\n",i);

        pthread_create(&serf_threads[i], &attr, serf_thread, NULL);
        printf("Serf:%d created\n",i);

    }

    for (int i = 0; i < 4; ++i) {
        pthread_join(serf_threads[i], NULL);
        printf("Waiting for serf:%d\n",i);
        pthread_join(hacker_threads[i], NULL);
        printf("Waiting for hacker:%d\n",i);

    }

    pthread_attr_destroy(&attr);
    pthread_mutex_destroy(&mutex);
    pthread_barrier_destroy(&barrier);
    sem_destroy(&hacker_sem);
    sem_destroy(&serf_sem);


    return 0;
}