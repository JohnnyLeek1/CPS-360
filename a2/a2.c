/*
 *  Name: Johnny Leek                   Class: CPS 360
 *  Section: 22383332 (M/W/F 12PM)      Assignment: 02
 *  Due: September 14, 2020             Started: September 9, 2020
 *  Credit: 10 Points
 *
 *  Problem: Generate n random integers within a specified range. If the generated number is odd,
 *           place it at the front of a single linked list (named lodds) if it is not already in the list.
 *           It the number is already in the list, delete it from the list. A similar action is to be taken
 *           for even numbers with a list named levens. Store and free list nodes using the library functions
 *           malloc() and free().
 *
 *           Assume that any input will be given, so ensure to sanitize the user input.
 *           Do not use the modulus operator to determine if a number is odd or even, use
 *           bitwise operators.
 *
 *  Solution: We start by defining 4 list nodes (to contain different data).
 *              - levens: Even list
 *              - lodds: Odd list
 *              - previous: Previous node
 *              - current: Current node
 *
 *            We also have defined "RANGE" at the top of the program. This allows us to pick an 
 *            arbitrary (inclusive) range.
 *
 *            Then we check the user input to ensure that the user has provided the correct amount
 *            and syntax of arguments to the program.
 *
 *            We create the seed for the random number generator, and then count from 0 - (n-1) where n
 *            is the input that the user provided. We generate a random number each time, if the number
 *            & 1 (bitwise AND) resolves to true, then we know that the random number has a 1 in the binary
 *            place of 2^0, thereby being ODD. We pass this value to be inserted or deleted to/from the ODD list.
 *            We do the opposite is n & 1 resolves to false, as this means the number is EVEN.
 *
 *            To insert into the list, we first check if the list is empty or not before searching (there is no point
 *            searching an empty list). If we search the list, and find a node with the target value, we delete it
 *            from the list and return from the function. Otherwise (if either the list is empty OR the target value
 *            was not found in the list), we push the node to the beginning of the list, so that it becomes the HEAD
 *            node, and then we return from the program.
 *
 *            When the loop is complete, we iterate through each list one last time to print the contents of the list.
 *
 *            We then iterate through the lists and free each node in memory to prevent memory leaks.
 *
 *  Data Structures: Primarily we have 2 linked lists, which consist of "node" structs.
 *
 *  Accessing functions for the data structure: We access the list and modify the nodes in the following functions:
 *                                                  - search()
 *                                                  - freenode()
 *                                                  - deletefromlist()
 *                                                  - getnode()
 *                                                  - printlist()
 *                                                  - insertinlist()
 *                                                  - freelist()
 *
 *  Errors handled: The program will exit if the user either doesn't provide an argument, or specifies the event count
 *                  as less than or equal to 0 (event count must be positive). By not allowing 0 as an input, we also
 *                  prevent alphabetical characters from being passed as input (as this makes the built in atoi function
 *                  return 0).
 *
 *                  The program also exits if node deletion fails (for whatever reason). This is signified by the
 *                  deletefromlist() function returning a 0.
 *
 *  Limitations: This program won't work for 0 or negative inputs. It also has a range of 5 (though this can be changed
 *               by modifying the source and updating the RANGE definition).
 *
 *  Acknowledgements: Owen Smith and I bounced ideas off of each other for this assignment, and we helped each other debug
 *                    certain parts of each others programs
 *
 */
#include <stdio.h>
#include <stdlib.h>

#define RANGE 5
typedef struct node { int value; struct node *next; } Node;


/*
 * Frees the provided node from memory and
 * sets its reference to null
 */
void freenode(Node **pointer) {
    if(*pointer) free(*pointer);
    *pointer = NULL;
}

/*
 * Searches list for node containing x, if found then "current" node
 * is set to the found value, and "previous" is set to the node
 * before it. If the value is not found, then "current" is undefined
 * and "previous" is irrelevant
 */
void search(Node *list, Node **current, Node **previous, int x) {
    *current = list;
    *previous = NULL;

    while(*current) {
        if( (*current)->value == x ) return;
        *previous = *current;
        *current = (*current)->next;
    }
}

/*
 *  Deletes a node from the given list after the predecessor node.
 *  If the predecessor node is null, then we know that the node
 *  is the head node in the list, so we delete the head.
 *
 *  Otherwise, we delete the predecessors next node and set
 *  the list nodes next value to the deleted nodes successor
 *
 *  Returns an int of either 1 (success) or 0 (failure)
 */
int deletefromlist(Node **list, Node *predecessor) {
    if(predecessor == NULL) {
        Node *tempnode = *list;
        *list = (*list)->next;

        freenode(&tempnode);
        return 1;
    }

    if((*list)->value == predecessor->value) {
        Node *tempnode = predecessor->next;
        (*list)->next = tempnode->next;
        freenode(&tempnode);
        return 1;
    }
    
    return 0;
}

/*
 * Allocates memory for a new node
 */
void getnode(Node **pointer) {
    *pointer = malloc(sizeof(Node));
}

/*
 * Prints out the provided list in
 * a readable format.
 *
 * If the list is empty then print
 * "Empty"
 */
void printlist(Node *list) {
    Node *current = list;

    if(current == NULL) {
        printf("Empty");
    }

    while(current != NULL) {
        printf("%d ", current->value);
        current = current->next;
    }

    printf("\n\n");
}

/*
 * Inserts a node with value 'x', at the
 * head of the provided list
 */
int insertinlist(Node **list, int x) {
    Node *newnode;

    getnode(&newnode);
    
    newnode->value = x;
    newnode->next = *list;
    *list = newnode;

    return 1;

}


/*
 * Iterates through each node in the list, and calls
 * the freenode() function to free each node from memory.
 *
 * This function essentially deletes the passed in
 * list.
 */
void freelist(Node **list) {

    Node *currentnode = *list;

    while(currentnode != NULL) {
        Node *tempnode = currentnode;
        
        currentnode = currentnode->next;

        freenode(&tempnode);
    }

}

/* 
 * Generates and returns a random number 
 * between 0 and the defined RANGE variable.
 *
 * We add one to the "RANGE" variable to make
 * this range inclusive
 */
int nextnum() {
    return (random() % (RANGE + 1));
}

/*
 * Prints the usage instructions for the program to stderr and exits
 */
void usage(char *progname) {
    fprintf(stderr, "Error. Usage: %s <event-count>\n", progname); 
    exit(1);
}

/* Entry point of program */
int main(int argc, char *argv[]) {
    Node *levens, *lodds, *previous, *current;
    int x;


    srand(1);

    if(argc == 1) {
        usage(argv[0]);
    }

    int events = atoi(argv[1]);

    if(events <= 0) {
        fprintf(stderr, "Error. Please enter an event count greater than 0\n");
        exit(1);
    }

    for(int i = 0; i < events; i++) {
        x = nextnum();
            
        Node** selectedlist;

        if (x & 1) {
            selectedlist = &lodds;
        } else {
            selectedlist = &levens;
        }
    
        if (*selectedlist != NULL) {
            search(*selectedlist, &current, &previous, x);
            
            if(current != NULL) {
                deletefromlist(selectedlist, previous);
            } else {
                insertinlist(selectedlist, x);
            }
        } else {
            insertinlist(selectedlist, x);
        }

        current = NULL;
    }
  

    printf("Odd List: ");
    printlist(lodds);

    printf("Even List: ");
    printlist(levens);

    freelist(&lodds);
    freelist(&levens);


    exit(0);
}
