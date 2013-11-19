//HOLY LAND, an interactive story/text based adventure game
//by Scott Wolcott
//don't expect much, it's shitty

#include <stdio.h>
/* holylandgamelocations.c holds the locations, locationMap, and locationOptions functions
 */

int locationMap (int, int, int); //prototype
int locations( int); //prototype
int locationOptions( int); //prototype

/*the locationMap function is called from the chapter function after the locations function
 * prints the current location and choices. the locatonMap function tells 
 * which input 1-n determines which location to be displayed on the next loop 
 * of the chapter function
 */
int locationMap (int chapterNumber, int locationNumber, int input)
{
	switch (chapterNumber)
	{
		case 1: //chapter1
			switch (locationNumber)
			{
				case 0: //location 0 "the road"
					switch (input)
					{
						case 1:
							return 1;
						case 2:
							return 2;
					}
					break;
				case 1: //location 1 "The chicken coop"
					switch (input)
					{
						case 1:
							return 2;
						case 2:
							return 0;
					}
					break;
				case 2: //location 2 "fix the coop"
					switch (input)
					{
						case 1:
							return 0;
					}
				case 3: //location 3 "the coop is fixed"
					switch (input)
					{
						case 1:
							return 0;
						case 2:
							return 1;
					}
				//chapter 2
				case 10:
					switch (input)
					{
						case 1:
							return 11;
						case 2:
							return 12;
						case 3:
							return 13;
					}
				case 11:
					switch (input)
					{
						case 1:
							return 11;
						case 2:
							return 11;
						case 3:
							return 11;
						case 4:
							return 12;
						case 5:
							return 13;
					}
				case 12:
					switch (input)
					{
						case 1:
							return 12;
						case 2:
							return 11;
						case 3:
							return 13;
					}
				case 13:
					switch (input)
					{
						case 1:
							return 13;
						case 2:
							return 10;

					}
				break;
			}
	}
}
/* the locationOptions function prints the options available for the current location.
 * It is called from the chapter function and recieves the locationNumber value.
 * it returns the max value used by checkInput.
 */
int locationOptions (int locationNumber)
{
	switch (locationNumber)
	{
		case 0:
			printf("Where would you like to go?\n1: To the chicken coop\n2: to the smithy\n");
			return 2;
		case 1:
			printf("What will you do?\n1: Attempt to fix the chicken coop\n2: go back to the road\n");
			return 2;
		case 2:
			printf("Where would you like to go?\n1: Back to the starting location\n2: Location 1\n");
			return 2;
		//chapter 2
		case 10:
			printf("Where do you want to go?\n1: Go towards the fight\n2: Go to the blacksmiths\n3: Go to the kitchen\n");
			return 3;
		case 11:
			//the fight
			printf("What will you do?\n1: Join in beating the kid that is down\n2: Join in against the bullies\n3: Watch the fight but do nothing\n4: Go to the blacksmith\n5: Go back and and see what your mother needs in the kitchen\n");
			return 5;
		case 12:
			//the smithy
			printf("What will you do?\n1: Stay here and watch the smith work\n2: leave and go see the fight\n3: Go see what your mother needs in the kitchen\n");
			return 3;
		case 13:
			//the kitchen
			printf("What will you do?\n1: Stay here and help your mother\n2: go back outside\n");
	}
}
/*the locations function is called from the chapter function has all of the location descriptions
 * hardcoded into it. in order to display the correct text, the locations functon recieves the
 * locationNumber value when called from the chapter function.
 * It returns a value that is appended to pastChoices
 */
int locations ( int locationNumber) 
{
	switch (locationNumber)
	{
		case 0:
			printf("You are standing in the road outside a small village near your home\n");
			return 0;
			//break;
		case 1:
			printf("You are at the chicken coop.\n");
			return 1;
			break;
		case 2:
			printf("you are at the smithy\n");
			return 10;
			break;
		//chapter2
		case 10:
			puts("You are in the street outside your home. You can see some children fighting in the distance. Your mother is calling your name from the kitchen.");
			return 0;
		case 11:
			//the fight
			puts("Some children are fighting here. One is on the ground trying to get up while three others kick him.");
			return 0;
		case 12:
			//the blacksmith
			puts("As you enter the smithy, it takes your eyes awhile to adjust to the dark. Once they do, you watch the smith working the steel.");
			return 100;
		case 13:
			//the kitchen
			puts("As soon as you enter the kitchen, your mother puts you to work helping her prepair the day's meals.");
			return 1000;
	}
	return 0;
}
