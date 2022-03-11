#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <semaphore.h>
#include <stdbool.h>


pthread_mutex_t cross_mutex;
pthread_cond_t cross_cv;
//Keep track of the direction of the car currently on the bridge
int current_dir = -1;
bool bridge_occupied = false;


void ArriveBridge(int dir){
    printf("Car has arrived at the bridge with direction: %d\n", dir);
    pthread_mutex_lock(&cross_mutex);
    while(bridge_occupied){
        /*if(current_dir == dir){
            //Check if our direction is the same as the current cars direction
            //If it is we can go as well
            //TODO: ensure that this functionality is actually wanted
            break;
        }*/
        pthread_cond_wait(&cross_cv, &cross_mutex);
    }
    //set current_dir to be current threads dir
    current_dir = dir;
    bridge_occupied = true;
    pthread_mutex_unlock(&cross_mutex);
    sched_yield();
}

void CrossBridge(){
    printf("Car is crossing the bridge in direction: %d\n", current_dir);
    sched_yield();
}

void ExitBridge(int dir){
    pthread_mutex_lock(&cross_mutex);
    //Weve reached the other side of the bridge so we can set bridge_occupied to false
    //and signal for the next thread
    printf("Car has exited the bridge with dir: %d\n", current_dir);
    bridge_occupied = false;
    pthread_cond_signal(&cross_cv);
    pthread_mutex_unlock(&cross_mutex);
    sched_yield();
}

void *OneVehicle(void *dir){
    ArriveBridge(*(int*)dir);
    CrossBridge();
    ExitBridge(*(int*)dir);
    return 0;
}

int main(int argc, char *argv[]){
    int dir = 0;
    int num_cars = 0;
    pthread_mutex_init(&cross_mutex, NULL);
    pthread_cond_init(&cross_cv, NULL);
    if(argc < 2){
        printf("Usage: oneLaneBridge <Number of Cars>\n");
        printf(" e.g.: ./oneLaneBridge 50\n");
        exit(0);
    }
    num_cars = atoi(argv[1]);
    pthread_t threads[num_cars];

    for(int i = 0; i < num_cars; i++){
        //Generate a direction and create a new thread with that direction
        dir = rand() % 2;
        //printf("Direction: %d\n", dir);
        pthread_create(&threads[i],NULL, &OneVehicle, (void*)&dir);
    }
    for(int i = 0; i < num_cars; i++){
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&cross_mutex);
    pthread_cond_destroy(&cross_cv);
    pthread_exit (NULL);


}





