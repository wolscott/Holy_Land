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
	location* main_menu = get_loc_from_name( loc_array, 0, num_locs, "default" );
	display_location( main_menu );
	free_loaded( loc_array, num_locs );
}//main

/* proctess_location **
 this function will check condreds, update keywords based
 on effects, and create the current list of valid options.
 Since conditional options are not planned yet, this will
 be all options.
*/
void process_location( location* loc ){
	
} //end process_location

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
