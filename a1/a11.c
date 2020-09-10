#include <stdio.h>
#include <stdlib.h>

/* Find the largest integer in array */
int largest(int d[], int size);

/* Find the smallest integer in the array */
int smallest(int d[], int size);

/* Find the average integer in the array */
int average(int d[], int size);

/* Print the data set */
void printdata(int d[], int size);

/* Print the results from the data */
void printrslts(int largest, int smallest, int average);

/* Entry point of program */
int main(void) {
    int data[11] = { 1, 2, 3, -3, -99, 999, 42, 14, 56, 76, 99 };
    int size;

    size = 11;

    if (size > 0) {
        int largestNum = largest(data, size);
        int smallestNum = smallest(data, size);
        int averageNum = average(data, size);

        /* Print dataset */
        printdata(data, size);

        /* Print results */
        printrslts(largestNum, smallestNum, averageNum);

        printf("%-30s%-20p\n", "Address of main(): ", main);
        printf("%-30s%-20p\n", "Address of data[]: ", data);
        printf("%-30s%-20p\n", "Address of size: ", &size);
        printf("%-30s%-20p\n", "Address of largest(): ", largest);
        printf("%-30s%-20p\n", "Address of largestNum: ", &largestNum);
        printf("%-30s%-20p\n", "Address of smallest(): ", smallest);
        printf("%-30s%-20p\n", "Address of smallestNum: ", &smallestNum);
        printf("%-30s%-20p\n", "Address of average(): ", average);
        printf("%-30s%-20p\n", "Address of averageNum: ", &averageNum);
        printf("%-30s%-20p\n", "Address of printdata(): ", printdata);
        printf("%-30s%-20p\n", "Address of printrslts(): ", printrslts);
    }
    
    exit(0);
}
/*
 *  Function:    largest
 *  ------------------------------------
 *  Iterates through the input array, and finds the largest
 *  number.
 *
 *  d[]: an array of integers to search
 *  size: the size of array d[]
 *
 *  returns: the largest integer contained within d[]
 */
int largest(int d[], int size) {
    int largestNum = d[0];

    for (int i = 0; i < size; i++) {
        if (d[i] > largestNum) {
            largestNum = d[i];
        }
    }

    return largestNum;
}

/*
 *  Function:   smallest
 *  ------------------------------------
 *  Iterates through the input array, and finds the smallest
 *  number.
 *
 *  d[]: an array of integers to serach
 *  size: the size of array d[]
 *
 *  returns: the smallest integer contained within d[]
 */ 
int smallest(int d[], int size) {
    int smallestNum = d[0];

    for (int i = 0; i < size; i++) {
        if (d[i] < smallestNum) {
            smallestNum = d[i];
        }
    }

    return smallestNum;
}

/*
 *  Function:   average
 *  ------------------------------------
 *  Iterates through the input array, and computes
 *  the average number. Computation is as follows:
 *      sum of all numbers / size of array
 *
 *  d[]: an array of integers to compute the average from
 *  size: the size of array d[]
 *
 *  returns: the average number as computed from the array
 */
int average(int d[], int size) {
    int sum = 0;

    for (int i = 0; i < size; i++) {
        sum += d[i];
    }

    return sum / size;
}

/*
 *  Function:   gtrAverage
 *  ------------------------------------
 *  Iterates through the input array, and computes
 *  a count of numbers that are greater than the
 *  average number.
 *
 *  d[]: an array of integers to collect count from
 *  size: the size of array d[]
 *
 *  returns: a count of numbers that are greater than
 *           the provided average
 */
int gtrAverage(int d[], int size, int average) {
    int count = 0;

    for (int i = 0; i < size; i ++) {
        if (d[i] > average) {
            count += 1;
        }
    }

    return count;
} 


/*
 *  Function:   printdata
 *  ------------------------------------
 *  Prints the provided dataset in a readable 
 *  format. Every number is right justified, with
 *  8 spaces between.
 *
 *  d[]: an array of integers to print
 *  size: the size of array d[]
 *
 *  returns: void
 */ 
void printdata(int d[], int size) {
    /* printf("Data: ");*/
    for (int i = 0; i < size; i++) {
        /* printf("%8d ", d[i]); */
    }

    /* printf("\n"); */
}

/*
 *  Function:   printrslts
 *  ------------------------------------
 *  Prints the results from the computations
 *  of the dataset. Labels are left justified
 *  with 20 spaces and results are right justified
 *  with 20 spaces.
 *
 *  largset:            an integer representing the largest number
 *  smallest:           an integer representing the smallest number
 *  average:            an integer representing the average
 *
 *  returns: void
 */ 
void printrslts(int largest, int smallest, int average) {
    /* printf("Results:\n");
    printf("%-20s%20d\n", "Largest Int: ", largest);
    printf("%-20s%20d\n", "Smallest Int: ", smallest);
    printf("%-20s%20d\n", "Average Int: ", average);
    printf("%-20s%20d\n", "Count Above Avg: ", countAboveAverage); */
}
