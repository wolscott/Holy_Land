//HOLY LAND, an interactive story/text based adventure game
//by Scott Wolcott
//don't expect much, it's shitty

#include <stdio.h>
#include <string.h>
#include <holy_land.h>

//begin main
int main(){
	puts( "Welcome to Holy Land" );
	/* Open file and make year_map (one time when program is run)
	 */
	FILE* l_file = fopen( LOC_SOURCE, "r" );
	loc_map year_map[MAX_YEARS];
	make_year_map( l_file, year_map );
	fclose( l_file );
	//create an array of location strucs
	long address; // get_address( go_to, map );
	location locs[MAX_LOCS];
	pair experiences[EXP_NUM];
	init_exp( experiences );
	location* current_location;
	int input;
	location* go_to;
	int alive = 1;
	int gameon = 1;
	int year = 0;
	int next_year;
	pair reset_time;
	strcpy( reset_time.keyword, "time" );
	reset_time.value = 0;
	cond timelimit[1];
	strcpy( timelimit[0].keyword, "time");
	timelimit[0].value = 17;
	loc_link link_map[MAX_LOCS];
	//timelimit[0].new_go = 1;
	while( gameon == 1){
		year++;
		//go_to = 1;
		go_to = &locs[1];
		address = year_map[year].offset;
		if( year > 1 && address == 0 ){
			gameon = 0;
			break;
		}
		l_file = fopen( LOC_SOURCE, "r" );
		get_locs( l_file, address, locs );
		fclose( l_file );
		build_link_map( locs, link_map );
		build_loc_links( locs, link_map );
		next_year = 0;
		if( !set_exp( experiences, reset_time )){
			puts( "time error");
		}
	//print_experiences( experiences );
		while( next_year == 0 ){
			//current_location = &locs[go_to];
			current_location = go_to;
			update_experiences( experiences, (*current_location).effects );
			current_location = do_condreds( current_location, experiences );
			if( go_to == 0 ){ 
				print_experiences( experiences );
				print_location( current_location );
				printf( "What will you do?: ");
				scanf( "%d", &input );
				go_to = parse_input( input, (*current_location).opts );
				while ( go_to == 0 ){
					printf( "Not a valid input, please select from the above options: ");
					scanf( "%d", &input );
					go_to = parse_input( input, (*current_location).opts );
				}
			}
			//next_year = do_condreds( experiences, timelimit, 1 );
			if( next_year != 0 ){
				printf( "time happened \n" );
			}
		}
	}
}

/* creates a list that associates location names with the location ptr address
 */
void build_link_map( location locs[MAX_LOCS], loc_link link_map[MAX_LOCS]){
	int i;
	for( i = 0; i < MAX_LOCS; i++ ){
		strcpy( link_map[i].loc_name, locs[i].loc_name );
		link_map[i].go_to_ptr = &(locs[i]);
	}
}

/* called from build_lock_links
 * returns the location pointer for the location corresponding to a given location name
 */
location* get_link( char loc_name[LOC_NAME_LEN], loc_link link_map[MAX_LOCS]){
	int i;
	for( i = 0; i < MAX_LOCS; i++ ){
		if( strcmp( link_map[i].loc_name, loc_name ) == 0 ){
			return link_map[i].go_to_ptr;
		}
	}
}

/* Goes through the list of locations and sets the location pointers for each option
 */
void build_loc_links(location locs[MAX_LOCS], loc_link link_map[MAX_LOCS]){
	int i, j;
	for( i = 0; i < MAX_LOCS; i++ ){
		for( j = 0; j < MAX_OPTIONS && locs[i].opts[j].opt_num != 0; j++ ){
			locs[i].opts[j].go_to_ptr = get_link( locs[i].opts[j].go_to, link_map);
		}	
	}
}//end build_loc_links( location locs[MAX_LOCS])

/* sets a value for a target attribute
 */
int set_exp( pair exp[EXP_NUM], pair target ){
	int i;
	for( i = 0; i < EXP_NUM; i++ ){
		if( strcmp( target.keyword, exp[i].keyword ) == 0 ){
			exp[i].value = target.value;
			return 1;
		}
	}
	return 0;
}

int test_exp( pair test, pair exp[EXP_NUM] ){
	int i;
	for( i = 0; i < EXP_NUM; i++ ){
		if( strcmp( test.keyword, exp[i].keyword ) == 0 ){
			if( test.value <= exp[i].value ){
				return 1;
			}
		}
	}
	return 0;
}

location* do_condreds( location* current_loc, pair exp[EXP_NUM] ){
	int i, j;
	for( i = 0; i < MAX_CONDREDS; i++ ){ //condreds must be the outer loop to be in the right order
		for( j = 0; j < EXP_NUM; j++ ){
			if( strcmp( (*current_loc).condreds[i].keyword, exp[j].keyword ) == 0 ){
				if( (*current_loc).condreds[i].value <= exp[j].value ){
					return (*current_loc).condreds[i].new_go_ptr;
				}
			}
		}
	}
	return current_loc;
}
/* old
location* do_condreds( pair exp[EXP_NUM], cond condreds[MAX_CONDREDS], int num_condreds ){
	int i, j;
	for( i = 0; i < num_condreds; i++ ){ //condreds must be the outer loop to be in the right order
		for( j = 0; j < EXP_NUM; j++ ){
			if( strcmp( condreds[i].keyword, exp[j].keyword ) == 0 ){
				if( condreds[i].value <= exp[j].value ){
					return condreds[i].new_go_ptr;
				}
			}
		}
	}
	return 0;
}*/

void init_exp( pair exp[EXP_NUM] ){
	int i, j;
	for( i = 0; i < EXP_NUM; i++ ){
		exp[i].value = 0;
		for( j = 0; j < KEY_LEN; j++ ){
			exp[i].keyword[j] = '?';
		}
	}
}

void update_experiences( pair exp[EXP_NUM], pair effects[MAX_EFFECTS] ){
	int i, j;
	for( i = 0; i < MAX_EFFECTS; i++ ){
		for( j = 0; j < EXP_NUM; j++ ){			 
			if( exp[j].keyword[0] == '?' && effects[i].keyword[0] != '\0' ){
				strcpy( exp[j].keyword, effects[i].keyword );
			}
			if( strcmp( exp[j].keyword, effects[i].keyword ) == 0 ){
				exp[j].value += effects[i].value;
				break;
			}
		}
		
	}
}
void print_experiences( pair exp[EXP_NUM] ){
	int i, printed = 0;
	printf( "\n" );
	for( i = 0; i < EXP_NUM; i++ ){
		if( exp[i].keyword[0] != '?' ){
			printf( "%s - %d\t", exp[i].keyword, exp[i].value );
			printed++;
			if( printed % 4 == 0 ){
				printf("\n");
			}
		}
	}
	if( printed % 4 != 0 ){
		printf("\n");
	}
}

void reset_location( location* blank ){
	//(*blank).loc_id = 0;
	int i, j;
	for( i = 0; i < BODY_LEN; i++ ){
		(*blank).body[i] = '\0';
	}
	for( i = 0; i < MAX_EFFECTS; i++ ){
		for( j = 0; j < KEY_LEN; j++ ){
			(*blank).effects[i].keyword[j] = '\0';
		}
		(*blank).effects[i].value = 0;
	}
	for( i = 0; i < MAX_CONDREDS; i++ ){
		for( j = 0; j < KEY_LEN; j++ ){
			(*blank).condreds[i].keyword[j] = '\0';
		}
		(*blank).condreds[i].value = 0;
	}
}

void print_loc_map( loc_map map[MAX_LOCATIONS] ){
	int i;
	for( i = 0; i < MAX_LOCATIONS; i++ ){
		printf("location %s is at %ld\n", map[i].loc_name, map[i].offset );
	}
}

/* no longer used
long get_address( int go_to, loc_map map[MAX_LOCATIONS] ){
	int i;
	for( i = 0; i < MAX_LOCATIONS; i++ ){
		if( go_to == map[i].loc_id ){
			return map[i].offset;
		}
	}
}*/

location* parse_input( int input, option opts[MAX_OPTIONS] ){
	int i;
	for( i = 0; i < MAX_OPTIONS && opts[i].opt_num != 0; i++ ){
		if( input == opts[i].opt_num ){
			//return opts[i].go_to;
			return opts[i].go_to_ptr;
		}
	}
	return 0;
}

void print_year_map( loc_map map[MAX_YEARS] ){
	int i;
	for( i = 0; i < MAX_YEARS; i++ ){
		printf("location %d is at %ld\n", map[i].loc_id, map[i].offset );
	}
}
void make_year_map( FILE* locations, loc_map map[MAX_YEARS] ){
	int i=1,c;
	while( (c = fgetc( locations )) != EOF ){
		if( c == '&' ){ // '&' is the sentinal char indicating the start of a year
			map[i].offset = ftell( locations );
			fscanf( locations, "%d", &map[i].loc_id );
			i++;
		}
	}
}

/* init_loc_map being phased out, because 1 years worth of locs will now be stored in an array. 
 * this same logic will be used to make a yearmap to facilitate this.
 */
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

void get_locs( FILE* l_file, long offset, location locs[MAX_LOCS] ){
	fseek( l_file, offset, SEEK_SET ); //seek to the start of the desired location
	int i=1, c;
	while( (c = fgetc( l_file ) ) != '&' && c != EOF ){
		if( c == '@' ){
			//locs[i++] = get_location( l_file );
			get_location( l_file, &locs[i++] );
		}
	}
}

void get_location( FILE* locations, location* new_l ) { 
	reset_location( new_l );
	fscanf( locations, "%s%d", (*new_l).loc_name, &(*new_l).year );
	int c, effect = 0, condred = 0;
	char* tempptr = (*new_l).body;
	while( (c = fgetc( locations ) ) != '#' ){
		switch( c ){
			case '^': //starting an effect
				if( effect < MAX_EFFECTS ){
					fscanf( locations, "%s%d", (*new_l).effects[effect].keyword,
								 &(*new_l).effects[effect].value );
					effect++;
				}
				break;
			case '?': //starting conditional redirect
				if( condred < MAX_CONDREDS ){
					fscanf( locations, "%s%d%s", (*new_l).condreds[condred].keyword,
								 &(*new_l).condreds[condred].value,
								(*new_l).condreds[condred].new_go );
					condred++;
				}
				break;
			default:		
				*(tempptr++) = c;
		}
	}
	*tempptr = '\0';
	get_options( locations, (*new_l).opts );
}

void get_options( FILE* locations, option opts[MAX_OPTIONS] ){
	int i, j, c;
	char* bodyptr;
	for( i = 0; i < MAX_OPTIONS; i++ ){
		bodyptr = opts[i].opt_body;
		fscanf( locations, "%d", &opts[i].opt_num );
		if( opts[i].opt_num == 0){
			break;
		}
		fscanf( locations, "%s", opts[i].go_to );
		for( j = 0; j < MAX_OPT_BODY && (c = fgetc( locations )) != '#'; j++ ){
			*(bodyptr++) = c;
		}
		*bodyptr = '\0';
	}
}
		
void print_location( location* ltp ){
	printf( "The year is %d, you are at location %s:%s\n", (*ltp).year, (*ltp).loc_name, (*ltp).body );
	printf( "first keyword: %s, increment: %d\n", (*ltp).effects[0].keyword, (*ltp).effects[0].value );
	print_options( (*ltp).opts );
}

void print_options( option opts[MAX_OPTIONS] ){
	int i;
	for( i = 0; i < MAX_OPTIONS && opts[i].opt_num != 0; i++ ){
		printf( "%d\t%s\n", opts[i].opt_num, opts[i].opt_body );
	}
}
