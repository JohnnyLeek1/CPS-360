/*
 *  Name: Johnny Leek                       Class: CPS 360
 *  Section: 22383332 (M/W/F 12PM)          Assignment: 05
 *  Due: October 26, 2020                   Started: October 25, 2020
 *  Credit: 15 Points
 *
 *  Problem: Write a program to produce a dump of a memory block in base-16 digits and in
 *           ASCII. The program will load an executable-image into memory (simulated by
 *           dynamically allocating an array of chars). It will produce a dump of memory 
 *           locations from the start-address to the end-address. The file path, start-address
 *           and end-address are specified via the command line. If the end-address is larger
 *           than the last memory location address, it will be adjusted to the last location
 *           address.
 *
 *           You cannot assume any user input. You must sanitize the input to ensure that the
 *           program does not break.
 *
 *  Solution: The solution to this program is quite cumbersome, but intuitive. After getting and
 *            sanitizing the user input, we calculate the filesize using the "stat" method call,
 *            and accessing the file size from the provided struct. From there, we dynamically 
 *            allocate a memory array using the provided file size as the arrays size.
 *
 *            Now we are ready to load the image into memory. We first open the file into a
 *            file descriptor titled 'fdin'. Then we keep track of both the bytes read and
 *            the bytes written to memory. Whilst the number of bytes written to memory is
 *            less than the file size, we continuously loop over the file. We load a buffer
 *            of 2048 bytes into a buffer, and write that buffer to memory. Then we tell the
 *            file descriptor to move forward by the number of bytes that were written 
 *            (hopefully 2048, but however many bytes were successfully written). After
 *            that we can close the file to free up that memory.
 *
 *            Then it is time to print the memory dump to stdout. We first set the 'first' and
 *            'last' pointers appropriately based on the specified start and end address. Then we
 *            print the headers, followed by the actual data. For each character in he memory array,
 *            we print it's hex value into stdout, followed by it's value in ASCII. If the value is
 *            > 0x7e or < 0x20, we print a '.' as characters in that range are unprintable. Otherwise,
 *            we print out the ASCII representation of the character.
 *
 *  Data Structures: We use a dynamically allocated char array.
 *
 *  Accessing functions for the data structure: The data structure is accessed in:
 *                                                                          - loadimage()
 *                                                                          - mdump()
 *
 *  Limitations: Obviously this program is limited by the amount of system memory, as it loads entire files
 *               into memory in order to perform operations on them.
 *
 *  Acknowledgements: Tyler Moses and I collaborated on parts of this program.
 */
#include <stdio.h>
#include <stdlib.h>

#include <sys/stat.h>
#include <sys/types.h>

#include <unistd.h>
#include <fcntl.h>

/* Have access to the 'errno' variable and 'strerror' function to diagnose errors  */
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
    int i = 3;
    for(; i >= 0; i--) {
        printbyte(x + i);
    }
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
    int nread, nwrote = 0, count = 0;

    fdin = open(path, O_RDONLY);

    if(fdin == -1) {
        die(strerror( errno ));
    }

    while(nwrote < size) {
        nread = read(fdin, buf, 2048);

        if(nread == -1) {
            die(strerror( errno ));
        }

        int bytesread = 0;
        for(; bytesread < nread; bytesread++) {
            mem[count] = buf[bytesread];
            count++;
        }

        if(nread == 0) break;
        
        nwrote += nread;
        lseek(fdin, nwrote, SEEK_CUR);
    }

    retval = close(fdin);
    if(retval == -1) {
        die(strerror( errno ));
    }

    return 1;
}

/*
 * Dumps the contents of array mem[] in required format starting at word
 * address first to word address last. If first and last addresses are
 * outside the range (0..size), dump the entire contents of array.
 */
void mdump(unsigned char *mem, int size, int first, int last) {
    int i;
    if(first < 0) first = 0;
    if(last > size) last = size;

    printf("%-8s  %-35s  %-16s\n", "Address","Words in Hexadecimal", "ASCII characters");
    printf("--------  -----------------------------------  ----------------\n");
    for(; first < last; first+= 16) {
        printf("%08x  ", first);
        for(i = 0; i < 4; i++) {
            printword( mem + (i * 4) );
        }
        
        printf(" ");

        for(i = 0; i < 16; i++) {
            char value = *(mem + i);
            
            if(value > 0x7e || value < 0x20) {
                printf(".");
            }
            else { 
                printf("%c", value);
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
