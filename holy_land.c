//HOLY LAND, an interactive story/text based adventure game
//by Scott Wolcott
//don't expect much, it's shitty

#include <stdio.h>
#include <string.h>

#define MAX_OPTIONS 5 //max number of options at a location
#define LOC_SOURCE "locations.txt"
#define BODY_LEN 1000
#define MAX_OPT_BODY 1000
#define MAX_LOCATIONS 10
#define KEY_LEN 10
#define MAX_EFFECTS 3
#define EXP_NUM 10

/*the 'option' structure is used to for storing and displaying menu options
 */
typedef struct {
	int opt_num; //number entered to select the option
	int go_to; //location id that this option will return
	char opt_body[MAX_OPT_BODY]; //description of this option
} option;

/* the 'influence' structure is used to associate keywords with bonuses to 
 * the status associated with those keywords
 */
typedef struct {
	char keyword[KEY_LEN]; //this keyword will be increment
	int increase; //by this much
} influence;

/* the 'location' structure is the core of this game engine. It is used to store
 * information from the locations.txt data file containing the game's story, and 
 * the possible gameplay options
 */
typedef struct {
	int year; //the year this location takes place
	int loc_id; //unique location id, used for moving between locations
	influence effects[MAX_EFFECTS]; //effects from traveling to this location
	char body[BODY_LEN];// description of this location
	option opts[MAX_OPTIONS]; //options that can be selected at this location
} location;

/* the loc_map structure is used to associate each unique loc_id with a position
 * in the file. an array of loc_map structs is generated at the begginning 
 * of main to allow the program to jump to parts of the file as needed
 */
typedef struct {
	int loc_id;
	long offset;
}loc_map;

//prototypes
location get_location( FILE* locations, long address, loc_map map[MAX_LOCATIONS], influence exp[EXP_NUM] );
void print_location( location );
void print_options( option[MAX_OPTIONS] );
void get_options( FILE* locations, option opts[MAX_OPTIONS] );
void init_loc_map( FILE* locations, loc_map map[MAX_LOCATIONS] );
int parse_input( int input, option opts[MAX_OPTIONS] );
long get_address( int go_to, loc_map map[MAX_LOCATIONS] );
void print_loc_map( loc_map map[MAX_LOCATIONS] );
location reset_location( void );
void print_experiences( influence experiences[EXP_NUM] );
void update_experiences( influence experiences[EXP_NUM], influence effects[MAX_EFFECTS] );
void init_exp( influence exp[EXP_NUM] );
int test_exp( influence test, influence exp[EXP_NUM] );

//being main
int main(){
	puts( "Welcome to Holy Land" );
	FILE* locations = fopen( LOC_SOURCE, "r" );
	loc_map map[MAX_LOCATIONS];
	init_loc_map( locations, map );
	print_loc_map( map );
	rewind( locations );
	long address = map[0].offset; // get_address( go_to, map );
	int input, go_to;
	int alive = 1;
	influence experiences[EXP_NUM];
	init_exp( experiences );
	location current_location;// = get_location( locations, map[0].offset );
	while( alive == 1 ){
		current_location = reset_location();
		current_location = get_location( locations, address, map, experiences );
		update_experiences( experiences, current_location.effects );
		print_experiences( experiences );
		print_location( current_location );
		printf( "What will you do?: ");
		scanf( "%d", &input );
		go_to = parse_input( input, current_location.opts );
		while ( go_to == 0 ){
			printf( "Not a valid input, please select from the above options: ");
			scanf( "%d", &input );
			go_to = parse_input( input, current_location.opts );
		}
		address = get_address( go_to, map );
	}
//	printf("%p", locations );
	fclose( locations );
	locations = fopen( LOC_SOURCE, "r" );
	print_location( current_location );
}

int test_exp( influence test, influence exp[EXP_NUM] ){
	int i;
	for( i = 0; i < EXP_NUM; i++ ){
		if( strcmp( test.keyword, exp[i].keyword ) == 0 ){
			if( test.increase <= exp[i].increase ){
				return 1;
			}
		}
	}
	return 0;
}

void init_exp( influence exp[EXP_NUM] ){
	int i;
	for( i = 0; i < EXP_NUM; i++ ){
		exp[i].keyword[0] = '?';
	}
}

void update_experiences( influence exp[EXP_NUM], influence effects[MAX_EFFECTS] ){
	int i, j;
	for( i = 0; i < MAX_EFFECTS; i++ ){
		for( j = 0; j < EXP_NUM; j++ ){			 
			if( exp[j].keyword[0] == '?' ){
				strcpy( exp[j].keyword, effects[i].keyword );
			}
			if( strcmp( exp[j].keyword, effects[i].keyword ) == 0 ){
				exp[j].increase += effects[i].increase;
				break;
			}
		}
		
	}
}
void print_experiences( influence exp[EXP_NUM] ){
	int i;
	for( i = 0; i < EXP_NUM; i++ ){
		printf( "%s - %d\n", exp[i].keyword, exp[i].increase );
	}
}

location reset_location(){
	location blank;
	blank.loc_id = 0;
	strcpy( blank.body, "" );
	int i, j;
	for( i = 0; i < EXP_NUM; i++ ){
		for( j = 0; j < KEY_LEN; j++ ){
			blank.effects[i].keyword[j] = '?';
		}
		blank.effects[i].increase = 0;
	}
	return blank;
}

void print_loc_map( loc_map map[MAX_LOCATIONS] ){
	int i;
	for( i = 0; i < MAX_LOCATIONS; i++ ){
		printf("location %d is at %ld\n", map[i].loc_id, map[i].offset );
	}
}

long get_address( int go_to, loc_map map[MAX_LOCATIONS] ){
	int i;
	for( i = 0; i < MAX_LOCATIONS; i++ ){
		if( go_to == map[i].loc_id ){
			return map[i].offset;
		}
	}
}

int parse_input( int input, option opts[MAX_OPTIONS] ){
	int i;
	for( i = 0; i < MAX_OPTIONS && opts[i].opt_num != 0; i++ ){
		if( input == opts[i].opt_num ){
			return opts[i].go_to;
		}
	}
	return 0;
}

void init_loc_map( FILE* locations, loc_map map[MAX_LOCATIONS] ){
	int i=0,c;
	while( (c = fgetc( locations )) != EOF ){
		if( c == '@' ){
			map[i].offset = ftell( locations );
			fscanf( locations, "%d", &map[i].loc_id );
			i++;
		}
	}
}

location get_location( FILE* locations, long offset, loc_map map[MAX_LOCATIONS], influence exp[EXP_NUM] ){
	location new_l;
	fseek( locations, offset, SEEK_SET ); //seek to the start of the desired location
	fscanf( locations, "%d%d", &new_l.loc_id, &new_l.year );
	int c, effect = 0;
	char* tempptr = new_l.body;
	//char* token;
	while( (c = fgetc( locations ) ) != '#' ){
		//if( c == '[' ) { //start effects block
		//	while( (c = fgetc( locations ) ) != ']' ){
		if( c == '^' && effect < MAX_EFFECTS ){
			fscanf( locations, "%s%d", new_l.effects[effect].keyword, &new_l.effects[effect].increase );
			printf( "\neffect dectected - keyword: %s, value: %d\n", new_l.effects[effect].keyword, new_l.effects[effect].increase );
			effect++;
		} else if( c == '?' ){ //conditional redirect
			influence test;
			int new_go_to;
			fscanf( locations, "%s%d%d", test.keyword, &test.increase, &new_go_to );
			if( test_exp( test, exp ) ){
				int address = get_address( new_go_to, map );
				return get_location( locations, address, map, exp );
			} 
		}
		if( c != '^' && c != '?' ) {
			*(tempptr++) = c;
		}
	}
	*tempptr = '\0';
	get_options( locations, new_l.opts );
	return new_l;
}

void get_options( FILE* locations, option opts[MAX_OPTIONS] ){
	int i, j, c;
	char* bodyptr;
	for( i = 0; i < MAX_OPTIONS; i++ ){
		bodyptr = opts[i].opt_body;
		fscanf( locations, "%d%d", &opts[i].opt_num, &opts[i].go_to );
		if( opts[i].opt_num == 0){
			break;
		}
		for( j = 0; j < MAX_OPT_BODY && (c = fgetc( locations )) != '#'; j++ ){
			*(bodyptr++) = c;
		}
		*bodyptr = '\0';
	}
}
		
void print_location( location ltp ){
	printf( "The year is %d, you are at location %d:%s\n", ltp.year, ltp.loc_id, ltp.body );
	printf( "first keyword: %s, increment: %d\n", ltp.effects[0].keyword, ltp.effects[0].increase );
	print_options( ltp.opts );
}

void print_options( option opts[MAX_OPTIONS] ){
	int i;
	for( i = 0; i < MAX_OPTIONS && opts[i].opt_num != 0; i++ ){
		printf( "%d\t%s\n", opts[i].opt_num, opts[i].opt_body );
	}
}
