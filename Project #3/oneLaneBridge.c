#include <cstddef>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <semaphore.h>


pthread_mutex_t cross_mutex;
pthread_cond_t cross_cv;


void ArriveBridge(int *dir){
}

void CrossBridge(){
}

void ExitBridge(){
}

void *OneVehicle(void *dir){
    //ArriveBridge(dir);
    //CrossBridge();
    //ExitBridge(dir);
    printf("Direction: %d\n", *(int*)dir);
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
        pthread_create(&threads[i],NULL, &OneVehicle, (void*)&dir);
    }
    for(int i = 0; i < num_cars; i++){
        pthread_join(threads[i], NULL);
    }


}





