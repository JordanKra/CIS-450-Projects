#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>


pthread_mutex_t cross_mutex;
pthread_cond_t cross_cv;
//Keep track of the direction of the car currently on the bridge
int current_dir = -1;
bool bridge_occupied = false;
int num_on_bridge = 0;
struct args{
    int dir;
    int num;
};


void ArriveBridge(int dir, int num){
    printf("Car %d direction %d arrives at the bridge\n",num, dir);
    pthread_mutex_lock(&cross_mutex);
    while(bridge_occupied){
        if(current_dir == dir && num_on_bridge < 3){
            //We can cross the bridge as well
            break;
        }
        pthread_cond_wait(&cross_cv, &cross_mutex);
    }
    //set current_dir to be current threads dir
    current_dir = dir;
    bridge_occupied = true;
    num_on_bridge++;
    pthread_mutex_unlock(&cross_mutex);
    sched_yield();
}

void CrossBridge(int num){
    pthread_mutex_lock(&cross_mutex);
    printf("Car %d is crossing the bridge. Current dir %d #cars: %d\n",num, current_dir, num_on_bridge);
    pthread_mutex_unlock(&cross_mutex);
}

void ExitBridge(int dir, int num){
    pthread_mutex_lock(&cross_mutex);
    //Weve reached the other side of the bridge so we can set bridge_occupied to false
    //and signal for the next thread
    printf("Car number %d has exited the bridge with dir: %d\n", num, dir);
    bridge_occupied = false;
    num_on_bridge--;
    pthread_cond_broadcast(&cross_cv);
    pthread_mutex_unlock(&cross_mutex);
    sched_yield();
}

void *OneVehicle(void *thread_args){
    struct args *arguments = (struct args*)thread_args;
    ArriveBridge((*arguments).dir, (*arguments).num);
    CrossBridge((*arguments).num);
    ExitBridge((*arguments).dir, (*arguments).num);
    free(thread_args);
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
        //Allocate memory for this threads arguments
        struct args* arguments = (args*)malloc(sizeof(struct args));
        //Generate a direction and create a new thread with that direction
        (*arguments).dir = rand() % 2;
        (*arguments).num = i;

        pthread_create(&threads[i],NULL, &OneVehicle, (void*)arguments);
    }
    for(int i = 0; i < num_cars; i++){
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&cross_mutex);
    pthread_cond_destroy(&cross_cv);
    pthread_exit (NULL);


}





