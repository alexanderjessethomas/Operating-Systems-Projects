/** ****************************************************
 * Authors: Antonio Rios and Alex Thomas
 * Homework 6: Race Conditions 2
 * CS480 Operating Systems
 * 4/22/2016
 * Dr. Otte
 * ************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <fcntl.h>
#include <stdbool.h>


#define DataBase ("Phone Book.txt")
#define Storage ("Storage Book.txt")

// define the structure that will be use
// to hold the record information of the user
// name and phone number.
typedef struct phone_record {
    int phone_number; 
    char name  [20];
} PhoneRecord;

// function prototypes
int truncate(const char *path, off_t length);
int init_Database();
int init_Storage();
void recordSetter(int fd, int recordNumber);
void printDatabase();
void add_record(PhoneRecord *phone_record);
long getPhoneNumber(char* name);
void *addRecordThenRemoveRecord(void *message);
void remove_record(char *name);

// Mutex Lock that is going to be needed for the program
pthread_mutex_t lock;

//Main function of the program 
int main(int argc, char** argv)
{
	
	char *messageOne;
	char *messageTwo;
	char *messageThree;
	char *messageFour;
	char *messageFive;
	char *messageSix;
	char *messageSeven;
	char *messageEight;
	char *messageNine;
	char *messageTen;

    //This begins all of the threads that are needed for this program. 
	pthread_t threadOne;
	pthread_t threadTwo;
	pthread_t threadThree;
	pthread_t threadFour;
	pthread_t threadFive;
	pthread_t threadSix;
	pthread_t threadSeven;
	pthread_t threadEight;
	pthread_t threadNine;
	pthread_t threadTen;
    
    //This is going to be for return threads that were created above
    int returnOne;
	int returnTwo;
	int returnThree;
	int returnfour;
	int returnFive;
	int returnSix;
	int returnSeven;
	int returnEight;
	int returnNine;
	int returnTen;

    messageOne = "Thread One";
    messageTwo = "Thread Two";
    messageThree = "Thread Three";
    messageFour = "Thread Four";
    messageFive = "Thread Five";
    messageSix = "Thread Six";
    messageSeven = "Thread Seven";
    messageEight = "Thread Eight";
    messageNine = "Thread Nine";
    messageTen = "Thread Ten";

    // Initialize mutext lock
    pthread_mutex_init(&lock,NULL);

    // This creates the running threads and saves the values for return 
    returnOne = pthread_create(&threadOne, NULL, &addRecordThenRemoveRecord, (void *)messageOne);
    returnTwo = pthread_create(&threadTwo, NULL, &addRecordThenRemoveRecord, (void *)messageTwo);
    returnThree = pthread_create(&threadThree, NULL, &addRecordThenRemoveRecord, (void *)messageThree);
    returnfour = pthread_create(&threadFour, NULL, &addRecordThenRemoveRecord, (void *)messageFour);
    returnFive = pthread_create(&threadFive, NULL, &addRecordThenRemoveRecord, (void *)messageFive);
    returnSix = pthread_create(&threadSix, NULL, &addRecordThenRemoveRecord, (void *)messageSix);
    returnSeven = pthread_create(&threadSeven, NULL, &addRecordThenRemoveRecord, (void *)messageSeven);
    returnEight = pthread_create(&threadEight, NULL, &addRecordThenRemoveRecord, (void *)messageEight);
    returnNine = pthread_create(&threadNine, NULL, &addRecordThenRemoveRecord, (void *)messageNine);
    returnTen = pthread_create(&threadTen, NULL, &addRecordThenRemoveRecord, (void *)messageTen);

    //all threads back to the parent process. 
    pthread_join(threadOne, NULL);
    pthread_join(threadTwo, NULL);
    pthread_join(threadThree, NULL);
    pthread_join(threadFour, NULL);
    pthread_join(threadFive, NULL);
    pthread_join(threadSix, NULL);
    pthread_join(threadSeven, NULL);
    pthread_join(threadEight, NULL);
    pthread_join(threadNine, NULL);
    pthread_join(threadTen, NULL);

    // Printout threads for testing. Remove before submission. 
   // printf("Thread One returns = %d\n", returnOne);
   // printf("Thread Two returns = %d\n", returnTwo);
   // printf("Thread Three returns = %d\n", returnThree);
   // printf("Thread Four returns = %d\n", returnfour);
   // printf("Thread Five returns = %d\n", returnFive);
   // printf("Thread Six returns = %d\n", returnSix);
   // printf("Thread Seven returns = %d\n", returnSeven);
   // printf("Thread Eight returns = %d\n", returnEight);
   // printf("Thread Nine returns = %d\n", returnNine);
   // printf("Thread Ten returns = %d\n", returnTen);

    // This gets rid of the lock at the end of the program. 
    pthread_mutex_destroy(&lock);

    // This prints out the database.
	// This database containts the phonebook 
    printf("\n\n Printing the database: \n");
    printDatabase();
	
    return (EXIT_SUCCESS);
}

 // If the database file is not found it creates one 
 // If the file is already there it just returns the file descriptor 
int init_Database() {
    int fileDescriptor;
    int recordCount = 0;
    if (access(DataBase, F_OK) != 0) {
        fileDescriptor = open(DataBase, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
        write(fileDescriptor, &recordCount, sizeof(int));
        return fileDescriptor;
    }
    return open(DataBase, O_RDWR);
}

 // Checks How many entries are in the database 
 // Then the function uses recordsetter to change the pointer to help go through the database 
 // The while loop goes throught the database and prints the entries 
void printDatabase(){
    int databaseFile = init_Database();
    int recordCount = 0;
    PhoneRecord phone_record;
    lseek(databaseFile, 0, SEEK_SET);
    read(databaseFile, &recordCount, sizeof(int));
    if (recordCount == 0) return;
    recordSetter(databaseFile, recordCount);
    while (recordCount--) {
        read(databaseFile, &phone_record, sizeof(PhoneRecord));
        recordSetter(databaseFile, recordCount);

        printf("Name: %s, \t\tNumber: %ld\n", phone_record.name, phone_record.phone_number);
    }
}

// takes in the phone_record 
// Then gets the database file 
// Then adds the record to the database file. 
// it inserts the record at the front of the file and then inserts the record count 
// Then it reads in storage and place in character arraysize
// After the lseek is done it writes the the storage file. 
void add_record(PhoneRecord *phone_record){

    int storageFile = init_Storage();
    int databaseFile = init_Database();
    int recordCount;
	int filesize;
	int n;
	int i;
	int status; 
    char buffer;
    n = read(databaseFile, &buffer, 1);
    if (n != 1) {           
        read(databaseFile, &recordCount, sizeof(int));
        filesize = lseek(databaseFile, (off_t) 0, SEEK_END);
        for (i = 0 + (sizeof(int));
             i <= filesize; i++) { 
            lseek(databaseFile, (off_t) i, SEEK_SET);
			
            n = read(databaseFile, &buffer, 1);
            if (n != 1) {
                fprintf(stderr, "can't read 1 byte");
                exit(-1);
            }

            n = write(storageFile, &buffer, 1);
            if (n != 1) {
                fprintf(stderr, "can't write 1 byte");
                exit(-1);
            }
        }
        status = remove(DataBase);
        // Lock the file.
        int x=pthread_mutex_lock(&lock);
        databaseFile = init_Database();
        lseek(databaseFile, sizeof(int), SEEK_SET);
        write(databaseFile, phone_record,
              sizeof(PhoneRecord));  
        recordCount++;
        lseek(databaseFile, 0, SEEK_SET);
        write(databaseFile, &recordCount, sizeof(int));      
        char file_read[filesize - (sizeof(int))];
        lseek(storageFile, 0, SEEK_SET);
        read(storageFile, &file_read,
             filesize - (sizeof(int)));  
        lseek(databaseFile, (sizeof(PhoneRecord) + sizeof(int)),
              SEEK_SET); 
        write(databaseFile, &file_read, filesize - (sizeof(int)));  
        status = remove(Storage);
        if (status == 0)
            printf("The Database was deleted successfully.\n");
        else {
            printf("Was Unable to delete the DataBase\n");
            perror("Error");
        }
        printf("Adding Record: Person's Name: %s, Person's Phone number: %ld, Number of entries in the DataBase: %d\n", phone_record->name,
               phone_record->phone_number, recordCount);
    }
    else {

        lseek(databaseFile, 0, SEEK_SET);
        read(databaseFile, &recordCount, sizeof(int));
        recordSetter(databaseFile, recordCount + 1);
        write(databaseFile, phone_record, sizeof(PhoneRecord));
        recordCount++;
        lseek(databaseFile, 0, SEEK_SET);
        write(databaseFile, &recordCount, sizeof(int));

        printf("Adding Record: Person's Name: %s, Person's Phone number: %ld, Number of entries in the DataBase: %d\n", phone_record->name,
               phone_record->phone_number, recordCount);


    }
    // Unlock the file.
    int y=pthread_mutex_unlock(&lock);
}

// This function removes a record from the database. 
// It does this by taking in the name tied into the record.
// If there is no record with the name that is given to the function it does nothing.  
// mutex lock is incorporated into this function as well. 
void remove_record(char *name){
    int databaseFile = init_Database();
    int recordCount = 0;
    int totalRecord = 0;
    PhoneRecord phone_record;
	PhoneRecord last_record;
    lseek(databaseFile, 0, SEEK_SET);
    read(databaseFile, &totalRecord, sizeof(int));
    
	if (totalRecord == 0){
		return;
	}
	
    recordCount = totalRecord;
	recordSetter(databaseFile, totalRecord);
    read(databaseFile, &last_record, sizeof(PhoneRecord));
    int f=pthread_mutex_lock(&lock);
    do {
        recordSetter(databaseFile, recordCount);
        read(databaseFile, &phone_record, sizeof(PhoneRecord));

        if (!strcmp(phone_record.name, name)) {
            if (totalRecord != recordCount) {
                recordSetter(databaseFile, recordCount);
                write(databaseFile, &last_record, sizeof(PhoneRecord));
            }
            lseek(databaseFile, 0, SEEK_SET);
            totalRecord--;
            write(databaseFile, &totalRecord, sizeof(int));
            truncate(DataBase, (sizeof(int) + (totalRecord * sizeof(PhoneRecord))));
            return;
        }
    } while (--recordCount);
    printf("Removing Record: person %s is not in the database\n", name);

    int g=pthread_mutex_unlock(&lock);
}
 
 //This function takes in a name of the person you need the phone number of. 
 // It check how many entries are inside of the database with the record count. 
 // After it figures out how many entries are in the database it starts reading them 
 // If the method finds a match to the name it returns the Phone Number
 // If the method does not find a match it returns a -1
long getPhoneNumber(char* name){
    int databaseFile = init_Database();
    int recordCount = 0;
    PhoneRecord phone_record;
    lseek(databaseFile, 0, SEEK_SET);
    read(databaseFile, &recordCount, sizeof (int));
    if (recordCount == 0){
		return -1;
	} 
    recordSetter(databaseFile, recordCount);
    while (recordCount--) {
        read(databaseFile, &phone_record, sizeof (PhoneRecord));
        recordSetter(databaseFile, recordCount);
        if (!strcmp(phone_record.name, name)) {
            return phone_record.phone_number;
        }
    }
    return -1;
}

// This function does exactly what it is called.
// It first adds the to the database 
// Then it removes the record from the database
// It does this by using the add and remove functions above 
void *addRecordThenRemoveRecord(void *message){
    PhoneRecord phone_records[5];       
    char  *msg;
    msg = (char *) message;
    printf("%s", msg);
    strcpy(phone_records[0].name, "Alex Thomas");
    phone_records[0].phone_number = getpid();
    add_record(&phone_records[0]);
    remove_record("Alex Thomas");
    return  NULL;
}

//This function takes the record number and file descriptor
//After it takes the number it moves the pointer to the positions of the record number. 
void recordSetter(int fileDescriptor, int recordNumber) {
    lseek(fileDescriptor, (recordNumber - 1) * sizeof (PhoneRecord) + sizeof(int), SEEK_SET);
}

// init of the Database, it this has not happened yet. 
// if the file is not there it creates one. 
// The file is initialized and contatins the number of structures in the Database
// If the file is already there it just returns the fileDescriptor 
int init_Storage() {
    int storage;
    int recordCount = 0;
    if (access(Storage, F_OK) != 0) {
        storage = open(Storage, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
        write(storage, &recordCount, sizeof(int));
        return storage;
    }
    return open(Storage, O_RDWR);
}

