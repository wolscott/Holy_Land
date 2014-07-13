/* hl_fontend.c
 this file contains the code for interacting with the user
*/

#include "holy_land.h" //header file that contains all definitions and prototypes

int main(){ //global mark 'F' set here in vim
	int num_locs = 0;
	location** loc_array = load_story( &num_locs ); //in hl_loader.c
	free_loaded( loc_array, num_locs );
}//main
