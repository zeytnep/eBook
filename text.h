/**
 * zerdogru@uoguelph.ca
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node_struct {
	void *data;
	struct node_struct *next;
};

int compar( struct node_struct *, void*); 
struct node_struct *txt2words( FILE *fp ); 
struct node_struct *search(struct node_struct *list, char *target, int (*compar)(struct node_struct*temp, void *)); 
struct node_struct *copy( struct node_struct *start, struct node_struct *end ); 
void ftext( FILE *fp, struct node_struct *list );  
void remove_repeats ( struct node_struct *list, int (*compar)(struct node_struct* , void *) );
int length( struct node_struct *list );  
void free_list( struct node_struct *list, int free_data); 

