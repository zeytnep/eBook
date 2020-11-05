/**
 * zerdogru@uoguelph.ca
 */
#include "text.h"
/*
* Reads thext from the file, splits each line into words, allocates a string for each word.
* Stores the address of the string in the data pointer 
* Assemble the words in the original order into a linked list
* RETURNS a pointer to the first node in the linked list
*/
struct node_struct *txt2words(FILE *fp) {
    char filename[] = "poem.txt";
    int length = 0; //to store the length of the array

    //loading file into data structure 
    fp = fopen(filename, "r");

    if (!fp) {
        fprintf(stderr, "Can't open %s for reading\n", filename);
        return NULL;
    }
    int wordLength = 256;
    //pre allocate the chunk of memory that I'm going to use start filling the array
    char **words = (char **)malloc(wordLength* sizeof(char *));

    //using fgets to read the file into a buffer up to 256 from our file
    char buf[256];
    int i = 0;
    //fgets reads until it finds a \n character of EOF
    while(fgets(buf,256, fp)) {
        //check if array is full. If so extend it 
        if (i == wordLength) {

            wordLength += 256;
            char **newWords = realloc(words, wordLength *sizeof(char*));

            if (!newWords) { //if the value is null I say we cannot realloc, so I exit
                fprintf(stderr, "Can't realloc\n");
                exit(1);
            }
            //assign that new address to new address
            words = newWords;
        }

        buf[strlen(buf)-1] = '\0'; //replacing \n with \0
        char *token;
        token = strtok(buf, " "); //get the first word
        //walk through other words
        while (token != NULL) {
            int tokenLENGTH = strlen(token);
            //alllocate memory just as big as the word's size
            char * word=(char*)malloc(tokenLENGTH * sizeof(char));
            strcpy(word, token); //copy the word from token to word char
            words[i] = word; //put word in words 2D array
            token = strtok(NULL, " ");
            i++;
            if (i == wordLength) {
                wordLength += 256;
                char **newWords = realloc(words, wordLength *sizeof(char*));

                if (!newWords) { //if the value is null we say we cannot realloc and exit
                    fprintf(stderr, "Can't realloc\n");
                    exit(1);
                }
                //we assign that new address to new address
                words = newWords;
            }
        }
    }
    length = i; //set the length of the array of char pointers

    if (!words) {
        fprintf(stderr, "Can't build data structure ");
    }
    //build a linked list and store the address of the string in the data pointer
    struct node_struct* head = NULL; //pointer to head

    for (int i = 0; i< length; i++) {
        int position = i + 1;
        //Getting a block of memory for a new node
        struct node_struct* temp1 = (struct node_struct*)malloc(sizeof(struct node_struct)); 
        temp1 -> data = words[i];
        temp1 -> next = NULL;

        //i == 0 position 1
        if (position == 1) {
            temp1 -> next = head;
            head = temp1;
        } 
        else {
            struct node_struct* temp2 = head;
            int m;
            for (m = 0; m < position-2; m++) {
                temp2 = temp2 -> next;
            }
            temp1 -> next = temp2 -> next;
            temp2 -> next = temp1;
        }     
    }

    return head;
}

/* Returns the number of nodes in the given list */
int length(struct node_struct *list) {
    struct node_struct *temp = list;
    int length = 0;
    while (temp != NULL) {
        length++;
        temp = temp -> next;
    }
    return length;
}
/*
* Prints out the strings pointed by the "data" pointers
*/
void ftext( FILE *fp, struct node_struct *list ) {
    struct node_struct* current = list;
    int indicator = 0;
    int counter = 0;
    char word[80];
    while (current != NULL) {
        strcpy(word, current -> data);
        counter += strlen(word);
        if ((counter == 80) || (counter > 79)) {
            printf("\n");
            counter =0;
            counter += strlen(word);
        } else {
            if (indicator ==1) {
                printf(" ");
            }
        }
        printf("%s", current -> data);
        indicator =1;
        counter++;
        current = current -> next;
    }
    printf("\n");
}
/* Retruns 0 if the comparison result is successful */
int compare( struct node_struct *temp, void *target) {
        if (strcmp(temp -> data, target) == 0) {
            return 0;
        } else {
            return 1;
        }
}
/*
* This function will construct a copy of a list 
* The copy of the list will begin copying at the node given by “start” 
* and finish just prior to “end” or when it reaches a NULL pointer.
*/
struct node_struct *copy( struct node_struct *start, struct node_struct *end ) {
    struct node_struct *start2 = NULL, *prev =NULL;

    while ((start != NULL) || (start != end)) {

        struct node_struct *temp = (struct node_struct*)malloc(sizeof(struct node_struct)); 
        temp -> data = start -> data;
        temp -> next = NULL;

        if (start2 == NULL) {
            start2 = temp;
            prev = temp;
        }
        else {
            prev -> next = temp;
            prev = temp;
        }
        start = start -> next; 
    }
    return start2;
}
/*
* This function will traverse the linked list given by “list”, inspecting each node in turn
* If the “compar” function returns success, then th enode will considered found
* Construct a second list for each word node that was found, in the second list data contain a pointer to the found node
* Return the list of nodes that are found 
*/
struct node_struct *search(struct node_struct *list, char *target, int (*compar)(struct node_struct*temp, void *)) {
    struct node_struct *tempHead1 = list; //temp pointer to the head pointer
    if (tempHead1 == NULL ) {
        printf("The list is empty");
        exit(1);
    }
    struct node_struct *head2 = NULL;
    int position = 1;
    while (tempHead1 != NULL) {
        int success = compar(tempHead1, target); //if compar returns 0 we consider that as target has found
        // Construct a second list for each word node that was found, in the second list data contain a pointer to the found node
        if (success == 0) {
            struct node_struct* temp1 = (struct node_struct*)malloc(sizeof(struct node_struct)); 
            temp1 -> data = tempHead1;
            temp1 -> next = NULL;

            if (position == 1) {
                temp1 -> next = head2;
                head2 = temp1; //changed this
                position ++;
            } 
            else if (position != 1) {
                struct node_struct* temp2 = head2;
                int i;
                for(int i =0; i< position -2; i++) {
                    temp2 = temp2 -> next;
                }
                temp1 -> next = temp2 -> next;
                temp2 -> next = temp1;
                position++;
            }
        }
        tempHead1 = tempHead1 -> next;
    }
    return head2;
}
/* This function will remove repeated entries from a list. */
void remove_repeats (struct node_struct *list, int (*compar)(struct node_struct* , void *)) {
    /* Pointer to traverse the linked list */
    struct node_struct *current = list; 
    /* Pointer to store the next pointer of a node to be deleted*/
    struct node_struct *next;
    if (current == NULL) 
        return;

    while (current -> next != NULL) {
        int compare = compar(current, current->next->data); /* Compare current node with next node */
        if (compare == 0) { // If repeat has found
            next = current->next->next; 
            free(current->next);
            current->next = next;
        }
        else {
            current = current -> next;
        }
    }
}
/*
* This function frees the nodes in the list given by “list”. If free_data is non-zero, it will also free the data pointed to by “data”.
*/
void free_list( struct node_struct *list, int free_data) {
    struct node_struct *temp;
    while (list != NULL) {
        temp = list;
        list = list -> next;
        free(temp);
    }
    if (free_data!= 0) {
        struct node_struct *temp2;
        while (list != NULL) {
            temp2 = list;
            char *currentWord = strdup(temp2 -> data);
            free(currentWord);
            list = list -> next;
        }
    }
    list = NULL;
    printf("freed the list");
}