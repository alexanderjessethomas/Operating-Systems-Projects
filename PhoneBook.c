#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

/*
 * Authors: Alex Varela, Lucas Rayburn
 * CS 480
 * Homework 5: Race Conditions 1
 *
 * This program is the parent, which spawns many children.  I used the gcc compiler to make an
 * executable.  The child program has to be named addRemove when compiled with gcc.
 * This is shown in the execvp function.
 *
 */

#define DB ("Phone Book.txt")
#define stor ("storage_book.txt")

typedef struct phone_record {
    int phone_number; // We had trouble with the ASCII so we used a long for the phone number
    char name  [20];
} PhoneRecord;


/*
 * aux function, initializes the database file (if it is not there)
 *
 * return the filedescriptor of the opened db file
 */
int init_db() {
    int fd;
    int record_count = 0;

    // init of dbfile, if not there yet
    if (access(DB, F_OK) != 0) {
        // the file is not accessible, create it ...
        fd = open(DB, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
        // initialize the beginning of the file, which contains the number of structs the db contains
        write(fd, &record_count, sizeof(int));
        return fd;
    }
    // the file is there, just return the fd
    return open(DB, O_RDWR);
}


int init_stor() {
    int storage;
    int record_count = 0;

    // init of dbfile, if not there yet
    if (access(stor, F_OK) != 0) {
        // the file is not accessible, create it ...
        storage = open(stor, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
        // initialize the beginning of the file, which contains the number of structs the db contains
        write(storage, &record_count, sizeof(int));
        return storage;
    }
    // the file is there, just return the fd
    return open(stor, O_RDWR);
}

//-----------------------------------------------------------------------
/*
 * aux function to help position the internal file pointer
 * take the record number and put the pointer to that position
 */
void set_to_record(int fd, int recordNumber) {
    lseek(fd, (recordNumber - 1) * sizeof (PhoneRecord) + sizeof(int), SEEK_SET);
}


//-----------------------------------------------------------------------
/*
 * Prints all of the people and id numbers in the database
 *
 */
void printDB() {
    int dbfile = init_db();
    int record_count = 0;
    PhoneRecord phone_record;

    // see, how many entries the db has ...
    lseek(dbfile, 0, SEEK_SET);
    read(dbfile, &record_count, sizeof(int));
    if (record_count == 0) return;

    // set the pointer to the last entry
    set_to_record(dbfile, record_count);
    // run through all entries
    while (record_count--) {
        read(dbfile, &phone_record, sizeof(PhoneRecord));
        set_to_record(dbfile, record_count);

        printf("Name: %s, \t\tNumber: %ld\n", phone_record.name, phone_record.phone_number);
    }
}

// Main function which is the parent.
int main(int argc, char** argv)
{
    int i, status;
    pid_t children[10]= {0,0,0,0,0,0,0,0,0,0};
    char *args[] = {"./addRemove", NULL};
    //char * path = "";
    //char * file = "";
    //args[0] = file;



    //args[0] = addRemove(getpid()); put the file name here for exec

    // set up the non sequential forks.  Making sure the children can't call fork.
    for (int c = 0; c< 10; c++)
    {
        children[c] = fork();
        // if it is a child continue on.
        if (children[c] == 0) break;
    }
/*
    // if any forks fail.
    if((pid0 || pid1 || pid2 || pid3 || pid4 || pid5 || pid6 || pid7 || pid8 || pid9)< 0)
    {
        perror("fork() error");
        exit(-1);
    }*/

    if(children[0] && children[1] && children[2] && children[3] && children[4] && children[5]
       && children[6] && children[7] && children[8] && children[9])//parent
    {

        // The father waits for the children to return
        // this code is tricky, as the father may be caught by a signal right in wait
        // which accounts for a number of false waits.  To avoid, we wait one more time
        // when this happens.

        // Initialize i to 10 because of 10 processes.
        i = 10;
        while (i--)
        {
            int return_from_wait = wait(&status);
            if (return_from_wait == -1) {
                i++;// this wait went wrong, so wait this one more.
                //perror("Error wait");
            }
            else {
                printf("Process %d returned\n", return_from_wait);
            }
        }

        printf("Hello I am the parent! %d\n", getpid());

       // Print the database hopefully after every child is completed.
        // I am having some trouble printing out the database, but my output as the program runs shows how many
        //entries there are and shows what it is doing to the database.
        printDB();

    }
        // HERE is where the child will use exec to call the addRemove function for all children when the child
        // processes are going.
    else if (children[0] == 0)
    {
        // This sets up the execvp call.  I use gcc and compile both the
        // parent and child processes into an executable. Then I call the
        // parent code.  The child has to have an executable with the
        // name 'addRemove'.

        // example of gcc compilation code:
        // gcc addRemove.c -o addRemove

        printf("Hello I am the child %d\n", getpid());

        // If there is an error in the exec call, print error.
        if(execvp("./addRemove", args) < 0)
        {
            perror("Fork() error");
        }

        exit(0);
    }

     // If child 2
    else if (children[1] == 0)
    {
        // This sets up the execvp call.  I use gcc and compile both the
        // parent and child processes into an executable. Then I call the
        // parent code.  The child has to have an executable with the
        // name 'addRemove'.

        // example of gcc compilation code:
        // gcc addRemove.c -o addRemove

        printf("Hello I am the child %d\n", getpid());

        // If there is an error in the exec call, print error.
        if(execvp("./addRemove", args) < 0)
        {
            perror("Fork() error");
        }
        exit(0);
    }

        // If child 3
    else if (children[2] == 0)
    {
        // This sets up the execvp call.  I use gcc and compile both the
        // parent and child processes into an executable. Then I call the
        // parent code.  The child has to have an executable with the
        // name 'addRemove'.

        // example of gcc compilation code:
        // gcc addRemove.c -o addRemove

        printf("Hello I am the child %d\n", getpid());

        // If there is an error in the exec call, print error.
        if(execvp("./addRemove", args) < 0)
        {
            perror("Fork() error");
        }
        exit(0);

    }


        // If child 4
    else if (children[3] == 0)
    {
        // This sets up the execvp call.  I use gcc and compile both the
        // parent and child processes into an executable. Then I call the
        // parent code.  The child has to have an executable with the
        // name 'addRemove'.

        // example of gcc compilation code:
        // gcc addRemove.c -o addRemove

        printf("Hello I am the child %d\n", getpid());

        // If there is an error in the exec call, print error.
        if(execvp("./addRemove", args) < 0)
        {
            perror("Fork() error");
        }
        exit(0);

    }

        // If child 5
    else if (children[4] == 0)
    {
        // This sets up the execvp call.  I use gcc and compile both the
        // parent and child processes into an executable. Then I call the
        // parent code.  The child has to have an executable with the
        // name 'addRemove'.

        // example of gcc compilation code:
        // gcc addRemove.c -o addRemove

        printf("Hello I am the child %d\n", getpid());

        // If there is an error in the exec call, print error.
        if(execvp("./addRemove", args) < 0)
        {
            perror("Fork() error");
        }
        exit(0);

    }


        // If child 6
    else if (children[5] == 0)
    {
        // This sets up the execvp call.  I use gcc and compile both the
        // parent and child processes into an executable. Then I call the
        // parent code.  The child has to have an executable with the
        // name 'addRemove'.

        // example of gcc compilation code:
        // gcc addRemove.c -o addRemove

        printf("Hello I am the child %d\n", getpid());

        // If there is an error in the exec call, print error.
        if(execvp("./addRemove", args) < 0)
        {
            perror("Fork() error");
        }
        exit(0);

    }


        // If child 7
    else if (children[6] == 0)
    {
        // This sets up the execvp call.  I use gcc and compile both the
        // parent and child processes into an executable. Then I call the
        // parent code.  The child has to have an executable with the
        // name 'addRemove'.

        // example of gcc compilation code:
        // gcc addRemove.c -o addRemove

        printf("Hello I am the child %d\n", getpid());

        // If there is an error in the exec call, print error.
        if(execvp("./addRemove", args) < 0)
        {
            perror("Fork() error");
        }
        exit(0);

    }


        // If child 8
    else if (children[7] == 0)
    {
        // This sets up the execvp call.  I use gcc and compile both the
        // parent and child processes into an executable. Then I call the
        // parent code.  The child has to have an executable with the
        // name 'addRemove'.

        // example of gcc compilation code:
        // gcc addRemove.c -o addRemove

        printf("Hello I am the child %d\n", getpid());

        // If there is an error in the exec call, print error.
        if(execvp("./addRemove", args) < 0)
        {
            perror("Fork() error");
        }
        exit(0);

    }


        // If child 9
    else if (children[8] == 0)
    {
        // This sets up the execvp call.  I use gcc and compile both the
        // parent and child processes into an executable. Then I call the
        // parent code.  The child has to have an executable with the
        // name 'addRemove'.

        // example of gcc compilation code:
        // gcc addRemove.c -o addRemove

        printf("Hello I am the child %d\n", getpid());

        // If there is an error in the exec call, print error.
        if(execvp("./addRemove", args) < 0)
        {
            perror("Fork() error");
        }
        exit(0);

    }

        // If child 10
    else if (children[9] == 0)
    {
        // This sets up the execvp call.  I use gcc and compile both the
        // parent and child processes into an executable. Then I call the
        // parent code.  The child has to have an executable with the
        // name 'addRemove'.

        // example of gcc compilation code:
        // gcc addRemove.c -o addRemove

        printf("Hello I am the child %d\n", getpid());

        // If there is an error in the exec call, print error.
        if(execvp("./addRemove", args) < 0)
        {
            perror("Fork() error");
        }
        exit(0);

    }

    //printf("My process ID : %d\n", getpid());


    return (EXIT_SUCCESS);
}