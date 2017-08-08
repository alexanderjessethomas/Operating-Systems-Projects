/**
 * Antonio Rios
 * Alex Thomas
 * March 12, 2016
 * CS 480 Operating Systems
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define BLOCK_SIZE 8

// prototype functions used to reverse the block that was read.
void reverse_string(char* s);


/**
 * The main entry point of the of the program which takes in the
 * sourcefile and destination file from the command line and stored
 * in the argv array.
 *
 * The program reads blocks of characters from the source file,
 * reverses the block, and finally writes it to the destination file.
 */
int main(int argc, char** argv){

	//buffer to store what is being read, allocate a size of the blocksize being read.
	char* word = (char*)malloc((BLOCK_SIZE)*sizeof(char));

	// variable used to store the length of the file
	int length =0;

	char* source = argv[1];	// stores the source file name passed in from the command line
	char* dest = argv[2]; 	// stores the destination file name passed in from the command line

	// create the file handlers.
	int source_file = open(source, O_RDONLY);		// file handler to read from the file.
	int dest_file = open(dest, O_RDWR);				// file handler to write to the file.


	// move the file pointer to the end to start reading
	// the source file from the end. This will return the length
	if( (length = lseek(source_file, 0, SEEK_END)) < 0){			// the file pointer has been moved to the end.

		printf("An error occur while trying to read the file.");

	}

	// bsize is used to iterate through the file in chunks of BLOCK_SIZE bytes.
	int bsize = BLOCK_SIZE;

	//iterate through the file until we have reached the beginning of the file.
	while((lseek(source_file, -bsize, SEEK_END)) > 0){

		// read a chunk of BLOCK_SIZE bytes starting from the end of the file.
		read(source_file, word, BLOCK_SIZE);
		*(word+bsize) = '\0';
		// call the reverse_string() function to reverse the block of bytes
		reverse_string(word);

		// write the reversed block of words to the destination file.
		write(dest_file, word, BLOCK_SIZE);

		//printf("%s", word);		// print to the screen what was written to the file.

		bsize += BLOCK_SIZE;	// increase the bsize to move to the next BLOCK_SIZE in the file.
	}

	// compute the remaining amount of bytes that are left to read which are not of BLOCK_SIZE.
	int amt_blocks = length % BLOCK_SIZE;

	// move the file pointer from the end of the file to the beginning to read the left over characters.
	lseek(source_file, -length, SEEK_END);

	// read the left over characters.
	read(source_file, word, amt_blocks);
	// terminate the character string that was read from the file since is not of BLOCK_SIZE.
	*(word+amt_blocks) = '\0';

	// reverse the last amount of bytes read.
	reverse_string(word);
	// write the reversed characters to the destination file.
	write(dest_file, word, amt_blocks);

	//printf("%s\n", word);	// print to console what was written to the destination file.
	printf("Completed writing to destination file.");	// print to console what was written to the destination file.

	// close the file handlers.
	close(source_file);
	close(dest_file);

	return 0;
}

/**
 * The reverse_string() function reverses an array of characters
 * that is passed in to the function.
 *
 * params:
 *  char* str - represents an array of characters that will be reversed.
 *
 * return:
 *  The function returns nothing, however it modifies the array of characters
 *  that was pass to the function.
 */
void reverse_string(char* str){

	// counters use to iterate through the array.
	int j = 0, i = 0;

	while(*(str+(j+1))) j++;	// move the pointer to the end of the array

	// iterate in reverse order to reverse the elements in the array.
	while(i < j) {

		char temp = *(str+i); // stores the current character in a temp variable
		// swap the characters.
		*(str+i) = *(str+j);
		*(str+j) = temp;

		// increment the counters to move to the next element in the array.
		i++;
		j--;
	}
}

