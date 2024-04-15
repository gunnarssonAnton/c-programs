#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <unistd.h>

void *student_actions(void *student_id);
void *ta_actions();
int isWaiting(int student_id);

#define NUM_OF_CHAIRS 3;
#define DEFAULT_NUM_OF_STUDENTS 5

sem_t sem_students;
sem_t sem_ta;
pthread_mutex_t mutex_thread;

int waiting_room_chairs[3];
int number_students_waiting=0, next_seating_position = 0, next_teaching_position = 0, ta_sleeping_flag = 0;

int main(){
    int i;
    int student_num = DEFAULT_NUM_OF_STUDENTS;


    int student_ids[student_num];
    pthread_t students[student_num];
    pthread_t ta;
    sem_init(&sem_students,0,0);
    sem_init(&sem_ta,0,1);

    pthread_mutex_init(&mutex_thread, NULL);
    pthread_create(&ta,NULL,ta_actions,NULL);
    for( i = 0; i < student_num; i++ )
    {
        student_ids[i] = i + 1;
        pthread_create( &students[i], NULL, student_actions, (void*) &student_ids[i] );
    }

    pthread_join(ta, NULL);
    for( i =0; i < student_num; i++ )
    {
        pthread_join( students[i],NULL );
    }

    return 0;
}
void *ta_actions(){
    printf("Checking for students. \n");

    while(true){
        if(number_students_waiting > 0){
            ta_sleeping_flag=0;
            sem_wait(&sem_students);
            pthread_mutex_lock(&mutex_thread);

            int help_time = rand()%5;

            printf( "Helping a student for %d seconds. Amount of students waiting = %d.\n", help_time, (number_students_waiting - 1) );
            printf( "\nStudent %d that receiving help.\n",waiting_room_chairs[next_teaching_position]);

            waiting_room_chairs[next_teaching_position]=0;
            number_students_waiting--;
            next_teaching_position = (next_teaching_position +1 ) % NUM_OF_CHAIRS;

            sleep(help_time);
            pthread_mutex_unlock(&mutex_thread);
            sem_post(&sem_ta);
        } else{
            if(ta_sleeping_flag == 0){
                printf("No students are waiting. Sleeping.\n");
                ta_sleeping_flag=1;
            }
        }

    }
}


void *student_actions(void *student_id){
    int id_student = *(int*)student_id;

    while (true){
        if(isWaiting(id_student)==1){continue;}

        int time = rand()%5;
        printf( "\tStudent %d is programming for %d seconds.\n", id_student, time );
        sleep( time);

        pthread_mutex_lock(&mutex_thread);

        if(number_students_waiting < 3){
            waiting_room_chairs[next_seating_position]=id_student;
            number_students_waiting++;

            printf( "\t\tStudent %d takes a seat. Students waiting = %d.\n", id_student, number_students_waiting );
            next_seating_position = ( next_seating_position + 1 ) % 3;

            pthread_mutex_unlock(&mutex_thread);

            sem_post(&sem_students);
            sem_wait(&sem_ta);
        }
        else{
            pthread_mutex_unlock( &mutex_thread );

            printf( "\t\t\tStudent %d will try later.\n",id_student );
        }
    }
}

int isWaiting( int student_id ) {
    int i;
    for ( i = 0; i < 3; i++ ) {
        if ( waiting_room_chairs[i] == student_id ) {return 1;}
    }
    return 0;
}