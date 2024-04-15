#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t h_sem;
sem_t o_sem;

int h_count = 0;
int o_count = 0;

void *hydrogen(void *arg);
void *oxygen(void *arg);


int main() {
    printf("####################[Task 2]####################\n\n");

    pthread_t h1, h2, o1;

    sem_init(&h_sem, 0, 1);
    sem_init(&o_sem, 0, 1);

    pthread_create(&h1, NULL, hydrogen, NULL);
    pthread_create(&h2, NULL, hydrogen, NULL);
    pthread_create(&o1, NULL, oxygen, NULL);

    pthread_join(h1, NULL);
    pthread_join(h2, NULL);
    pthread_join(o1, NULL);

    sem_destroy(&h_sem);
    sem_destroy(&o_sem);

    return 0;
}

void makeWater() {
    printf("Water is formed!\n");
}

void hReady() {
    sem_wait(&h_sem);
    h_count++;

    if (h_count >= 2 && o_count >= 1) {
        makeWater();
        sem_post(&h_sem);
        sem_post(&h_sem);
        sem_post(&o_sem);
        h_count -= 2;
        o_count--;
    } else {
        sem_post(&h_sem);
    }
}

void oReady() {
    sem_wait(&o_sem);
    o_count++;

    if (h_count >= 2 && o_count >= 1) {
        makeWater();
        sem_post(&h_sem);
        sem_post(&h_sem);
        sem_post(&o_sem);
        h_count -= 2;
        o_count--;
    } else {
        sem_post(&o_sem);
    }
}

void *hydrogen(void *arg) {
    hReady();
    printf("H atom reacted to form water.\n");
    pthread_exit(NULL);
}

void *oxygen(void *arg) {
    oReady();
    printf("O atom reacted to form water.\n");
    pthread_exit(NULL);
}