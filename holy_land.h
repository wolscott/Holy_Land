//Moved header stuff here for easy access
//global mark 'H' set here in vim
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_OPTIONS 5 //max number of options at a location. BEING PHASED OUT, linked-list implementations allows unlimited options
#define LOC_SOURCE "holy_land_story.txt" //should add optional command line parameter for this
#define BODY_LEN 1000
#define MAX_OPT_BODY 1000
#define MAX_LOCATIONS 10 //phasing out, replaced with MAX_YEARS. NOT USED
#define KEY_LEN 10
#define MAX_EFFECTS 3 //number of exp effects that a location can have. PHASING OUT
#define MAX_CONDREDS 3 //number of conditional redirects that a location can have. PHASING OUT
#define EXP_NUM 5 // PHASING OUT
#define MAX_LOCS 20 //max locations per block
#define MAX_YEARS 5 //should be like a billion, but small for testing purposes. NOT USED
#define LOC_NAME_LEN 32 //max length of location name
#define DISP_NAME_LEN 32 //max length of display name
#define GLOB_CONDREDS 8 //number of global condreds. PHASING OUT
#define MAX_INPUT 32 //max characters read from user

/*the 'option' structure is used to for storing and displaying menu options
 */
typedef struct option_ {
	//int opt_num; //number entered to select the option
	char target_name[LOC_NAME_LEN]; //location id that this option will return
	struct location* target;
	char body[MAX_OPT_BODY]; //description of this option
	struct option_* next;
} option;

/* the 'pair' structure is used to associate keywords with bonuses to 
 * the status associated with those keywords
 */
typedef struct {
	char keyword[KEY_LEN]; //this keyword will be increment
	int value; //by this much
} pair;

/* the 'l_pair' structure is used to associate keywords with bonuses to 
 * the status associated with those keywords, but is for linked lists
 */
typedef struct l_pair_{
	char keyword[KEY_LEN]; //this keyword will be increment
	int value; //by this much
	struct l_pair_* next;
} l_pair;

/* the 'cond' structure is used to associate keywords with conditions to 
 * to be met and the new destination when those conditions are met
 */
typedef struct {
	char keyword[KEY_LEN]; //this keyword will be increment
	int value; //by this much
	char new_go[LOC_NAME_LEN];
	struct location* new_go_ptr;
} cond;

/* the 'l_cond' structure is used to associate keywords with conditions to 
 * to be met and the new destination when those conditions are met
 */
typedef struct l_cond_ {
	char keyword[KEY_LEN]; //check the value associated with this keyword
	int value; //trigger value
	char new_go[LOC_NAME_LEN]; //name of location for redirect
	struct location* new_go_ptr; //address of location for redirect
	struct l_cond_* next;
} l_cond;

/* the 'location' structure is the core of this game engine. It is used to store
 * information from the locations.txt data file containing the game's story, and 
 * the possible gameplay options
 */
typedef struct location_ {
	int block; //the block this location takes place
	int has_disp_name; //flag for existance of display name
	int show_name; //flag as to whether the display name should be shown
	char disp_name[DISP_NAME_LEN]; //name to be displayed
	int loc_id; //unique location id, used for moving between locations
	char name[LOC_NAME_LEN]; //replacing loc_id which is being phased out
	l_pair** effects; //new linked list implementation
//	pair effects[MAX_EFFECTS]; //effects from traveling to this location
//	cond condreds[MAX_CONDREDS];
	l_cond** condreds; //new linked list implementation
	char body[BODY_LEN];// description of this location
	//option opts[MAX_OPTIONS]; //options that can be selected at this location
	option** options; //new linked list implementation
} location;

/* the loc_map structure is used to associate each unique loc_id with a position
 * in the file. an array of loc_map structs is generated at the begginning 
 * of main to allow the program to jump to parts of the file as needed
 */
typedef struct loc_map_{
	int loc_id;
	char loc_name[LOC_NAME_LEN]; //replacing loc_id
	long offset;
}loc_map;

typedef struct loc_link_{
	char loc_name[LOC_NAME_LEN];
	location* go_to_ptr;
} loc_link;


typedef struct loc_mapper_{
	location* loc;
	struct loc_mapper_* next;
}loc_mapper;

//prototypes for hl_loader.c
location** load_story( int* );
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
location* get_fresh_loc( void );
int process_input( char* input, location** curloc );
void process_location( location** curloc, l_pair** status );
void do_effects( l_pair** effects, l_pair** player_e_head );
void handle_effect( l_pair* effect, l_pair** player_e_head );
location* do_condreds( l_cond** condreds, l_pair** player_e_head );

//parse family of functions
void get_effect( FILE*, location* );
void get_body( FILE*, location* );
void get_condred( FILE*, location* );
void get_option( FILE*, location* );
void get_disp_name( FILE*, location* );
// OLD prototypes
/*
//location get_location( FILE* locations, long address, loc_map map[MAX_LOCATIONS], pair exp[EXP_NUM] );
void get_location( FILE* l_file, location* );
void print_location( location* ltp );
void print_options( option[MAX_OPTIONS] );
void get_options( FILE* locations, option opts[MAX_OPTIONS] );
void init_loc_map( FILE* locations, loc_map map[MAX_LOCATIONS] );
location* parse_input( int input, option opts[MAX_OPTIONS] );
long get_address( int go_to, loc_map map[MAX_LOCATIONS] );
void print_loc_map( loc_map map[MAX_LOCATIONS] );
void reset_location( location* );
void print_experiences( pair experiences[EXP_NUM] );
void update_experiences( pair experiences[EXP_NUM], pair effects[MAX_EFFECTS] );
void init_exp( pair exp[EXP_NUM] );
int test_exp( pair test, pair exp[EXP_NUM] );
void make_block_map( FILE* locations, loc_map map[MAX_YEARS] );
void print_block_map( loc_map map[MAX_YEARS] );
void get_locs( FILE* l_file, long offset, location locs[MAX_LOCS] );
location* do_condreds( location*, cond globcons[GLOB_CONDREDS], pair experiences[EXP_NUM] );
void build_loc_links(location locs[MAX_LOCS], cond globconds[GLOB_CONDREDS], loc_link link_map[MAX_LOCS]);
void build_link_map(location locs[MAX_LOCS], loc_link link_map[MAX_LOCS]);
location* get_link( char loc_name[LOC_NAME_LEN], loc_link link_map[MAX_LOCS]);
location* load_save( FILE* savefile, pair exp[EXP_NUM], loc_link link_map[MAX_LOCS] );
int get_save_block( FILE* savefile );
void get_glob_condreds( FILE* l_file, cond condreds[GLOB_CONDREDS] );
*/
//end header file
