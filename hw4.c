/*
 * =====================================================================================
 *
 *       Filename:  raceCondition.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  4/6/2016 9:36:47 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Antonio Rios, Alex Thomas
 *   Organization:  CS480 Operating Systems
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>


#define DB ("phone_book.txt")

int truncate(const char *path, off_t length);
int ftruncate(int fd, off_t length); 

typedef struct phone_book_record PhoneBookRecord;


struct phone_book_record {

	char name[80];
	char number[10];
};


/*helper function */
int init_db();
void destruct_db();

void add_record(PhoneBookRecord* a_record);
int get_record(char* number, PhoneBookRecord* returned_record);
int remove_record(char* number);
void printDB(PhoneBookRecord phbr[]);
void set_to_record(int fd, int recordNumber);

int main(int argc, char* argv[]){

	PhoneBookRecord phone_records[5], example_record;
	strcpy(phone_records[0].name, "Peter");

	for(int i=0; i<10; i++) *(phone_records[0].number+i) = "1111111111"[i];

	add_record(&phone_records[0]);
	//get_record("3333333333", &example_record);

	printDB(phone_records);

	destruct_db();

	return 0;
}


int init_db(){
	int fd;

	if(access(DB, F_OK) !=0){
		fd = open(DB, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
	}

	return fd;
}

void destruct_db(){
	int fd = init_db();
	close(fd);	
}
void set_to_record(int fd, int recordNumber){
	lseek(fd, (recordNumber-1) * sizeof(PhoneBookRecord) + sizeof(int), SEEK_SET);

}


int get_record(char* number, PhoneBookRecord* returned_record){

	int dbfile = init_db();

}

void add_record(PhoneBookRecord* a_record){

	int dbfile = init_db();
	int record_count =0;
	PhoneBookRecord temp_record;

	char number[11];

	lseek(dbfile, 0, SEEK_SET);
	read(dbfile, &record_count, sizeof(int));

	if(record_count !=0){
		read(dbfile, &temp_record, sizeof(PhoneBookRecord));

		set_to_record(dbfile, record_count+1);
		write(dbfile, &temp_record, sizeof(PhoneBookRecord));
	}
	
	set_to_record(dbfile, 1);
	write(dbfile, a_record, sizeof(PhoneBookRecord));

	record_count++;
	lseek(dbfile, 0, SEEK_SET);
	write(dbfile, &record_count, sizeof(int));

	for(int i=0; i<10; i++) number[i] = a_record->number[i];
	number[10] = '\0';

	printf("[add]-added %s, phone number: %s, number of entries: %d\n", a_record->name, number, record_count);

}

int remove_record(char* number){
	int dbfile = init_db();
	int record_count =1;
	int total = 0;
	PhoneBookRecord phone_record, last_record;

	lseek(dbfile, 0, SEEK_SET);
	read(dbfile, &total, sizeof(int));

	if(!total){
		printf("[remove]-record with number %s is not in the database\n", phone_record.number);
			return-1;
	}
		set_to_record(dbfile, total);

		set_to_record(dbfile, 1);

		while(record_count <= total){
			read(dbfile, &phone_record, sizeof(PhoneBookRecord));

			if(!strncmp(phone_record.name, number, 10)){
				set_to_record(dbfile, record_count);
				write(dbfile, &last_record, sizeof(PhoneBookRecord));
				lseek(dbfile, 0, SEEK_SET);
			
				total--;
				write(dbfile, &total, sizeof(int));	
			}
		
		
		read(dbfile, &last_record, sizeof(PhoneBookRecord));

		truncate(DB, (sizeof (int) + (total * sizeof(PhoneBookRecord))));
		printf("[remove]-person %s remove, %d record(s) remaining\n", phone_record.name);

		}
	}	
void printDB(PhoneBookRecord phbr[5]){
	int i =0;

	printf("\n========DISPLAY DATABASE=============\n");

	while(i < 5){
		if( phbr[i].name != " ")
			printf("Name:%s\nPhone:%s\n", phbr[i].name, phbr[i].number);

		i++;
	}	
}
