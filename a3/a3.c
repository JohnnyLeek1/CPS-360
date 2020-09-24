/*
 *  Name: Johnny Leek                       Class: CPS 360
 *  Section: 22383332 (M/W/F 12PM)          Assignment: 03
 *  Due: September 14, 2020                 Started: September 23, 2020
 *  Credit: 10 Points
 *
 *  Problem: Write a program that takes in a user-defined memory size from the command line. 
 *           The user provided parameter is the amount of bytes to allocate to the defined char
 *           array. Next, store numbers of each size going from byte (8-bit), half-word (16-bit),
 *           word (32-bit), and double-word (64-bit).
 *
 *           After storing the numbers, iterate through the dynamically allocated array and sum
 *           the values of each number and print the numbers to the command line.
 *
 *           You cannot use arithmetic operators * or /, and should opt to use bitwise operators instead.
 *           You cannot assume any user input. You must sanitize the input to ensure that the program does
 *           not break.
 *
 *  Solution: We start by declaring our dynamically allocated array attempting to assign the block size
 *            that the user provided. From there we then start allocating our data types.
 *
 *            From there, we load the byte data into our array. This is implemented easily because each
 *            index in the array can support one byte, so we just assign the values for each index. The
 *            only thing to keep in mind is that rand() generates a 32-bit integer. Because of this, we must
 *            mask the output to only obtain the lower 8 bits.
 *
 *            To sum the bytes, we just iterate through the mem[] array and keep track of a running total of
 *            the sum to return.
 *
 *            We repeat this process for the other data types: half-word, word, and double-word, but they are
 *            slightly more complicated.
 *
 *            For half-words, we first generate the input, and mask the randomly generated integer to provide us
 *            with 15 bits. We shift the result 8 bits to the right to get the most significant 8 bits, and assign
 *            that to the first memory address available, we then assign the second memory address the other 8 bits,
 *            again using a bitmask. We then set the next available memory address to be used in the next iteration
 *            of the loop.
 *
 *            To sum the half-words, we run this process in reverse. We first get the 8 bits from the first memory address,
 *            and then concatenate them with the next 8 bits using the | (bitwise OR) operator. We mask the second
 *            set of bits using 0xff to only select the 8 bits from that integer, so as to not override the rest
 *            of the bits.
 *
 *            We repeat the above process for word, and double-word, though they require more bitshifts, and fewer of them
 *            can be stored in the memory array. Something else to keep in mind is that we must cast (long long) to the
 *            double-word in it's sum function to prevent the compiler from converting it back to an integer, which would
 *            cause unexpected errors/results.
 *
 *            Before the program quits, we must make sure to free the memory that we initially used.i
 *
 *  Data Structures: The primary data structure at play here is a dynamically allocated char array
 *
 *  Accessing functions for the data structure: We access the data structure and modify its contents in the following functions:
 *                                                  - getmemblock()
 *                                                  - loadbytedata()
 *                                                  - sumbytedata()
 *                                                  - loadhalfwdata()
 *                                                  - sumhalfwdata()
 *                                                  - loadworddata()
 *                                                  - sumworddata()
 *                                                  - loaddoublewdata()
 *                                                  - sumdoublewdata()
 *
 *  Errors handled: The program will exit if the user input does not match the following conditions:
 *                      - block size is <= 0
 *                      - block size is not divisible by 8
 *                      - block size is not a number
 *
 *                  The program will also exit if malloc fails when attempting to dynamically allocate
 *                  memory to the array.
 *
 *  Limitations: This program will not work for values less than 8, even though technically speaking, the data
 *               types of byte, half-word, and word can all function on 1, 2 and 4 bytes respectively
 *
 *  Acknowledgements: Owen Smith and I bounced ideas off of each other through out this assignment and helped each
 *                    other debug issues.
 *                    Tyler Moses and I also helped debug each other's programs and helped each other comprehend
 *                    the assignment description.
 */

#include <stdio.h>
#include <stdlib.h>

/*
 * Allocates memory block of defined size blocksize. Returns
 * pointer to this memory block. This function also checks
 * if the malloc() called failed, and will exit the
 * program is this is the case.
 */
char *getmemblock(int blocksize) {
    char *block = malloc( sizeof(char) * blocksize );

    if (!block) {
        fprintf(stderr, "Error allocating block.\n");
        exit(1);
    }

    return (char *)block;
}

/*
 * Generates a random 64-bit integer
 */
long long rand64() {
    int a, b;
    long long r;

    a = rand();
    b = rand();

    r = (long long)a;
    r = (r << 31) | b;

    return r;
}

/*
 * Fills up the array with randomly generated
 * 7-bit integers.
 */
void loadbytedata(char mem[], int size) {
    int i = 0, result;
    for (; i < size; i++) {
        result = rand() & 0x7f;
        *mem = (char)result; 
        mem++;
    }

}

/*
 * Sums the values in the array, assuming
 * they are 7-bit integers.
 */
int sumbytedata(char mem[], int size) {
    int i = 0, sum = 0;
    for(; i < size; i++) {
        sum += *mem;
        mem++;
    }

    return sum;
}

/*
 * Fills up the array with randomly generated
 * 15-bit integers.
 */
void loadhalfwdata(char mem[], int size) {
    int i, result;
    for (i = 0; i < (size >> 1); i++) {
        result = (rand() & 0x7fff);
        *mem = (char)(result >> 8);
        *(mem + 1) = (char)(result & 0xff);
        mem += 2;
    }
}

/*
 * Sums the values in the array, assuming
 * they are 15-bit integers.
 */
int sumhalfwdata(char mem[], int size) {
    int i, sum, result;
    sum = 0;
    for (i = 0; i < (size >> 1); i++) {
        result = (*mem << 8);
        result |= (*(mem + 1) & 0xff);
        sum += result;
        mem += 2;
    }
    return sum;
}

/*
 * Fills up the array with randomly generated
 * 31-bit integers.
 */
void loadworddata(char mem[], int size) {
    int i, result;
    for(i = 0; i < (size >> 2); i++) {
        result = (rand() & 0x7fffffff);
        *mem = (char)(result >> 24);
        *(mem + 1) = (char)(result >> 16 & 0xff);
        *(mem + 2) = (char)(result >> 8 & 0xff);
        *(mem + 3) = (char)(result & 0xff);
        mem += 4;
    }
}

/*
 * Sums the values in the array, assuming
 * they are 31-bit integers.
 */
int sumworddata(char mem[], int size) {
    int i, sum, result;
    sum = 0;
    for(i = 0; i < (size >> 2); i++) {
        result = (*mem & 0xff) << 24;
        result |= (*(mem + 1) & 0xff) << 16;
        result |= (*(mem + 2) & 0xff) << 8;
        result |= (*(mem + 3) & 0xff);

        sum += result;
        mem += 4;
    }
    return sum;
}

/*
 * Fills up the array with randomly generated
 * 63-bit integers.
 */
void loaddoublewdata(char mem[], int size) {
    int i;
    long long result;
    for(i = 0; i < (size >> 3); i++) {
        result = rand64();
        *mem = (char)(result >> 56);
        *(mem + 1) = (char)(result >> 48 & 0xff);
        *(mem + 2) = (char)(result >> 40 & 0xff);
        *(mem + 3) = (char)(result >> 32 & 0xff);
        *(mem + 4) = (char)(result >> 24 & 0xff);
        *(mem + 5) = (char)(result >> 16 & 0xff);
        *(mem + 6) = (char)(result >> 8  & 0xff);
        *(mem + 7) = (char)(result & 0xff);

        mem++;
    }

}

/*
 * Sums the values in the array, assuming
 * they are 63-bit integers.
 */
long long sumdoublewdata(char mem[], int size) {
    int i;
    long long sum = 0;
    long long result;
    for(i = 0; i < (size >> 3); i++) {
        result = (long long)(*mem & 0xff) << 56;
        result |= (long long)(*(mem + 1) & 0xff) << 48;
        result |= (long long)(*(mem + 2) & 0xff) << 40;
        result |= (long long)(*(mem + 3) & 0xff) << 32;
        result |= (long long)(*(mem + 4) & 0xff) << 30;
        result |= (long long)(*(mem + 5) & 0xff) << 24;
        result |= (long long)(*(mem + 6) & 0xff) << 16;
        result |= (long long)(*(mem + 7) & 0xff);

        sum += result;

        mem += 8;
    }

    return sum;
}

/*
 * Prints the results in a readable format
 */
void printresult(int bytesum, int hwsum, int wordsum, long long dwsum) {
    printf("%-30s: %-20d\n", "Sum of bytes", bytesum);
    printf("%-30s: %-20d\n", "Sum of half-words", hwsum);
    printf("%-30s: %-20d\n", "Sum of words", wordsum);
    printf("%-30s: %-20lld\n", "Sum of double-words", dwsum);
}

/*
 * Prints the usage information for the program to stderr and exits
 */
void usage(char *progname) {
    fprintf(stderr, "Error. Usage: %s <blocksize>\n\nBlocksize must be divisible by 8.\n", progname);
    exit(1);
}

/*
 * Entry point of program. First checks if the
 * user input is valid, and then fills up the
 * array with each data type
 */
int main( int argc, char *argv[] ) {
    char *blockpointer = NULL;
    int bytesum, hwsum, wordsum;
    long long dwsum;

    if (argc == 1) {
        usage(argv[0]);
    }

    int blocksize = atoi(argv[1]);

    if (blocksize <= 0) {
        fprintf(stderr, "Error. Please enter a blocksize greater than 0\n");
        exit(1);
    }

    if ( ((blocksize >> 3) << 3) != blocksize ) {
        fprintf(stderr, "Error. Blocksize must be divisible by 8.\n");
        exit(1);
    }

    srand(1);
   
    blockpointer = getmemblock(blocksize);

    loadbytedata(blockpointer, blocksize);
    bytesum = sumbytedata(blockpointer, blocksize);

    loadhalfwdata(blockpointer, blocksize);
    hwsum = sumhalfwdata(blockpointer, blocksize);

    loadworddata(blockpointer, blocksize);
    wordsum = sumworddata(blockpointer, blocksize);

    loaddoublewdata(blockpointer, blocksize);
    dwsum = sumdoublewdata(blockpointer, blocksize);

    printresult(bytesum, hwsum, wordsum, dwsum);

    free(blockpointer);
    exit(0);

}
