/* hl_fontend.c
 this file contains the code for interacting with the user
*/

#include "holy_land.h" //header file that contains all definitions and prototypes

/***************************************

The main function will call the load_story function from hl_loader.c. 
Then it will present the user with the default location or main menu,
allowing them to start a new game or load an in progress one.

***************************************/

int main(){ //global mark 'F' set here in vim
	int num_locs = 0;
	location** loc_array = load_story( &num_locs ); //in hl_loader.c
	location* curloc = get_loc_from_name( loc_array, 0, num_locs, "default" );
	int game = 1; //placeholder, replace this with a keyword
	char input[MAX_INPUT];
	l_pair* status_list = NULL;
	//main io loop
	while( game == 1 ){
		process_location( &curloc, &status_list );
		display_location( curloc );
		scanf( "%s", input, MAX_INPUT );
		game = process_input( input, &curloc );	
	}
	free_loaded( loc_array, num_locs ); //final step: free all allocated memory.
}//main

/* process_input **
 this function parses the user's input for commands
 and performs the actions instructed
*/
int process_input( char* input, location** curloc ){
	if( strcmp( input, "quit" ) == 0 ){
		return 0;
	}
	int i;
	if( isdigit( input[0] )){
		option* selection = *(*curloc)->options;		
		for( i = 1; i < atoi( input ) && selection->next != NULL; i++ ){
			selection = selection->next;	 
		}
		*curloc = selection->target;			
	}
	return 1;
}


/* display_location **
 this function is the function that will be used by the game
 to display locations to the player. 
*/
void display_location( location* loc ){
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
