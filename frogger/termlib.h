/* kblib v3.0 

Linux support for keyboard functions for getch and getche and kbhit 
ansy.sys Like functions added
* 
by Daniel Jacoby 10 oct 2016 
*/

#ifndef KBLIB_H
#define KBLIB_H

int getch(void);	  // Get a key without waiting for it (no echo) 
int getche(void);	  // Get a key without waiting for it (with echo) 
int kbhit(void);	  // Returns 1 if a key is waiting in the keyboad buffer
void changemode(int); // Turns terminal line buffering on or off
 

#define BUFFERED_ON  1
#define BUFFERED_OFF 0


// Ansi.sys like

void gotoxy(int x, int y);   // x is colum and y is row
void clrscr();				 // Clear Screen 	

// ============ Colored Text ===========

#define BLACK_TEXT		   "\e[30;1m"
#define RED_TEXT		   "\e[31;1m"
#define GREEN_TEXT 		   "\e[32;1m"
#define YELLOW_TEXT 	   "\e[33;1m"
#define BLUE_TEXT 		   "\e[34;1m"
#define MAGENTA_TEXT 	   "\e[35;1m"
#define CYAN_TEXT 	   	   "\e[36;1m"
#define WHITE_TEXT 		   "\e[37m"

// How to use:  printf(RED_TEXT "Colored Text \n" WHITE_TEXT);


#endif
