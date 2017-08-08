/*
 * =====================================================================================
 *
 *       Filename:  homework7.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  5/3/2016 9:50:46 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Antonio Rios, Alex Thomas
 *   Organization: CS 480 Operating Systems 
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/stat.h>
/************************************************************************
 * preprocessor directives
 ************************************************************************/
#define INITIAL_NUMBER_COKES 5
#define MAX_NUMBER_COKES 10

#define MAX_RUNS 10

#define NUMBER_PRODUCERS 2
#define NUMBER_CONSUMERS 10



/************************************************************************
 * for convenience, global variables - to be read by threads
 ************************************************************************/
pthread_mutex_t lock;
pthread_cond_t not_empty, not_full;
int cokes = INITIAL_NUMBER_COKES;


/************************************************************************
 * function prototype declarations
 ************************************************************************/
void* thread_function(void* function);

void refill_coke(void);
void consume_coke(void);


/************************************************************************
 * MAIN
 ************************************************************************/
int main(int argc, const char * argv[]) {
    
    int i;
    pthread_t producers[NUMBER_PRODUCERS];
    pthread_t consumers[NUMBER_CONSUMERS];

    // init mutex and condition variables, see above
    // ... FILL IN
   pthread_mutex_init(&lock, NULL); 
   
   pthread_cond_init(&not_empty, NULL);
   pthread_cond_init(&not_full, NULL);
   

  

    // create consumers
    for (i=0; i<NUMBER_CONSUMERS; i++) {
        if (pthread_create(&consumers[i], NULL,thread_function,(void *) refill_coke
            )) {
            perror("Thread creation failed");
            exit(EXIT_FAILURE);
        }
    }
    
    // create producers
    for (i=0; i<NUMBER_PRODUCERS; i++) {
        if (pthread_create(&producers[i], NULL, thread_function, (void *) consume_coke)) {

            perror("Thread creation failed");
            exit(EXIT_FAILURE);
        }
    }

    // just sleep and then bail out
    sleep(10);
    
    return EXIT_SUCCESS;
}


/************************************************************************
 * generic thread function
 ************************************************************************/
void* thread_function(void* function) {
    int i;
    
    for (i=0; i<MAX_RUNS; i++) {
        // call function, either of refill_coke() or consume_coke()
        ((void (*)(void ))function)();
    }

    pthread_exit(NULL);
}


/************************************************************************
 * refilling and consuming coke functions
 ************************************************************************/
void refill_coke(void) {
	

   pthread_mutex_lock(&lock);

   while(cokes < MAX_NUMBER_COKES){

	   printf("--->>%d Refill 10 \n", pthread_self());
	   cokes = 10;
	
	   pthread_cond_signal(&not_empty);


   }

	while(cokes == MAX_NUMBER_COKES ){
		pthread_cond_wait(&not_full, &lock);
	
	}


   pthread_mutex_unlock(&lock);


}


void consume_coke(void) {

	pthread_mutex_lock(&lock);

	while(cokes == 0){
		pthread_cond_wait(&not_empty, &lock);

	}
	cokes--;
	pthread_cond_signal(&not_full);

	printf("<<---%d Take from %d\n", pthread_self(), cokes);
        pthread_mutex_unlock(&lock);
}





