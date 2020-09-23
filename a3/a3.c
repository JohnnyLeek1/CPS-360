#include <stdio.h>
#include <stdlib.h>

char *getmemblock(int blocksize) {
    char *block = malloc( sizeof(char) * blocksize );

    if (!block) {
        fprintf(stderr, "Error allocating block.\n");
        exit(1);
    }

    return (char *)block;
}

long long rand64() {
    int a, b;
    long long r;

    a = rand();
    b = rand();

    r = (long long)a;
    r = (r << 31) | b;

    return r;
}

void loadbytedata(char mem[], int size) {
    int i = 0;
    for (; i < size; i++) {
        char result = (char)rand64() & 0x7f;
        *mem = result;

        //printf("Byte: %d\n", result);
        
        mem++;
    }

}

int sumbytedata(char mem[], int size) {
    int i = 0, sum = 0;
    for(; i < size; i++) {
        sum += *mem;
        mem++;
    }

    return sum;
}

void loadhalfwdata(char mem[], int size) {
    int i = 0;
    for (; i < size; i++) {
        char result = (char) ((rand64() & 0x7ffff) / 2);
        *mem = result;
        printf("Half word: %d\n", result);
        printf("Half word: %c\n", (char)result);
        mem++;
    }
}

int sumhalfwdata(char mem[], int size) {
    int i = 0, sum = 0;
    for(; i < (size >> 3); i++) {
        sum += *mem;
        mem++;
    }

    return sum;
}

void loadworddata(char mem[], int size) {
    int i = 0;
    for(; i < (size >> 3); i++) {
        *mem = ( (char)rand64() & 0x7fffffff );
        mem++;
    }
}

int sumworddata(char mem[], int size) {
    int i = 0, sum = 0;
    for(; i < (size >> 3); i++) {
        sum += *mem;
        mem++;
    }

    return sum;
}

void loaddoublewdata(char mem[], int size) {
    int i = 0;
    for(; i < (size >> 3); i++) {
        *mem = ( (char)rand64());
        mem++;
    }

}

long long sumdoublewdata(char mem[], int size) {
    int i = 0;
    long long sum = 0;
    for(; i < (size >> 3); i++) {
        sum += *mem;
        mem++;
    }

    return sum;
}

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
