#include<stdio.h>
#include"DataStructure.h"
#include<stdlib.h>
#include<string.h>
#include<ncurses.h>
#include<errno.h>
#define KEY_DELETE 330
#define KEY_F1 265 //to save file
#define KEY_F2 266 //to save and exit
#define KEY_F3 267 //to exit
#define KEY_F4 268 //to to start selecting 
#define KEY_F5 269 //to copy
#define KEY_F6 270 //to cut 
#define KEY_F7 271 //to paste
#define KEY_F8 272 //to search
#define KEY_F9 273 //to search and replace 
#define TAB 9
#define BACK_SLASH 92
#define SINGLE 39
#define DOUBLE 34
#define KEY_SPACE 32
#define KEY_ENT 10
#define KEY_PAGEUP 339
#define KEY_PAGEDOWN 338
#define KEY_ENDB 360
#define KEY_HOMEB 262
int main(int argc, char *argv[]) {
	int saveflag;
	if(argc > 2){
		perror("Wrong arguments :");
		return errno;
	}
	if(argc == 1){
		saveflag = 0;
	}
	else{
		saveflag = 1;
	} 
	TOTALDATA t;
	inittotaldata(&t);
	char *filename;
	
	FILE *fp;
	if(saveflag){
		filename = argv[1];
		fp = openfile(filename,&t,saveflag);
	}
	else{
		//fp = openfile("t",&t,saveflag);
	}

	CURSOR cur, show, tempc;
	initcursor(&cur);
	initcursor(&show);
	SELECTED s;
	CLIPBOARD clipb;
	initselected(&s);
	initclipboard(&clipb);
	int ch,temp, flagselect = -1,x;
	char *searchstring;
	

	initscr();	
	raw();				
	keypad(stdscr, TRUE);		
	noecho();
	printscreen(&t, &show, cur);
	wmove(stdscr,cur.x - show.x,cur.y - show.y);
	refresh();
	ch = getch();
	while(!(ch == KEY_F2 || ch == KEY_F3)) {
		switch(ch) {

			case '1' : case '2' : case '3' : case '4' : case '5' :
			case '6' : case '7' : case '8' : case '9' : case '0' :
			case '-' : case '_' : case '=' : case '+' : case '!' :
			case '@' : case '#' : case '$' : case '%' : case '^' :
			case '&' : case '*' : case '(' : case ')' :	case TAB :
			case 'q' : case 'Q' : case 'w' : case 'W' : case 'e' :
			case 'E' : case 'r' : case 'R' : case 't' : case 'T' :
			case 'y' : case 'Y' : case 'u' : case 'U' : case 'i' :
			case 'I' : case 'o' : case 'O' : case 'p' : case 'P' :
			case '[' : case '{' : case ']' : case '}' : case '|' :
			case BACK_SLASH : case 'a' : case 'A': case 's' :
			case 'S' : case 'd' : case 'D' : case 'f' : case 'F' :
			case 'g' : case 'G' : case 'h' : case 'H' : case 'j' :
			case 'J' : case 'k' : case 'K' : case 'l' : case 'L' :
			case ':' : case ';' : case SINGLE : case DOUBLE :
			case 'z' : case 'Z' : case 'x' : case 'X' : case 'c' :
			case 'C' : case 'v' : case 'V' : case 'b' : case 'B' :
			case 'n' : case 'N' : case 'm' : case 'M' : case ',' :
			case '<' : case '>' : case '.' : case '?' : case '/' :
			case KEY_SPACE : case KEY_ENT: 
				addchar(&t,cur.x,cur.y,ch);
				if(ch == KEY_ENT || ch == '\n') {
					cur.y = 0;
					show.y = 0;
					cur.x++;
				}
				else {
					cur.y++;
				}
				break;
			case KEY_LEFT :
				if(cur.x == 0 && cur.y == 0) {
					//do nothing
				}
				else if(cur.y){
					cur.y--;
				}
				else{
					cur.y = t.arr[cur.x -1].length;
					cur.x--;
				}
				break;
			case KEY_RIGHT :
				if(t.arr[cur.x].length > cur.y){
					cur.y++;
				}
				else if(cur.x >= t.numoflines){}
				else{
					cur.y = 0;
					cur.x++;
				}
				break;
			case KEY_UP :
				if(cur.x > 0){
					cur.x--;
					if(t.arr[cur.x].length < cur.y)
						cur.y = t.arr[cur.x].length;
				}
				break;
			case KEY_DOWN :
				if(cur.x < t.numoflines - 1){
					cur.x++;
					if(t.arr[cur.x].length < cur.y)
						cur.y = t.arr[cur.x].length;
				}
				break;
			case KEY_BACKSPACE :
					temp = t.arr[cur.x - 1].length;
					deletechar(&t,cur.x,cur.y-1);
					if(cur.y){
						cur.y--;
					}
					else if(cur.x == 0){
						//do nothing
					}
					else{
						cur.x--;
						cur.y = temp;						
					}
					break;
				case KEY_DELETE :
					deletechar(&t,cur.x,cur.y);
					break;
				case KEY_F1 :
					//to save file
					savefile(&t,filename, &saveflag);
					break;
				case KEY_F2 :
					//save and exit
					savefile(&t,filename, &saveflag);
					break;
				case KEY_F3 :
					//savefile(&t,argv[1]);
					clear();
					//endwin();
					//return 0;
					break;
				case KEY_F4 :
					//start selecting 
					//if started selecting flag is 1
					if(flagselect == 1) {
						//start cursor is selected already
						s.end.x = cur.x;
						s.end.y = cur.y;
						flagselect = 0;
					}
					else{
						s.start.x = cur.x;
						s.start.y = cur.y;
						s.end.x = -1;
						s.end.y = -1;
						flagselect = 1;
					}
					break;
				case KEY_F5 :
					if(flagselect == -1){
						s.start.x = cur.x;
						s.start. y = cur.y;
						s.end.x = -1;
						s.end.y = -1;
					}
					if(s.start.x > s.end.x){
						//interchange
						tempc = s.start;
						s.start = s.end;
						s.end = tempc;
					}
					else if(s.start.x == s.end.x){
						//check for y
						if(s.start.y > s.end.y){
							//interchange
							tempc = s.start;
							s.start = s.end;
							s.end = tempc;
						}
					}
					else{}
					copydata(&t, &clipb, &s);
					break;
				case KEY_F6 :
					if(flagselect == -1){
						s.start.x = cur.x;
						s.start.y = cur.y;
						s.end.x = -1;
						s.end.y = -1;
					}
					if(s.end.x != -1) {
						if(s.start.x > s.end.x){
							//interchange
							tempc = s.start;
							s.start = s.end;
							s.end = tempc;
						}
						else if(s.start.x == s.end.x){
							//check for y
							if(s.start.y > s.end.y){
								//interchange
								tempc = s.start;
								s.start = s.end;
								s.end = tempc;
							}
						}
						else{}
					}
					cur.x = s.start.x;
					cur.y = s.start.y;
					cutdata(&t, &clipb, &s);
					break;
				case KEY_F7 :
					pastedata(&t, &clipb, cur);
					break;
				case KEY_F8 :
					//wmove(stdscr,LINES, 0);
					//x = 0;
					searchstring = (char *)malloc(sizeof(char) * 256);
					searchui(searchstring);
					tempc = cur;
					cur.x = 0;
					cur.y = 0;
					int p;
					
					p = KEY_DOWN;
					cur = searchstr(&t, searchstring, cur);
					while(p == KEY_DOWN && cur.x != -1){						
						printscreen(&t,&show,cur);
						wmove(stdscr,cur.x,cur.y);
						refresh();
						cur.y++;
						cur = searchstr(&t, searchstring, cur);
						p = getch();
						clear();
					}
					cur = tempc;
					free(searchstring);
					break;
				case KEY_F9 :
					//printscreen(&t,&show,cur);
					searchandreplaceui(&t);
					cur.x = 0;
					cur.y = 0;
					//printscreen(&t,&show,cur);
					break;
				case KEY_PAGEUP :
					x = cur.x - LINES - 1;
					if(x < 0) {
						cur.x = 0;
					}
					else {
						cur.x = x;
					}
					break;
				case KEY_PAGEDOWN :
					x = cur.x + LINES - 1;
					if(x > (t.numoflines - LINES - 1)){
						cur.x = t.numoflines - LINES - 1;
					}
					else{
						cur.x = x;
					}
					break;
				case KEY_ENDB :
					cur.y = t.arr[cur.x].length;
					break;
				case KEY_HOMEB :
					cur.x = 0;
					cur.y = 0;
				default :
				{}




 		}
 		printscreen(&t, &show, cur);
		wmove(stdscr, cur.x - show.x, cur.y - show.y);
		refresh();
		ch = getch();
		clear();
		if(t.numoflines >= t.arrsize - 1){
			increasetotaldata(&t);
		}
		if(ch == KEY_F2){
			savefile(&t,filename, &saveflag);
		}
	}
	endwin();
	return 0;
}