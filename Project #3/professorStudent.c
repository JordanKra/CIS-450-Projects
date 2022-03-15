#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <semaphore.h>
#include <stdbool.h>

#define SHARED 1

sem_t professor_ready, question_in_progress, ready_for_answer, answer_done;

void AnswerStart(){
    printf("The professor wants to be asked a question!\n");
    sem_wait(&ready_for_answer);
    sched_yield();
}

void AnswerDone(){
    printf("The professor has finished answering a question\n");
    sem_post(&answer_done);

    sched_yield();
}

void QuestionStart(int student_number){
    //Wait for the professor to be ready
    printf("Student number %d is ready to ask a question\n", student_number);
    sem_wait(&professor_ready);
    //Wait until the student can ask a question
    sem_wait(&question_in_progress);
    sched_yield();
}

void QuestionDone(int student_number){
    printf("Student number %d has finished asking a question\n", student_number);
    sem_post(&ready_for_answer);
    sem_wait(&answer_done);
    printf("Student number: %d has recieved an answer and has left\n", student_number);
    sem_post(&question_in_progress);
    sched_yield();
}

void *TheProfessor(void *num_students){
    int i = *((int*)(num_students));
    while(i > 0){
        AnswerStart();
        printf("The professor is answering a question\n");
        AnswerDone();
        sem_post(&professor_ready);
        i--;
    }
    return 0;
}

void *OneStudent(void *stuNum){
    int student_number = *((int*)(&stuNum));
    QuestionStart(student_number);
    printf("Student Number %d is asking a question\n", student_number);
    QuestionDone(student_number);
    pthread_exit(0);
    return 0;
}

int main(int argc, char *argv[]){
    int numStudent = 0;
    sem_init(&professor_ready, SHARED, 1);
    sem_init(&question_in_progress, SHARED, 1);
    sem_init(&ready_for_answer, SHARED, 0);
    sem_init(&answer_done, SHARED, 0);
    pthread_attr_t attr;

    if(argc < 2){
        printf("Usage: ./professorStudent <Number of students>\n");
        printf("e.g: ./professorStudent 50\n");
        exit(0);
    }
    
    pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    numStudent = atoi(argv[1]);
    pthread_t threads[numStudent];
    pthread_t professor;
    pthread_create(&professor, &attr,&TheProfessor, (void*)&numStudent);
    for(int i = 0; i < numStudent; i++){
        pthread_create(&threads[i], &attr, OneStudent, (void*)i);
    }
    
    for(int i = 0; i < numStudent; i++){
        pthread_join(threads[i], NULL);
    }
    pthread_join(professor,NULL);
    pthread_attr_destroy(&attr);
    pthread_exit(NULL);

}

