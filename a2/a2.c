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
 *            arbitrary (exclusive) range. Exclusive in this case meaning that it contains all values
 *            from 0 - (range - 1). If RANGE is set to 9999, then our range of numbers is 0-9998.
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
 *                                                  - freeNode()
 *                                                  - deleteFromList()
 *                                                  - getNode()
 *                                                  - printList()
 *                                                  - insertInList()
 *                                                  - freeList()
 *
 *  Errors handled: The program will exit if the user either doesn't provide an argument, or specifies the event count
 *                  as less than or equal to 0 (event count must be positive). By not allowing 0 as an input, we also
 *                  prevent alphabetical characters from being passed as input (as this makes the built in atoi function
 *                  return 0).
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


void usage(char *progname) {
    fprintf(stderr, "Error. Usage: %s <event-count>\n", progname); 
    exit(1);
}

void search(Node *list, Node **current, Node **previous, int x) {
    *current = list;
    *previous = NULL;
    while(*current) {
        if( (*current)->value == x ) return;
        *previous = *current;
        *current = (*current)->next;
    }
}

void freeNode(Node **pointer) {
    if(*pointer) free(*pointer);
    *pointer = NULL;
}

int deleteFromList(Node **list, Node *predecessor) {
    Node **currentNode = list;

    if(predecessor == NULL) {
        Node *tempNode = *list;
        *currentNode = (*currentNode)->next;


        freeNode(&tempNode);

        return 1;
    }
    
    while(*currentNode) {

        if( (*currentNode)->next == NULL ) {
            return 0;
        }

        if( (*currentNode)->value == predecessor->value ) {         
            
            Node *tempNode = (*currentNode)->next;
            
            /* Set the next node of the current node to the node AFTER the node we want to skip */
            (*currentNode)->next = predecessor->next->next;


            freeNode(&tempNode);

            return 1;
        }

        *currentNode = (*currentNode)->next;

    } 

    return 0;
}

void getNode(Node **pointer) {
    *pointer = malloc(sizeof(Node));
}

void printList(Node *list) {
    Node *current = list;

    while(current != NULL) {
        printf("%d ", current->value);
        current = current->next;
    }

    printf("\n\n");
}

/*
 * 
 */
int insertInList(Node **list, int x) {

    //if(*list != NULL) {
    //    search(*list, &searchNode, &previousNode, x);
    //
    //    if(searchNode != NULL) {
    //        deleteFromList(list, previousNode);
    //        return 1;
    //    }
    //}

    Node *newNode;

    /* Assign memory to new node */
    getNode(&newNode);
    
    /* Set value of node */
    newNode->value = x;
    newNode->next = *list;
    *list = newNode;

    return 1;

}

/* 
 * Generates and returns a random number 
 * between 0 and the defined RANGE variable.
 */
int nextNum() {
    return (random() % RANGE);
}

/*
 * Iterates through each node in the list, and calls
 * the freeNode() function to free each node.
 *
 * This function essentially deletes the passed in
 * list.
 */
void freeList(Node **list) {

    Node *currentNode = *list;

    while(currentNode != NULL) {
        Node *tempNode = currentNode;
        
        currentNode = currentNode->next;

        freeNode(&tempNode);
    }

}

int main(int argc, char *argv[]) {
    Node *levens, *lodds, *previous, *current;
    int x;


    /* Create seed for random number generator */
    srand(1);

    /* Verify that the user input is accurage*/
    if(argc == 1) {
        usage(argv[0]);
    }

    int events = atoi(argv[1]);

    if(events <= 0) {
        fprintf(stderr, "Error. Please enter an event count greater than 0\n");
        exit(1);
    }

    /* Begin generating numbers */
    for(int i = 0; i < events; i++) {
        x = nextNum();
            
        Node** selectedList;

        if (x & 1) {
            selectedList = &lodds;
        } else {
            selectedList = &levens;
        }
    
        /* Check if the list is empty before searching */
        if (*selectedList != NULL) {
            search(*selectedList, &current, &previous, x);
            
            /* 
             * If current doesn't exist, then delete the item from the list,
             * otherwise insert it.
             */
            if(current != NULL) {
                deleteFromList(selectedList, previous);
            } else {
                insertInList(selectedList, x);
            }
        } else {
            insertInList(selectedList, x);
        }

        /* Set current back to null */
        current = NULL;
    }
  

    /* Print and then delete each list */
    printf("Odds: ");
    printList(lodds);

    printf("Evens: ");
    printList(levens);

    freeList(&lodds);
    freeList(&levens);


    exit(0);
}
