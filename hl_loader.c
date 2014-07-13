//new algorithms

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "holy_land.h" //this head is required for everythhing
//#include "holy_land.c" //i need some functions from this broken ass code

/*
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
location* get_loc_from_name( location** loc_array, int start, int end, char* name );
void free_list( loc_mapper* );
void append_pair( l_pair**, l_pair* );
void append_option( option**, option* );
void append_condred( l_cond**, l_cond* );
void free_locs( location**, int num );
void free_location( location* );
void free_pair_list( l_pair* );
void free_option_list( option* );
void free_condred_list( l_cond* );
void find_targets( location** loc_array, int len );
void set_option_targets( option** head, location** loc_array, int len );
void set_condred_targets( l_cond** head, location** loc_array, int len );
void display_location( location* loc );

//parse family of functions
void get_effect( FILE*, location* );
void get_body( FILE*, location* );
void get_condred( FILE*, location* );
void get_option( FILE*, location* );
void get_disp_name( FILE*, location* );
*/

/* load_story **
 this function is called from hl_frontend.c
 this function calls all the functions required to parse the 
 story file and returns a pointer to the resulting locations array
*/
location** load_story( int* num_locs ){ // global mark 'L' set here in vim
	FILE* l_file = fopen( LOC_SOURCE, "r" );
	char c = 'a';
	loc_mapper* head = NULL;
	*num_locs = 0;
	while( c != EOF ){
		c = fgetc( l_file );
		if( c == '@' ){
			(*num_locs)++;
			location* new_loc = parse_loc( l_file );	
			//printf( "New Location: \n" ); //debugging
			//print_loc( new_loc ); //debugging
			head = insert( head, new_loc );
		}
	}
	fclose( l_file );
//	print_all( head );
	location** loc_map_array = malloc( sizeof(location*) * *num_locs);
	build_loc_map_array( loc_map_array, head, *num_locs );
	find_targets( loc_map_array, *num_locs );
	print_array( loc_map_array, *num_locs );
	free_list( head );
	//printf( "sofarsogood" );
	return loc_map_array;
} //end load_story

void free_loaded( location** loc_map_array, int num_locs ){
	free_locs( loc_map_array, num_locs );
	free( loc_map_array );
} //end free_loaded

/* print_array **
 this function prints locs from the loc_map_array
*/
void print_array( location** array, int num ){
	int i;
	for( i = 0; i < num; i++ ){
		//print_loc( array[i] );
		display_location( array[i] );
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

/* get_fresh_loc **
 this function allocations memory for a new location
 and initializes all values.
*/
location* get_fresh_loc(){
	location* new_loc = calloc( 1, sizeof( location ));
	new_loc->effects = malloc( sizeof( l_pair* ));
	*(new_loc->effects) = NULL;
	new_loc->options = malloc( sizeof( option* ));
	*(new_loc->options) = NULL;
	new_loc->condreds = malloc( sizeof( l_cond* ));
	*(new_loc->condreds) = NULL;
	new_loc->has_disp_name = 0; //init no display name
	strcpy( new_loc->disp_name, "no display name\0");
	new_loc->show_name = 0; //init value
	strcpy( new_loc->body, "no body\0");
	return new_loc;
} //end get_fresh_loc
/* parse_loc **
 this function is a placeholder. It only parses the name of the location
*/
location* parse_loc( FILE* l_file ){
	location* new_loc = get_fresh_loc();	
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
				case '>':
					parse = get_option;
					break;
				case 'b':
					parse = get_body;
					break;
				case 'q':
					return new_loc;
				case '?':
					parse = get_condred;
					break;
				case 'n':
					parse = get_disp_name;
					break;
				default:
					parse = NULL;
			}
			if( parse ){
				parse( l_file, new_loc );
			}
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

/* get_disp_name **
 THIS FUNCTION MUST BE VOID AND TAKE A FILE POINTER AND A LOCATION POINTER
 it is the target of a function pointer.
*/
void get_disp_name( FILE* l_file, location* loc ){
	loc->has_disp_name = 1;
	loc->show_name = 1; //testing only, to be removed
	char c = fgetc( l_file );
	char* pos = loc->disp_name;
	int i = 0;
	while( c != ']' && i++ < DISP_NAME_LEN ){
		*pos++ = c;
		c = fgetc( l_file );
	}
} //end get_disp_name

/* get_condred **
 THIS FUNCTION MUST BE VOID AND TAKE A FILE POINTER AND A LOCATION POINTER
 it is the target of a function pointer.
*/
void get_condred( FILE* l_file, location* loc ){
	l_cond* new_condred = malloc( sizeof( l_cond ));
	new_condred->next = NULL;
	/* scan in the keyword to check the value of,
		the value to compair to,
		and the name of the location to redirect to
	*/
	fscanf( l_file, "%s %s %d", new_condred->new_go, new_condred->keyword, &(new_condred->value) );
	append_condred( loc->condreds, new_condred );
} //end get_condred

/* append_condred
 this function appends a condred node
 to a linked list;
*/
void append_condred( l_cond** head, l_cond* new ){
	l_cond* current = *head;
	if( *head == NULL ){
		*head = new;
		return;
	}
	while( current->next != NULL ){
		current = current->next;
	}
	current->next = new;
} //end append_condred

/* get_option **
 THIS FUNCTION MUST BE VOID AND TAKE A FILE POINTER AND A LOCATION POINTER
 it is the target of a function pointer.
*/
void get_option( FILE* l_file, location* loc ){
	option* new_option = calloc( 1, sizeof( option ));
	new_option->next = NULL;
	fscanf( l_file, "%s", new_option->target_name );
	char c = fgetc( l_file );
	char* pos = new_option->body;
	int i = 0;
	while( c != ']' && i++ < MAX_OPT_BODY ){
		*pos++ = c;
		c = fgetc( l_file );
	}
	append_option( loc->options, new_option );
} //end get_option

/* append_option
 this function appends a pair (key : value) node
 to a linked list;
*/
void append_option( option** head, option* new ){
	option* current = *head;
	if( *head == NULL ){
		*head = new;
		return;
	}
	while( current->next != NULL ){
		current = current->next;
	}
	current->next = new;
} //end append_option

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
	printf( "\nName: %s Address: %p\n\tEffects:\n", loc->name, loc );
	l_pair* effect = *(loc->effects);
	while( effect != NULL ){
		printf( "\t%s %d\n", effect->keyword, effect->value );
		effect = effect->next;
	}
	printf( "\tConditional Redirects:\n" );
	l_cond* condred = *(loc->condreds);
	while( condred != NULL ){
		printf( "\t%s %d %s %p\n", condred->keyword, condred->value, condred->new_go, condred->new_go_ptr );
		condred = condred->next;
	}
	printf( "\tDescription: %s\n", loc->body );
	option* option = *(loc->options);
	int opt_num = 0;
	while( option != NULL ){
		printf( "\t%d\t%s %p \t%s\n", ++opt_num, option->target_name, option->target, option->body );
		option = option->next;
	}
}
/* print_all **
 this function prints all the names in the loc list
*/
void print_all( loc_mapper* head ){
	while( head ){
		print_loc( head->loc );
		head = head->next;
	}
} //end print_all

/* find_targets **
 this function goes through the location array and sets the location
 pointers for each option of each location
*/
void find_targets( location** loc_array, int len ){
	int i;
	for( i = 0; i < len; i++ ){
		set_option_targets( loc_array[i]->options, loc_array, len );
		set_condred_targets( loc_array[i]->condreds, loc_array, len );
	}
} //end find_targets

void set_option_targets( option** head, location** loc_array, int len ){
	option* current = *head;
	while( current != NULL ){
		current->target = get_loc_from_name( loc_array, 0, len, current->target_name );
		current = current->next;
	}
} //end set_option_targets

void set_condred_targets( l_cond** head, location** loc_array, int len ){
	l_cond* current = *head;
	while( current != NULL ){
		current->new_go_ptr = get_loc_from_name( loc_array, 0, len, current->new_go );
		current = current->next;
	}
} //end set_condred_targets

/* get_loc_from_name **
 this function takes the array of location pointers
 and the name of a location and returns a pointer to
 the location with that name. use binary search algorithm.
*/
location* get_loc_from_name( location** loc_array, int start, int end, char* name ){
	int mid = ( start + end ) / 2;
	int val = strcmp( loc_array[mid]->name, name );
	int len = end - start;
	//base case: the current location pointer points to a location with a matching name
	if( len == 0 ){ //did not find the location
		printf( "ERROR: BAD OPTION TARGET: LOCATION NAME \"%s\" NOT FOUND\n", name );
		return NULL;
	}
	if( val == 0 ){ //match
		return loc_array[mid];
	}
	if( val > 0 ){ //name occurs earlier
		return get_loc_from_name( loc_array, start, mid, name );
	}
	if( val < 0 ){ //name occurs later
		return get_loc_from_name( loc_array, mid+1, end, name );
	}
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
	if( *(loc->options) != NULL ){
		free_option_list( *(loc->options));
	}
	if( *(loc->condreds) != NULL ){
		free_condred_list( *(loc->condreds));
	}
	free( loc->options );
	free( loc->effects );
	free( loc->condreds );
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

/* free_option_list ** TO DO: make one function to free all types of linear linked lists
 this function frees a linked list of l_pair nodes
*/
void free_option_list( option* head ){
	option* temp;
	while( head ){
		temp = head;
		head = head->next;
		free( temp );
	}
} //end free_option_list

/* free_condred_list ** TO DO: make one function to free all types of linear linked lists
 this function frees a linked list of l_pair nodes
*/
void free_condred_list( l_cond* head ){
	l_cond* temp;
	while( head ){
		temp = head;
		head = head->next;
		free( temp );
	}
} //end free_option_list

/* display_location **
 this function is the function that will be used by the game
 to display locations to the player. This function should be
 moved out of the loader file
*/
void display_location( location* loc ){
	/*printf( "\nName: %s Address: %p\n\tEffects:\n", loc->name, loc );
	l_pair* effect = *(loc->effects);
	while( effect != NULL ){
		printf( "\t%s %d\n", effect->keyword, effect->value );
		effect = effect->next;
	}
	printf( "\tConditional Redirects:\n" );
	l_cond* condred = *(loc->condreds);
	while( condred != NULL ){
		printf( "\t%s %d %s %p\n", condred->keyword, condred->value, condred->new_go, condred->new_go_ptr );
		condred = condred->next;
	}*/
	if( loc->has_disp_name && loc->show_name ){
		printf( "\n%s\n", loc->disp_name );
	}
	printf( "\n%s\n", loc->body );
	option* option = *(loc->options);
	int opt_num = 0;
	while( option != NULL ){
		printf( "\t%d\t%s\n", ++opt_num, option->body );
		option = option->next;
	}
	
} //end display_location
