#include <stdio.h>
#include <stdlib.h>

#include <sys/stat.h>
#include <sys/types.h>

#include <unistd.h>
#include <fcntl.h>

#include <errno.h>
#include <string.h>

extern int errno;

/*
 * Prints the usage information for the program to stderr and exits
 */
void usage(char *progname) {
    fprintf(stderr, "Error. Usage %s <filepath> <start-address> <end-address>\n", progname);
    exit(1);
}

/*
 * Prints the reason of termination and terminates execution
 */
void die(char *reason) {
    fprintf(stderr, "Error. Reason: %s\n", reason);
    exit(1);
}

/*
 * Prints a byte
 */
void printbyte(unsigned char *b) {
    printf("%02x", (*b)&0xff);
}

/*
 * Prints a word (4-bytes) starting at address w
 */
void printword(unsigned char *w) {
    unsigned char *x = w; 

    printbyte(x + 1);
    printbyte(x);
    printbyte(x + 3);
    printbyte(x + 2);

    printf(" ");
}

/*
 * Returns the file size (in bytes) using stat() system call, returns
 * 0 on error.
 */
int getfilesize(char *path) {
    struct stat sbuffer;
    int filesize = 0;

    filesize = stat(path, &sbuffer);
    if (filesize == -1) {
        return 0;
    } else {
        return sbuffer.st_size;
    }
}

/*
 * Loads array mem[] with the contents of executable file
 * using system call open(), read(), close(), etc., returns 1
 * on success, 0 otherwise.
 */
int loadimage(char *path, unsigned char *mem, int size) {
    int fdin, retval;
    unsigned char buf[2048];
    int nread;

    fdin = open(path, O_RDONLY);

    if(fdin == -1) {
        die(strerror( errno ));
    }

    int count = 0;
   
    nread = read(fdin, buf, 2048);
    for(; count < nread; count++) {
        mem[count] = buf[count]; 
    }

    if(nread == -1) {
        die(strerror( errno ));
     }

    retval = close(fdin);
    if(retval == -1) {
        die(strerror( errno ));
    }

    return 1;
}

/*
 * Dumps the contents of array memm[] in required format starting at word
 * address first to word address last. If first and last addresses are
 * outside the range (0..size), dump the entire contents of array.
 */
void mdump(unsigned char *mem, int size, int first, int last) {
    if(first < 0) first = 0;
    if(last > size) last = size;

    printf("%-8s  %-35s  %-16s\n", "Address","Words in Hexadecimal", "ASCII characters");
    printf("--------  -----------------------------------  ----------------\n");
    for(; first < last; first+= 16) {
        printf("%08x    ", first);
        for(int i = 0; i < 4; i++) {
            printword( mem + (i * 4) );
        }
        
        printf(" ");

        for(int i = 0; i < 16; i++) {
            char value = *(mem + i);

            if(value > 0x7e || value < 0x20) {
                printf(".");
            }
            else { 
                printf("%c", *(mem + i) );
            }
        }

        printf("\n");
        mem += 16;
    }

}

/*
 * Entry point of the program, checks for command line
 * errors and then executes the program
 */
int main( int argc, char *argv[] ) {

    if (argc != 4) {
        usage(argv[0]);
    }

    char *endptra = NULL, *endptrb = NULL;
    int startaddress = 0, endaddress = 0;
    char *filepath = argv[1];

    startaddress = strtol(argv[2], &endptra, 16);
    endaddress = strtol(argv[3], &endptrb, 16);

    if(*endptra != '\0') {
        printf("%d\n", startaddress);

        die("Start address is invalid\n");
        exit(1);
    }

    if(*endptrb != '\0') {
        printf("%d\n", endaddress);
        die("End address is invalid\n");
        exit(1);
    }

    int filesize = getfilesize(filepath);

    unsigned char *mem = malloc( filesize );


    loadimage(filepath, mem, filesize);
    mdump(mem, filesize, startaddress, endaddress);

    exit(0);
}
