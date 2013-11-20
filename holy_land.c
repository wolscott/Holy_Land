//HOLY LAND, an interactive story/text based adventure game
//by Scott Wolcott
//don't expect much, it's shitty

#include <stdio.h>

#define MAX_OPTIONS 5 //max number of options at a location
#define LOC_SOURCE "locations.txt"
#define BODY_LEN 1000
#define MAX_OPT_BODY 1000
#define MAX_LOCATIONS 10

typedef struct {
	int opt_num;
	int go_to;
	char opt_body[MAX_OPT_BODY];
} option;

typedef struct {
	int year;
	int loc_id;
	char body[BODY_LEN];
	option opts[MAX_OPTIONS]; //5 by default
} location;

typedef struct {
	int loc_id;
	long offset;
}loc_map;

//prototypes
location get_location( FILE* locations, long address );
void print_location( location );
void print_options( option[MAX_OPTIONS] );
void get_options( FILE* locations, option opts[MAX_OPTIONS] );
void init_loc_map( FILE* locations, loc_map map[MAX_LOCATIONS] );
int parse_input( int input, option opts[MAX_OPTIONS] );
long get_address( int go_to, loc_map map[MAX_LOCATIONS] );
void print_loc_map( loc_map map[MAX_LOCATIONS] );

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
	location current_location;// = get_location( locations, map[0].offset );
	while( alive == 1 ){
		current_location = get_location( locations, address );
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

location get_location( FILE* locations, long offset ){
	location new_location;
	//printf("%p", locations );
	fseek( locations, offset, SEEK_SET );
/*	if( fgetc( locations ) != '@' ){
		puts("this is not the start of a location");
	}*/
	fscanf( locations, "%d%d", &new_location.loc_id, &new_location.year );
	int c;
	//char temp[100];
	char* tempptr = new_location.body;
	while( (c = fgetc( locations ) ) != '#' ){
		*(tempptr++) = c;
	}
	*tempptr = '\0';
	//new_location.body = temp;
	get_options( locations, new_location.opts );
	//printf("%p", locations );
	return new_location;
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
	print_options( ltp.opts );
}

void print_options( option opts[MAX_OPTIONS] ){
	int i;
	for( i = 0; i < MAX_OPTIONS && opts[i].opt_num != 0; i++ ){
		printf( "%d\t%s\n", opts[i].opt_num, opts[i].opt_body );
	}
}
