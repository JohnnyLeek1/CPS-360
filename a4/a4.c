/*
 *  Name: Johnny Leek                       Class: CPS 360
 *  Section: 22383332 (M/W/F 12PM)          Assignment: 04
 *  Due: October 5, 2020                    Started: October 4, 2020
 *  Credit: 10 Points
 *
 *  Problem: Write a program to simulate a number of combinational circuits. The input to circuits
 *           will be specified via command line arguments in base 16. Multiple input bits will be
 *           specified with bits in arg-a or arg-b or both and carry-in in arg-c. The program should
 *           not contain any control-flow constructs except for the checking and parsing or command
 *           line arguments.
 *
 *           Do not use any arithmetic operators, use bitwise operators only.
 *           You cannot assume any user input. You must sanitize the input to ensure that the pgoram
 *           does not break.
 *
 *  Solution: The solution to each circuit is relatively simple. We just implement the logic diagram
 *            using code and bitwise operators.
 *            Half adder:
 *              - We take the LSB's of a and b (the lowest-order bit)
 *              - The carry bit is the result of a & b
 *              - The sum bit is the result of a ^ b
 *           Full adder:
 *              - Take the LSB's of a, b, and c (carry)
 *              - The sum = (a ^ b) ^ c
 *              - The carry = (a & b) | ( (a ^ b) & c )
 *          Ripple Carry 4-bit:
 *              - TO-DO
 *          Even Parity 3-bit Generator:
 *              - a, b, c = lower 3 bits of a
 *              - Parity bit = ( a ^ b ) ^ c
 *          Odd Parity 4-bit Checker:
 *              - a, b, c, d = lower 4 bits of a
 *              - abcheck = ~(a ^ b)
 *              - cdcheck = ~(c ^ d)
 *              - paritycheck = ~(abcheck ^ cdcheck)
 *          2x1 mux:
 *              - i1, i2 = lower 2 bits of a
 *              - selection bit = LSB of b
 *              - i2out = i2 & selection
 *              - i1out = i1 & ~selection
 *              - yout = i2out | i1out
 *          4x1 mux:
 *              - i0, i1, i2, i3 = lower 4 bits of a
 *              - s0, s1 = lower 2 bits of b
 *              - i0out = ~s0 & ~s1 & i0
 *              - i1out = s0 & ~s1 & i1
 *              - i2out = ~s0 & s1 & i2
 *              - i3out = s0 & s1 & i3
 *              - yout = (i3out | i2out | i1out | i0out)
 *  
 *  Data Structures: No data structures were used in this program
 *
 *  Accessing functions for the data structure: N/A (No data structures used)
 *
 *  Errors handled: The program will exit if the input does not match the following conditions:
 *                      - invalid number of args
 *                      - args cannot be fully parsed into a number (check for null termination character in the endptr)
 *
 *  Limitations: For the most part, the program only operates on the lowest 4 bits of any given integer, so the other bits
 *               in the integer are irrelevant.
 *
 *  Acknowledgements: Owen Smith and I helped each other out, and I showed him how to test the user input for validity
 *                    I also helped Tyler Moses with parts of his program.
 */
#include <stdio.h>
#include <stdlib.h>

/*
 * Function taken from 360handouts > bitprint.c
 * Takes a 32 bit integer and prints the value in
 * base 2 (byte by byte)
 */
void printbits(int x) {
    int i, j;
    for(i = 31, j = 0; i >= 0; i--, j++) {
        if ((j & 0x7)  == 0) putchar(' ');
        (x & (1 << i)) ? putchar('1') : putchar('0');                        
    }
    printf("\n");        
}

/*
 * Simulates a half-adder circuit on bit 0 of a and b.
 * Sets the values of *sum and *outcarry to the sum
 * bit and the carry bit
 */
void halfadder(int a, int b, int *sum, int *outcarry) {
    *sum = (int) (a & 1) ^ (b & 1);
    *outcarry = (int) (a & 1) & (b & 1);
}

/*
 * Simulates a full-adder circuit on bit 0 of a, b, and incarry.
 * Sets the values of *sum and *outcarry to the sum bit and the
 * carry bit
 */
void fulladder(int a, int b, int incarry, int *sum, int *outcarry) {
    *sum = (int) ( (a & 1) ^ (b & 1) ) ^ (incarry & 1);
    *outcarry = ( (a & 1) & (b & 1) ) | ( ((a & 1) ^ (b & 1)) & (incarry & 1) );
}

/*
 * Simulates the ripple-carry scheme and 4 full-adders to add lower order
 * 4-bits of a and b and incarry is in c
 */
void ripcarryadd4(int a, int b, int incarry, int *sum, int *outcarry) {
    int a0, a1, a2, a3;
    int b0, b1, b2, b3;
    int s0, s1, s2, s3;
    int c0, c1, c2, c3;

    a0 = a & 1;
    b0 = b & 1;

    fulladder(a0, b0, incarry, &s0, &c0);

    a1 = (a >> 1) & 1;
    b1 = (b >> 1) & 1;

    fulladder(a1, b1, c0, &s1, &c1);

    a2 = (a >> 2) & 1;
    b2 = (b >> 2) & 1;

    fulladder(a2, b2, c1, &s2, &c2);

    a3 = (a >> 3) & 1;
    b3 = (b >> 3) & 1;

    fulladder(a3, b3, c2, &s3, &c3);

    *sum = (s3 << 3) | (s2 << 2) | (s1 << 1) | s0;
    *outcarry = c3;
}

/*
 * Generates even parity bit for lower order 3 bits in a
 */
void evenparity3gen(int a, int *paritybit) {
    int xor = ( (a >> 2) & 1 ) ^ ( (a >> 1) & 1 );
    *paritybit = xor ^ (a & 1);
}

/*
 * Checks odd parity of the lower order 4 bits of a
 */
void oddparity4check(int a, int *checkbit) {
    *checkbit = ~(( ~(((a >> 3) & 1) ^ ((a >> 2) & 1)) & 1 ) ^ ( ~(((a >> 1) & 1) ^ (a & 1)) )) & 1; 
}

/*
 * Connects one of the 2 inputs (lowest order 4-bits
 * from a) to y-out based on selection input (lowest 
 * order bit of b)
 */
void mux2by1(int a, int b, int *yout) {
    *yout = ((a & 1) & (b & 1)) | ( ((a >> 1) & 1) & (~(a >> 1) & 1) );
}

/*
 * Connects one of the four inputs (lower order 4-bits
 * of a) to out based on selection input (lowest order bit
 * of b)
 */
void mux4by1(int a, int b, int *yout) {
    int lower4bits = ((1 << 4) - 1) & a;
    int selectionbits = ((1 << 2) - 1) & b;

    int i0, i1, i2, i3;
    int s0, s1;

    s0 = (selectionbits >> 1) & 1;
    s1 = selectionbits & 1;

    i0 = lower4bits & 1;
    i1 = (lower4bits >> 1) & 1;
    i2 = (lower4bits >> 2) & 1;
    i3 = (lower4bits >> 3) & 1;

    int nots0 = ~s0 & 1;
    int nots1 = ~s1 & 1;

    int i3out = s0 & s1 & i3;
    
    int i2out = nots0 & s1 & i2;

    int i1out = s0 & nots1 & i1;

    int i0out = nots0 & nots1 & i0;

    

    *yout = (i3out | i2out | i1out | i0out);
}

/*
 * Prints all bits in a as 8-bit groups (with the
 * corresponding label)
 */
void printresult(char *label, int a) {
    printf("%-20s", label);
    printbits(a);
}

/*
 * Prints the usage information for the program to stderr and exits
 */
void usage(char *progname) {
    fprintf(stderr, "Error. Usage %s <arg-a> <arg-b> <arg-c>\n", progname);
    exit(1);
}

int main( int argc, char *argv[] ) {
    if (argc != 4) {
        usage(argv[0]);
    }

    char *endptra = NULL, *endptrb = NULL, *endptrc = NULL;
    int a, b, c;

    a = strtol(argv[1], &endptra, 0);
    
    if(*endptra != '\0') {
        fprintf(stderr, "Error. arg-a is invalid\n");
        exit(1);
    }

    b = strtol(argv[2], &endptrb, 0);
    if(*endptrb != '\0') {
        fprintf(stderr, "Error. arg-b is invalid\n");
        exit(1);
    }
    
    c = strtol(argv[3], &endptrc, 0);
    if(*endptrc != '\0') {
        fprintf(stderr, "Error. arg-c is invalid\n");
        exit(1);
    }

    int sum = 0, outcarry = 0, paritybit = 0, checkbit = 0;
    int yout = 0;

    printf("Half Adder:\n");
    halfadder(a, b, &sum, &outcarry);
    printresult("Sum:", sum);
    printresult("Carry:", outcarry);
    printf("\n");

    printf("Full Adder:\n");
    fulladder(a, b, c, &sum, &outcarry);
    printresult("Sum:", sum);
    printresult("Carry:", outcarry);
    printf("\n");

    printf("Ripple Carry 4-bit Adder:\n");
    ripcarryadd4(a, b, c, &sum, &outcarry);
    printresult("Sum:", sum);
    printresult("Carry:", outcarry);
    printf("\n");

    printf("Even Parity Generator:\n");
    evenparity3gen(a, &paritybit);
    printresult("Parity Bit:", paritybit);
    printf("\n");

    printf("Odd Parity Checker:\n");
    oddparity4check(a, &checkbit);
    printresult("Check Bit:", checkbit);
    printf("\n");

    printf("2x1 Multiplexer:\n");
    mux2by1(a, b, &yout);
    printresult("Y-out:", yout);
    printf("\n");

    printf("4x1 Multiplexer:\n");
    mux4by1(a, b, &yout);
    printresult("Y-out:", yout);
    printf("\n");

    exit(0);
}
