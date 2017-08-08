/**-----------------------------------------*
 * Antonio Rios
 * April 10, 2016
 * CS480
 * Homework 5 race conditions
 *----------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "phone_record.h"

#define USE_LOCK_FILE "PhoneBook.txt"
#define DB "PhoneBook.txt"

void unlock();
void lock();

int main(int argc, char* argv[]){

	PhoneBookRecord  phbr;
	int fd = open(DB,O_RDWR | O_CREAT);
	pid_t pid;
	int index;
	int status;

	int return_from_wait; 

	phbr.name = argv[1];
	phbr.number = getpid();
	phbr.email = "ar383@nau.edu";


	for(int i =0; i < 10; i++){

		
		add_record(&phbr);
		printf("\nrecord added to file\n");
		remove_record(1);	
		printf("\nrecord deleted from file\n");

		pid = fork();

	
		if(pid ==0){
			execl("~/Documents/","hw5", "Antonio");
			exit(127);
		}
		else{
			
		  // the father waits for the children to return
		  // this code is tricky, as the father may be caught by a signal right in wait,
		  // which accounts for a number of false waits. To avoid, we wait one more time
		  // when this happens.
		  while (i--) {
			return_from_wait = wait(&status);

			if(return_from_wait == -1){
				i++;  // this wait went wrong, so wait this one more
				// perror("Error wait");
			
			} else{
				printf("Process %d returned\n", return_from_wait);

			}

		  }
			exit(EXIT_SUCCESS);
		}
			

	} 

	add_record(&phbr);

	close(fd);

	return 0;
}

void unlock(int fd){
  #ifdef USE_LOCK_FILE

	struct flock lock;
	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLKW, &lock);
	
  #endif

}

void lock(int fd){
	struct flock lock;

	fd = open(DB, O_RDWR);
	memset(&lock, 0, sizeof(lock));
	lock.l_type = F_WRLCK;
	fcntl(fd, F_SETLKW, &lock);

}
