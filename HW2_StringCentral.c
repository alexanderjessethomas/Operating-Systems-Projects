/*
 * Antonio Rios
 * Alex Thomas
 * February 14, 2016
 * CS480 Operating Systems
 * Homework 2: String Central
 *
 */

// include any necessary header files
#include <stdio.h>
#include <stdlib.h>



// function prototypes

char* char_double(char* str);

char* char_shift(char* str);

char* char_flip(char* str);

char** string_central(char* (*array[])(char * ) ,char* string);

// main entry point of the program
int main(int argc, char** argv){

	// declare and initialize the variables used to hold the string
	// that will be modified by each function
	char *mystring = "Antonio\0";

	// variable to
	//char *test_str = (char *)malloc(100*sizeof(char));

	// declare and initialize the array of pointers to functions.
	char* (*array_func_pointers[])(char *) = {char_double, char_shift, char_flip, NULL};


	// array to hold the strings returned back from string_central()
	char** string_array = (char**)malloc(sizeof(char ));

	// call the string_central() function passing in the array of function pointers and the string
	// that will be modified by each function.
	// store the strings returned by the function in the string_array variable.
	string_array = string_central(array_func_pointers, mystring);

	// iterate through each string in the array and print out the modified string.
	while(*string_array != NULL){
		printf("___%s___\n", *(string_array++));
	}

	//free the memory allocated for the string array
	free(string_array);

	return 0;
}

/**
 * The char_double( ) function creates a string
 * where each of the characters is doubled so that
 * the result string is twice the size of the input string.
 *
 * Takes in an array of characters which is a string.
 *
 * return: returns a modified string in which each characters is doubled.
 */
char * char_double(char* str){

	// allocate memry to store the new string
	char *output_str = (char *)malloc(100*sizeof(char));

	// variable that does not get modified by the arithmetic pointer.
	// stores the beginning of the string
	// which will be return by the function
	char *return_str;

	// assign the string that gets modified to the return_str variable
	// to keep the pointer at the beginning of the address.
	return_str = output_str;

	// iterate through the string passed in to the function
	// and create the new string with the characters being
	// doubled.
	while(*(str) != '\0'){

		// insert the original character to the new char array
		*(output_str++) =  *(str);

		// insert the same character to make it double
		// and increment the pointers for to insert the next
		// character on the next iteration.
		*(output_str++) = *(str++);

	}

	// terminate the character array to properly have define a string
	*(output_str++) = '\0';

	free(output_str);		// free the memory allocated.

	return return_str;		// return the new string that has been created
}

/**
 * The char_shift( ) function creates a string in which
 * each second character is the successor of the preceding
 * character in the ASCII table. So, for let's say the string Bye you get BCyzef
 *
 * The function takes in an array of characters which is a string.
 *
 * return: returns a string that has been modified by adding for character
 * the successor character next to it.
 */
char * char_shift(char* str){

	// allocate memory for the new string that will be created
	char *output_str = (char *)malloc(100*sizeof(char));

	// variable to hold the address of the first character in the string
	char *return_str;

	// assigned the return_str pointer variable to point to the new
	// string that will be created.
	return_str = output_str;

	// iterate through the string that was passed in to the function
	// and created the new string by the successor for each character in
	// in the string
	while(*(str) != '\0'){

		// add the original character the new character array
		*(output_str++) =  *(str);

		// add the successor character to the new character array
		// and increment he pointers to add the next characters on
		// the next iteration.
		*(output_str++) = 1 + *(str++);

	}

	// terminate the new character array with the zero ascii character
	// to properly create the new string.
	*(output_str++) = '\0';

	free(output_str);		// free the allocated memory for storing the new string

	return return_str; 		// return the new string
}


/**
 * The char_flip( ) function makes a small letter a capital letter and vice versa.
 * So, for Bye you get bYE
 *
 * The function takes in a character array as a parameter which is the string that
 * will be modified.
 *
 * The function returns a new string that is the modified original string with
 * the letters that  were lower-case now are upper-case and vice versa.
 *
 * return: a character array which is the new modified string
 */
char * char_flip(char* str){

	// allocate memory to store the new modified string
	char *output_str = (char *)malloc(100*sizeof(char));

	// declare a variable to point the new string that will be created
	// this will be return by the function since is pointing to the first
	// character of the new string.
	char *return_str;

	// assign the return_str variable to point to the new string that
	// will be created.
	return_str = output_str;

	// iterate through the string that was passed in to the function
	while(*(str) != '\0'){

		// check if the current character is upper-case
		if((*(str) < 97) && (*(str) >= 65)){

			// the letter is upper-case so we add 32 to make it lower case.
			// and store the lower case in the new character array
			// and we increment the pointers to move to the next character in the string
			// and the next slot in the new character array
			*(output_str++) = 32 + *(str++);
		}
		else if((*(str) > 97) && (*(str) <= 122)){ 	// check that the current character is upper-case
			// the letter is lower-case so we subtract 32 to make it upper-case
			// and store the upper-case letter in the new character array.
			// We increment the pointers to move to the next character in the string
			// and the next slot in the new character array.
			*(output_str++) = *(str++) - 32;
		}


	}

	// terminated the new character array the with the ascii zero character to
	// properly terminate the string.
	*(output_str++) = '\0';


	free(output_str); // free the allocated memory

	return return_str;	// return the new string that has bee created.
}

/**
 * String Central takes just two pointers as arguments:
 * The first pointer points to an array containing pointers to functions, i.e. the processors.
 * The second pointer points to the beginning of the array of characters, to be processed by each of the processors.
 * Both arrays need to be properly zero-terminated, i.e. the first array is terminated by NULL and the second array is terminated by ASCII ZERO.
 */
char** string_central(char* (*array[]) (char*), char* str ){

	// allocate memory to store the new gereanted strings to an array
	char** return_str = (char **)malloc(100*sizeof(char*));

	// declare variable to point to the new created array that stores
	// the new modified strings
	char** first;

	// assign the first variable to point to the allocated memory
	// this variable is return since it will point to the first
	// element in the array.
	first = return_str;

	// counter use to represent the index in the array
	int i =0;

	// iterate through the array of function pointers
	while(array[i] != NULL){

		// call the function at index (i) and pass the string that will be modified
		// store the string that is modified and return by the function in to the
		// the new array.
		*(return_str)++ = array[i++](str);

	}

	free (return_str); 	//free the allocated memory

	return (first);		// return the array with the modified strings.
}
