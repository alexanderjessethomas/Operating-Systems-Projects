/**
 * Antonio Rios
 * Alex Thomas
 * January 28, 2016
 * CS 480 Operating Systems
 * Homework-1: String Concatenating function
 ************************************************/

// include the necessary header files
#include <stdio.h>
#include <stdlib.h>


// prototype of the function that concatenates 
char* concat_strings(char** strings);

// main entry point of the program
int main(int argc, char** argv){

    // declare and initialize the variable that will hold the concatenated strings 
    char* new_string = concat_strings(++argv);      // calls the concat_strings() function passing in the argv array 
                                                    // without the first argument

    // catches the exception if the user enters more than 10 arguements
    if(new_string == NULL){
        printf("You entered more arguments than what is allowed.\n");
        printf("There is a limit of 10 arguments.\n");
        return 1;
    }
    // prints to console the concatenated string
    printf("\nThe concatenated string that was returned: %s\n", new_string);


  return 0;
}


/**
 * The concat_strings() function concatenates
 * the arguments that are pass when the program
 * is executed. 
 * 
 * params: char** strings   The function takes in pointer to a character pointer
 * return: char* output_string  The function returns a pointer to the memory location 
 *         storing the concatenated string.
 */
char* concat_strings(char** strings){
    // variable to compute the amount of arguments in strings.
    int sizeOfStrings = 0;


    // allocate memory to store the concatenated string
    //char* output_string = (char *)malloc((sizeof(strings)/sizeof(char))*sizeof(char)); 
    char* output_string = (char *)malloc(100*sizeof(char)); 

    // variable that stores the location of the first character of the concatenated string
    char* first = (char *)malloc(100*sizeof(char)); 
    first = output_string;

//    int k = 1;

    /* Make sure that malloc did not fail and memory
       was allocated correctly */
    if(NULL == output_string){
        fprintf(stderr, "malloc failed\n");
        return NULL;
    }


    // loop through the strings array
    for(char** s = strings; *s !='\0'; s++){

        // loop through each string
        while(*(*s)!= '\0'){
            *(output_string)++ = *(*s)++;      // store the characters for each string
//            k++;

        }

        *(output_string)++ = ' ';           // add a space to seperate each string being concatenated
        sizeOfStrings++;

        if(sizeOfStrings>10){               // sets a limit to 10 arguments that can be passed in from the command line
           return NULL;
        }
        
 //       k++;            //keeps track of where the pointer is in output_string

    }

    // terminate the concatenated string with the ASCII Zero 
    *(output_string)++ = '\0';
    
   // return output_string-k;     // return the concatenated string
    return first;
}

