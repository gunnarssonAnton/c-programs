#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <unistd.h>

#define COLOR_RESET "\033[0m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE "\033[34m"

/*
Near Redmond, Washington, a rowboat is used by Linux hackers and Microsoft employees (serfs) to cross a river.
The rowboat can hold exactly four people and cannot leave the shore with more or fewer. To guarantee the safety of
the passengers, it is not permissible to put one hacker in the boat with three serfs or one serf with three hackers.
Any other combination is safe.
Your task is to write a program to simulate this scenario. The hackers and serfs shall be programmed as threads.
- When a thread boards the boat, it should invoke a function called board().
- It would be best if you guarantee that all four threads from the boatload invoke the board() before any of the threads
    from the next boatload do.
- After all four threads have invoked the board, exactly one of them should call a function rowboat(), indicating that
    that thread will take the oars. It doesn't matter which thread calls the function as long as one does.
- Don’t worry about the direction of travel. Assume we are only interested in traffic going in one of the directions.
*/

void *hacker(void *arg);

void *serf(void *arg);

void board(int id, char *type);

void rowboat(int id, char *type);

pthread_mutex_t mutex;
pthread_barrier_t barrier;
sem_t hacker_sem;
sem_t serf_sem;

int boat_counter = 0;
int hacker_counter = 0;
int serf_counter = 0;

int main(int argc, char *argv[]) {
    int boat_trips = (argc > 1) ? atoi(argv[1]) : 5;
    int *ids = malloc(sizeof(int) * boat_trips * 4);

    pthread_attr_t attr;
    pthread_t hacker_threads[boat_trips * 2];
    pthread_t serf_threads[boat_trips * 2];

    pthread_attr_init(&attr);
    pthread_mutex_init(&mutex, NULL);
    pthread_barrier_init(&barrier, NULL, 4);
    sem_init(&hacker_sem, 0, 0);
    sem_init(&serf_sem, 0, 0);

//    create the threads for the hackers and serfs
    for (int i = 0; i < boat_trips * 2; i++) {
        ids[i] = i;
        pthread_create(&hacker_threads[i], &attr, hacker, &ids[i]);
        pthread_create(&serf_threads[i], &attr, serf, &ids[i]);
    }

//    join the threads
    for (int i = 0; i < boat_trips * 2; i++) {
        pthread_join(hacker_threads[i], NULL);
        pthread_join(serf_threads[i], NULL);
    }

//    clean up
    free(ids);
    pthread_attr_destroy(&attr);
    pthread_mutex_destroy(&mutex);
    pthread_barrier_destroy(&barrier);
    sem_destroy(&hacker_sem);
    sem_destroy(&serf_sem);

    return 0;
}

/**
 * This function will be used by hacker threads.
 * It will wait for the mutex to be available and then try to enter a boat.
 * If it occupies the last spot in a boat and neither hacker nor serf is alone in the boat, it will be the captain.
 * @param arg id used for printing
 */
void *hacker(void *arg) {
    int id = *(int *) arg;
    char *type = "hacker";
    bool is_captain = false;

    pthread_mutex_lock(&mutex);
    hacker_counter++;
    if (hacker_counter >= 4) {
//        signal four hackers to board
        for (int i = 0; i < 4; ++i) {
            sem_post(&hacker_sem);
        }
        hacker_counter = 0;  // reset hacker count
        is_captain = true;
    } else if (hacker_counter == 2 && serf_counter >= 2) {
//        signal two hackers and serfs to board
        for (int i = 0; i < 2; ++i) {
            sem_post(&hacker_sem);
            sem_post(&serf_sem);
        }
        serf_counter -= 2;  // decrement serf counter
        hacker_counter = 0;  // reset hacker counter
        is_captain = true;
    } else {
//        boat is not full or either serf or hacker is alone
        pthread_mutex_unlock(&mutex);
    }
    sem_wait(&hacker_sem);  // wait for captain

    board(id, type);
    if (is_captain) {
        rowboat(id, type);
    }
}

/**
 * This function will be used by serf threads.
 * It will wait for the mutex to be available and then try to enter a boat.
 * If it occupies the last spot in a boat and neither hacker nor serf is alone in the boat, it will be the captain.
 * @param arg id used for printing
 */
void *serf(void *arg) {
    int id = *(int *) arg;
    char *type = "serf";
    bool is_captain = false;

    pthread_mutex_lock(&mutex);
    serf_counter++;
    if (serf_counter >= 4) {
//        signal four serfs to board
        for (int i = 0; i < 4; ++i) {
            sem_post(&serf_sem);
        }
        serf_counter = 0;  // reset serf count
        is_captain = true;
    } else if (serf_counter == 2 && hacker_counter >= 2) {
//        signal two hackers and serfs to board
        for (int i = 0; i < 2; ++i) {
            sem_post(&serf_sem);
            sem_post(&hacker_sem);
        }
        hacker_counter -= 2;  // decrement hacker counter
        serf_counter = 0;  // reset serf counter
        is_captain = true;
    } else {
//        boat is not full or either serf or hacker is alone
        pthread_mutex_unlock(&mutex);
    }
    sem_wait(&serf_sem);  // wait for captain

    board(id, type);
    if (is_captain) {
        rowboat(id, type);
    }
}

/**
 * A passenger enters the next boat.
 * Prints out a string indicating the boarding of a thread.
 * @param id given thread id to print
 * @param type type of passenger
 */
void board(int id, char *type) {
    printf("%sBOARD: %s %d is boarding boat number %d.%s\n",COLOR_BLUE , type, id, boat_counter + 1, COLOR_RESET);
    pthread_barrier_wait(&barrier);  // wait for the barrier condition to be met
}

/**
 * The captain rows the boat to the other side.
 * Prints out a string indicating that the boat is leaving.
 * @param id given thread id to print
 * @param type type of passenger
 */
void rowboat(int id, char *type) {
    printf("%sROW: %s %d is rowing boat number %d.%s\n", COLOR_YELLOW, type, id, ++boat_counter, COLOR_RESET);
    pthread_mutex_unlock(&mutex);
}