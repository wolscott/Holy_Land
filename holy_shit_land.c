//new algorithms

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "holy_land.h" //this head is required for everythhing
//#include "holy_land.c" //i need some functions from this broken ass code


typedef struct loc_mapper_{
	location* loc;
	struct loc_mapper_* next;
}loc_mapper;

location* parse_loc( FILE* );
loc_mapper* add( loc_mapper*, location* );
loc_mapper* append( loc_mapper*, location* );
loc_mapper* insert( loc_mapper*, location* );
void print_loc( location* );
void print_all( loc_mapper* head );
void build_loc_map_array( location**, loc_mapper*, int );
void print_array( location**, int );
location* get_loc_from_name( location** loc_array, int len, char* name );
void free_list( loc_mapper* );
void append_pair( l_pair**, l_pair* );
void free_locs( location**, int num );
void free_location( location* );
void free_pair_list( l_pair* );

//parse family of functions
void get_effect( FILE*, location* );
void get_body( FILE*, location* );

int main(){
	FILE* l_file = fopen( LOC_SOURCE, "r" );
	char c = 'a';
	loc_mapper* head = NULL;
	int num_locs = 0;
	while( c != EOF ){
		c = fgetc( l_file );
		if( c == '@' ){
			num_locs++;
			location* new_loc = parse_loc( l_file );	
			head = insert( head, new_loc );
		}
	}
	print_all( head );
	location** loc_map_array = malloc( sizeof(location*) * num_locs);
	build_loc_map_array( loc_map_array, head, num_locs );
	print_array( loc_map_array, num_locs );
	free_locs( loc_map_array, num_locs );
	free_list( head );
} //end main

/* print_array **
 this function prints locs from the loc_map_array
*/
void print_array( location** array, int num ){
	int i;
	for( i = 0; i < num; i++ ){
		print_loc( array[i] );
	}
} //end print_array

/* build_loc_map_array **
 this function takes the sorted linked list of locations and
 makes an array of location pointers sorted alphabetically by
 location name
*/
void build_loc_map_array( location** array, loc_mapper* head, int num ){
	int i;
	for( i = 0; i < num; i++ ){
		array[i] = head->loc;
		head = head->next;
	}
} //end build_loc_map_array


/* parse_loc **
 this function is a placeholder. It only parses the name of the location
*/
location* parse_loc( FILE* l_file ){
	location* new_loc = malloc( sizeof( location ));
	new_loc->effects = malloc( sizeof( l_pair* ));
	*(new_loc->effects) = NULL;
	fscanf( l_file, "%s", new_loc->name );
	char c = 'a';
	char tag[10];
	void (*parse)( FILE*, location* ); //function pointer
	while( c ){
		c = fgetc( l_file );
		if( c == '[' ){ //the beginning of a tag
			c = fgetc( l_file );
			switch( c ){
				case '^':
					parse = get_effect;
					break;
				case 'b':
					parse = get_body;
					break;
				case 'q':
					return new_loc;
			}
			parse( l_file, new_loc );
		}	
	}
	return new_loc;
} //end parse_loc

/* get_effect **
 THIS FUNCTION MUST BE VOID AND TAKE A FILE POINTER AND A LOCATION POINTER
 it is the target of a function pointer.
*/
void get_effect( FILE* l_file, location* loc ){
	l_pair* new_effect = malloc( sizeof( l_pair ));
	new_effect->next = NULL;
	fscanf( l_file, "%s %d", new_effect->keyword, &(new_effect->value) );
	append_pair( loc->effects, new_effect );
} //end get_effect

/* get_body **
 THIS FUNCTION MUST BE VOID AND TAKE A FILE POINTER AND A LOCATION POINTER
 it is the target of a function pointer.
*/
void get_body( FILE* l_file, location* loc ){
	char c = fgetc( l_file );
	char* pos = loc->body;
	int i = 0;
	while( c != ']' && i++ < BODY_LEN ){
		*pos++ = c;
		c = fgetc( l_file );
	}
} //end get_body
/* append_pair
 this function appends a pair (key : value) node
 to a linked list;
*/
void append_pair( l_pair** head, l_pair* new ){
	l_pair* current = *head;
	if( *head == NULL ){
		*head = new;
		return;
	}
	while( current->next != NULL ){
		current = current->next;
	}
	current->next = new;
} //end append_pair

/* insert **
 this function inserts a location into the linked list 
 in alphabetical order and returns the head
*/
loc_mapper* insert( loc_mapper* head, location* new_loc ){	
	loc_mapper* new_node = malloc( sizeof( loc_mapper ));
	new_node->loc = new_loc;
	if( head == NULL ){ //the list is empty
		new_node->next = NULL;
		head = new_node; //the new node is now the head
		return head;
	} 
	if( strcmp( head->loc->name, new_loc->name ) > 0 ){
		new_node->next = head;
		head = new_node;
		return head;
	} else {
		loc_mapper* current = head;
		while( current->next != NULL ){
			if( strcmp( current->next->loc->name, new_loc->name) > 0 ){ //if the new one goes before the next one
				new_node->next = current->next;
				current->next = new_node;
				return head;
			}		
			current = current->next;
		}
		new_node->next = NULL;
		current->next = new_node;
		return head;
	}
	return head;
} //end insert
 
/* append **
 the append function take the head of a linked list
 of loc_mapper nodes, appends a new node with the 
 given location, and returns the head
*/
loc_mapper* append( loc_mapper* head, location* new_loc ){
	loc_mapper* new_node = malloc( sizeof( loc_mapper ));
	new_node->loc = new_loc;
	new_node->next = NULL;
	if( head == NULL ){ //the list is empty
		head = new_node; //the new node is now the head
	} else {
		loc_mapper* current = head;
		while( current->next != NULL ){
			current = current->next;
		}
		current->next = new_node;
	}
	return head;
} //end append

/* print_loc **
 this function prints the name of a location (placeholder)
*/
void print_loc( location* loc ){
	printf( "\nName: %s\n\tEffects:\n", loc->name );
	l_pair* effect = *(loc->effects);
	while( effect != NULL ){
		printf( "\t%s %d\n", effect->keyword, effect->value );
		effect = effect->next;
	}
	printf( "Description: %s\n", loc->body );
} //end print_loc

/* print_all **
 this function prints all the names in the loc list
*/
void print_all( loc_mapper* head ){
	while( head ){
		print_loc( head->loc );
		head = head->next;
	}
} //end print_all

/* get_loc_from_name **
 this function takes the array of location pointers
 and the name of a location and returns a pointer to
 the location with that name. use binary search algorithm.
*/
location* get_loc_from_name( location** loc_array, int len, char* name ){

} //end get_loc_from_name

/* free_list **
 this function frees the memory associated with
 a linked list of loc_mapper structs
*/
void free_list( loc_mapper* head ){
	loc_mapper* temp;
	while( head ){
		temp = head;
		head = head->next;
		free( temp );
	}
} //end free_list

/* free_locs **
 this this function calls free_location for each location
*/
void free_locs( location** loc_array, int len ){
	int i;
	for( i = 0; i < len; i++ ){
		free_location( loc_array[i] );
	}
}
/* free_location **
 this function frees all the of the memory associated with 
 a location structure
*/
void free_location( location* loc ){
	if( *(loc->effects) != NULL ){
		free_pair_list( *(loc->effects));
	}
	free( loc );
}

/* free_pair_list ** TO DO: make one function to free all types of linear linked lists
 this function frees a linked list of l_pair nodes
*/
void free_pair_list( l_pair* head ){
	l_pair* temp;
	while( head ){
		temp = head;
		head = head->next;
		free( temp );
	}
} //end free_pair_list
