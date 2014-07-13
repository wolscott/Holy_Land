// global marker 'B' set here in vim

#include "holy_land.h" //header file that contains all definitions and prototypes

/* process_location **
 this function will check condreds, update keywords based
 on effects, and create the current list of valid options.
 Since conditional options are not planned yet, this will
 be all options. This will eventually be moved to a seperate
 file with the rest of the mechanical aspects that shouldn't
 be in the frontend.
*/
void process_location( location* loc, l_pair* status ){
	do_effects( *loc->effects, status );
} //end process_location

void do_effects( l_pair* loc_e_head, l_pair* player_e_head ){
	l_pair* effect = loc_e_head;
	while( effect != NULL ){ //systematically handle all of the location's effects
		handle_effect( effect, player_e_head );
		effect = effect->next;
	}
}

void handle_effect( l_pair* effect, l_pair* player_e_head ){
	l_pair* curstat = player_e_head;
	while( curstat != NULL ){
		if( strcmp( curstat->keyword, effect->keyword ) == 0 ){ //if there is a keyword match	
			curstat->value += effect->value;
			return;
		} else {
			curstat = curstat->next;
		}		
	}
	//if the while loops ends, that means that no match was found, so add a new pair to the status	
	l_pair* new_effect = malloc( sizeof( l_pair ));
	new_effect->next = NULL;
	strcpy( new_effect->keyword, effect->keyword );
	new_effect->value = effect->value;
	append_pair( &player_e_head, new_effect );
}
