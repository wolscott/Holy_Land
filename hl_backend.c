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
void process_location( location** loc, l_pair** status ){
	location* go_to = do_condreds( (*loc)->condreds, status );
	if( go_to != *loc && go_to != NULL ){
		*loc = go_to;
		process_location( loc, status );
	} else {
		//*loc = go_to;
		do_effects( (*loc)->effects, status );
	}
} //end process_location

location* do_condreds( l_cond** condreds, l_pair** status ){
	if( condreds == NULL ){
		return NULL;
	}
	l_cond* curcond = *condreds;
	l_pair* curstat = *status;
	while( curcond != NULL ){ //for each condred
		while( curstat != NULL ){ //for each status
			if( strcmp( curcond->keyword, curstat->keyword ) == 0 ){ //if there's a keyword match
				if( curstat->value >= curcond->value ){ //if the value is met
					return curcond->new_go_ptr; //return the new location pointer
				}
			}
			curstat = curstat->next;
		}
		curcond = curcond->next;
	}
	return NULL;
}

void do_effects( l_pair** effects, l_pair** player_e_head ){
	if( effects == NULL ){
		return;
	}
	l_pair* effect = *effects;
	while( effect != NULL ){ //systematically handle all of the location's effects
		handle_effect( effect, player_e_head );
		effect = effect->next;
	}
}

void handle_effect( l_pair* effect, l_pair** player_e_head ){
	l_pair* curstat = *player_e_head;
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
	append_pair( player_e_head, new_effect );
}
