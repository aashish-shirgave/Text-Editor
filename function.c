#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include "DataStructure.h"
#include <errno.h>

void inittotaldata(TOTALDATA *t) {
	//mhanje apan eth line cha array declare karnar 
	//ani initialize with NULL 
	int i;
	t->arrsize = 1024;
	t->numoflines = 0;
	t->arr = (LINE*)malloc(sizeof(LINE) * 1024);
	for(i = 0; i < 1024; i++) {
		(t->arr)[i].head = (t->arr)[i].tail = NULL;
		(t->arr)[i].length = 0;

	}
}
void increasetotaldata(TOTALDATA *t) {
	int i;
	LINE *temp;
	t->arrsize *= 2;
	temp = (LINE *)malloc(sizeof(LINE) * t->arrsize);
	for(i = 0; i < t->numoflines; i++) {
		temp[i] = t->arr[i];
	}
	free(t->arr);
	t->arr = temp;
	for(i = t->numoflines; i < t->arrsize; i++) {
		(t->arr)[i].head = (t->arr)[i].tail = NULL;
		(t->arr)[i].length = 0;
	}

}
void initcursor(CURSOR *c) {
	c->x = c->y = 0;
}
void initselected(SELECTED *s) {
	s->start.x = s->start.y = s->end.x = s->end.y = -1;
}
void initclipboard(CLIPBOARD *clip) {
	clip->clip = NULL;
	clip->length = 0;
}
void addchar(TOTALDATA *t, int linenumber, int col, int ch) {
	//add a character at specified position 
	//the position will be given by ncurses
	//doubly linked NULL terminated linked list

	//validate the data from the function call
	
	CHARNODE *p;
	int i;
	//jr te \n asel tr vegala code
	if(ch == KEY_ENTER || ch == '\n') {
		//go to that node
		for(i = t->numoflines; i > linenumber+1; i--) {
			(t->arr)[i] = (t->arr)[i-1];
		}
		//append it to the next line
		//create a space for the line
		if(col == (t->arr)[linenumber].length) {
			(t->arr)[linenumber + 1].head = (t->arr)[linenumber+1].tail = NULL;
			(t->arr)[linenumber +1].length = 0;
			t->numoflines++;
			return;
		}
		if(col == 0) {
			(t->arr)[linenumber + 1].head = (t->arr)[linenumber].head;
			(t->arr)[linenumber + 1].tail = (t->arr)[linenumber + 1].tail;
			(t->arr)[linenumber].head = NULL;
			(t->arr)[linenumber].tail = NULL;
			(t->arr)[linenumber +1].length = (t->arr)[linenumber].length;
			(t->arr)[linenumber].length = 0; 
			t->numoflines++;
			return;
		}
		p = (t->arr)[linenumber].head;
		for(i = 0; i < col-1; i++) {
			p = p->next;
		}
		
		(t->arr)[linenumber + 1].head = p->next;
		(t->arr)[linenumber + 1].tail = (t->arr)[linenumber].tail;
		p->next->prev = NULL;
		p->next = NULL;
		(t->arr)[linenumber].tail = p;
		(t->arr)[linenumber + 1].length = (t->arr)[linenumber].length - col;
		(t->arr)[linenumber].length = col;

		t->numoflines++;
		return;
	}
	//adding a character at given node
	//serach the node
	//if it is at last (Most obvious condition)
	CHARNODE *q;
	q = (CHARNODE *)malloc(sizeof(CHARNODE));
	q->c = ch;
	//for first node
	if((t->arr)[linenumber].head == NULL) {
		(t->arr)[linenumber].head = q;
		(t->arr)[linenumber].tail = q;
		q->next = q->prev =NULL;
		if(linenumber >= t->numoflines){
			t->numoflines ++;
		}
		(t->arr)[linenumber].length++;
		return;
	}
	if(col == 0){
		//head
		q->prev = NULL;
		q->next = (t->arr)[linenumber].head;
		(t->arr)[linenumber].head->prev = q;
		(t->arr)[linenumber].head = q;
		if(linenumber >= t->numoflines){
			t->numoflines ++;
		}	
		(t->arr)[linenumber].length++;
		return;
	}
	else if(col == (t->arr)[linenumber].length) {
		q->next = NULL;
		q->prev = (t->arr)[linenumber].tail;
		(t->arr)[linenumber].tail->next = q;
		(t->arr)[linenumber].tail  = q;
		(t->arr)[linenumber].length++;

		return;

	}
	else {
		p = (t->arr)[linenumber].head;
		//p  node cha nantr add karyacha ahe apalyala
		for(i = 0; i < col-1; i++) {
			p = p->next;
		}
		
		//adding a node next to p
		q->next = p->next;
		q->prev = p;
		p->next = q;
		q->next->prev = p;
		(t->arr)[linenumber].length++;
		
		return;
	}
}

void deletechar(TOTALDATA *t, int linenumber,int col) {
	//this will be called when the backspace is called or delete button
	//we will get the position where the character is to be deleted
	//will get -1 when backspace is pressed at first of the line
	//and if delete is pressed at last of the line we will get index
	int i;
	if((linenumber == t->numoflines - 1) && (t->arr[linenumber].length == col)){
		return;
	}
	//means backspace at start of the line
	if(col == -1) {
		//append this line to the previous line
		//if the line before is NULL
		if(linenumber == 0){
			return;
		}
		if(t->arr[linenumber - 1].head == NULL){
			
			t->arr[linenumber - 1].head = t->arr[linenumber].head;
			t->arr[linenumber].tail = t->arr[linenumber].tail;
			t->arr[linenumber -1].length = t->arr[linenumber].length;
			//return;
		}
		else if(t->arr[linenumber].head == NULL){
			
		}
		else {
			t->arr[linenumber - 1].tail->next = t->arr[linenumber].head;
			t->arr[linenumber].head->prev = t->arr[linenumber - 1].tail;
			t->arr[linenumber -1].tail = t->arr[linenumber].tail;
			t->arr[linenumber -1].length += t->arr[linenumber].length; 
			t->arr[linenumber].length = 0;
		}
		//move all lines
		t->numoflines --;
		for(i = linenumber; i < t->numoflines; i++){
			t->arr[i] = t->arr[i+1];
		}
		t->arr[i].head = NULL;
		t->arr[i].length = 0;
		t->arr[i].tail = NULL;
		
		return;
	}
	//delete at the end of line
	if(col == t->arr[linenumber].length) {
		if(t->arr[linenumber].head == NULL){
			t->numoflines --;
		for(i = linenumber; i < t->numoflines; i++){
			t->arr[i] = t->arr[i+1];
		}
		return;

		}
		if(t->arr[linenumber].head != NULL) {
		//append the next line to it
			t->arr[linenumber].tail->next = t->arr[linenumber+1].head;
			t->arr[linenumber+1].head->prev = t->arr[linenumber].tail;
			t->arr[linenumber].tail = t->arr[linenumber+1].tail;
			t->arr[linenumber].length += t->arr[linenumber+1].length;
		//move all the lines
		}
		t->numoflines --;
		for(i = linenumber + 1; i < t->numoflines; i++){
			t->arr[i] = t->arr[i+1];
		}
		t->arr[i].head = NULL;
		t->arr[i].length = 0;
		t->arr[i].tail = NULL;
		return;
	}
	CHARNODE * p;
	//and if not this then we will do the deletion in single line
	//in this there will be threee cases 
	//if lenghth of line is 1
	if(t->arr[linenumber].tail == t->arr[linenumber].head){
		free(t->arr[linenumber].head);
		t->arr[linenumber].head = t->arr[linenumber].tail = NULL;
		t->arr[linenumber].length --;
		return;
	}
	if(col == t->arr[linenumber].length -1){
		//this is written first bcoz its most obvious condition
		//last character
		p = t->arr[linenumber].tail;
		t->arr[linenumber].tail = t->arr[linenumber].tail->prev;
		t->arr[linenumber].tail->next = NULL;
		t->arr[linenumber].length --;

		free(p);
		return;

	}
	if(col == 0){
		//first character
		p = t->arr[linenumber].head;
		t->arr[linenumber].head = t->arr[linenumber].head->next;
		t->arr[linenumber].head->prev = NULL;
		t->arr[linenumber].length --;

		free(p);
		return;
	}

	//p delete kar
	//midddle character
	p = t->arr[linenumber].head;
	for (i = 0; i < col; i++){
		p = p->next;
	}
		t->arr[linenumber].length --;

	p->next->prev = p->prev;
	p->prev->next = p->next;
	free(p);
}


FILE * openfile(char *filename, TOTALDATA *t,int flag) {
	//opens a file
	//in both read and write mode
	//reads data from file saves to TOTALDATA
	FILE *fp;
	if(flag){
		int x = 0, y = 0;
		char ch;
		fp = fopen(filename,"r+");
		if(fp == NULL) {
			fp = fopen(filename,"w+");
		}
		fscanf(fp,"%c", &ch);
		while(!feof(fp)) {
			
			addchar(t, x, y, ch);
			y++;
			if(ch == '\n') {
				x++;
				y = 0;
			}
			fscanf(fp,"%c", &ch);
		}
 	}
 	else{
 		//fp = fopen(,"w+");
 	}
 	return fp;
}
void savefile(TOTALDATA *t,char * filename, int *flag) {
	CHARNODE *p;
	int i;
	FILE *fp;
	if(*flag == 0){
		filename = (char*)malloc(sizeof(char) * 256);
		saveui(filename);
		*flag = 1;
	}
	fp = fopen(filename, "w+");
	if(fp == NULL) {
		perror("Unable to open file");
		exit(errno);
	}
	for(i = 0; i < t->numoflines; i++){
		p = t->arr[i].head;
		while(p != NULL) {
			fprintf(fp,"%c",p->c);
			p = p->next;
		}
		fprintf(fp,"\n");
	}

}

void copydata(TOTALDATA *t, CLIPBOARD *c,SELECTED *s){
	//we will increase clipboard size if needed
	//this function will copy to clipboard which is string
	//clipboard will contain the data with '\n'
	//we will copy the line if selected is NULL(means -1)

	//clear clipboard or init it
	char *str;
	c->length = 1024;
	str = (char *)malloc(sizeof(char) * 1024);
	c->clip = str;
	int linenumber, i = 0;
	if(s->end.x == -1){
		//means we have to copy a line
		CHARNODE *p;
		linenumber = s->start.x;
		p = t->arr[linenumber].head;
		while(p) {
			c->clip[i] = p->c;
			i++;
			p = p->next;
		}
		c->clip[i] = '\0';

	}
	else {
		//copy from start to end
		//first line
		CHARNODE *p;
		int j,y;
		linenumber = s->start.x;
		p = t->arr[s->start.x].head;
		for(j = 0; j < s->start.y; j++)
			p = p->next;
		y = j;
		while(!(linenumber == s->end.x && y == s->end.y)) {
			//printf("while madhe alo\n");
			if(p){
				c->clip[i++] = p->c;
				p = p->next;
				y++;
			}
			else{
				c->clip[i++] = '\n';
				linenumber ++;
				p = t->arr[linenumber].head;
				y = 0;
			}
		}
		//add \0 at last 
		c->clip[i] = '\0';
	}
	initselected(s);
}

void pastedata(TOTALDATA *t, CLIPBOARD *c, CURSOR s){
	//cursor position addchar 
	int x, y, n, i;
	x = s.x;
	y = s.y;
	n = strlen(c->clip);
	for(i = 0; i < n; i++){
		addchar(t, x, y, c->clip[i]);
		y++;
		if(c->clip[i] == '\n') {
			x++;
			y = 0;
		}
	}
}

void cutdata(TOTALDATA *t, CLIPBOARD *c, SELECTED *s) {
	//copy data to the clipboard then delete the data
	int n, x, y, i;
	//copydata
	x = s->start.x;
	y = s->start.y;
	copydata(t, c, s);
	//deletedata
	
	if(s->end.x == -1){
		y = 0;
	}
	else
		y = s->start.y;
	n = strlen(c->clip);
	for(i = 0; i < n ; i++) {
		deletechar(t,x,y);
	}
	
}

CURSOR searchstr(TOTALDATA *t, char *searchstr, CURSOR start){
	//it will output a cursor 
	//it will search for the character sequence after the input CURSOR
	int linenumber, col, i, j = 0;
	CURSOR result;
	CHARNODE *p;
	linenumber = start.x;
	col = start.y;
	//moving to the CURSOR point
	p = t->arr[linenumber].head;
	for(i = 0; i < col; i++)
		p = p->next;

	//making strings from the data structure line by line 
	//and searching for string
	//for first line
	char *str, *res, flag = 0;
	str = (char *) malloc(sizeof(char) * (t->arr[linenumber].length - col));
	if(str == NULL) {
		printf("no space allocated\n");
	}
	while(p){
		str[j++] = p->c;
		p = p->next;
	}
	str[j] = '\0';
	res = strstr(str, searchstr);
	if(res != NULL){
		//returning appropriate pointer
		result.x = linenumber;
		p = t->arr[i].head;
		j = 0;
		while((str + j) != res) {
			j++;
		}
		result.y = j;
		return result;
	}
	else {
		for(i = linenumber  + 1; i < t->numoflines; i++){
			j = 0;
			free(str);
			str = (char *) malloc(sizeof(char) * (t->arr[i].length));
			p = t->arr[i].head;
			while(p) {
				str[j++] = p->c;
				p = p->next;
			}
			str[j] = '\0';
			res = strstr(str, searchstr);
			if(res){
				flag = 1;
				break;
			}
		}
	}
	//returning appropriate CURSOR position
	if(flag) {
		//if found
		result.x = i;
		p = t->arr[i].head;
		j = 0;
		while((str + j) != res) {
			j++;
		}
		result.y = j;
	}
	else{
		//not found
		//return cursor with -1
		result.x = -1;
		result.y = -1;
	}
	return result;
}

void searchandreplace(TOTALDATA *t,char *str, char * replace){
	//search and replace from whole file
	CURSOR cur;
	int n1, n2, i;
	n1 = strlen(str);
	n2 = strlen(replace);
	cur.x = 0;
	cur.y = 0;
	printf("%d %d\n",n1, n2 );
	cur = searchstr(t, str, cur);
	//
	while(cur.x != -1) {
		//printf("\n%d %d\n",cur.x,cur.y );
		for(i = 0; i < n1; i++) {
			deletechar(t,cur.x,cur.y);
			//printf("%d ", i);
		}
		for(i = 0; i < n2; i++) {
			addchar(t, cur.x, cur.y, replace[i]);
			cur.y++;
			if(replace[i] == '\n') {
				cur.x++;
				cur.y = 0;
			}
		}
		if(t->arr[cur.x].length > cur.y){
			cur.y++;
		}
		else {
			cur.x++;
			cur.y = 0;
		}
		cur = searchstr(t, str, cur);
	}

}

void printscreen(TOTALDATA *t, CURSOR *show, CURSOR cur){
	//show CURSOR is starting of page display
	// cur CURSOR is for current state of cursor
	int xmax, ymax, ystart, xdiff, ydiff, x, y, i, j, l;
	CHARNODE * p;
	l = LINES - 1;
	xmax = l - 1;
	ymax = COLS - 1;
	
	if(show->x > cur.x) {
		show->x = cur.x;
	}
	else{
		xdiff = cur.x - show->x;
		if(xdiff > xmax) {
			show->x = show->x + xdiff - xmax ;
		}
		else if(show->x != 0){
			show->x = show->x + xdiff - xmax;
			if(show->x < 0)
				show->x = 0;
		}
			
	}
	
	if(show->y > cur.y) {
		show->y = cur.y;
	}
	else {
		ydiff = cur.y - show->y;
		if(ydiff > ymax) {
			show->y = show->y + ydiff - ymax;
		}
		else if(show->y != 0){
			show->y = show->y - ymax + ydiff;
			if(show->y < 0 )
				show->y = 0;
		}
	}
	
	
	
	
	//now printing from the show cursor
	x = show->x + l;
	y = show->y + COLS;
	
	for(i = show->x; i < x; i++) {
		//printw("%d ",i);
		p = t->arr[i].head;
		for(j = 0; (j < y) && p; j++){
			if(j >= show->y) {
				printw("%c", p->c);
			}

			p = p->next;
		}
		if(i >= t->numoflines){
			printw("~");
		}
		printw("\n");
		
	}
	for( i = 0; i < COLS - 7; i++){
		printw(" ");
	}
	printw("%d %d",cur.x, cur.y);
}

WINDOW * create_newwin(int height, int width, int starty, int startx)
{	WINDOW *local_win;

	local_win = newwin(height, width, starty, startx);
	box(local_win, 0 , 0);		/* 0, 0 gives default characters 
					 * for the vertical and horizontal
					 * lines			*/
	wrefresh(local_win);		/* Show that box 		*/

	return local_win;
}

void destroy_win(WINDOW *local_win)
{	
	/* box(local_win, ' ', ' '); : This won't produce the desired
	 * result of erasing the window. It will leave it's four corners 
	 * and so an ugly remnant of window. 
	 */
	wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	/* The parameters taken are 
	 * 1. win: the window on which to operate
	 * 2. ls: character to be used for the left side of the window 
	 * 3. rs: character to be used for the right side of the window 
	 * 4. ts: character to be used for the top side of the window 
	 * 5. bs: character to be used for the bottom side of the window 
	 * 6. tl: character to be used for the top left corner of the window 
	 * 7. tr: character to be used for the top right corner of the window 
	 * 8. bl: character to be used for the bottom left corner of the window 
	 * 9. br: character to be used for the bottom right corner of the window
	 */
	wrefresh(local_win);
	delwin(local_win);
}
void searchandreplaceui(TOTALDATA *t) {
	WINDOW *my_win;
	int startx, starty, width, height, x4, y4;
	char *orig = (char*)malloc(256 * sizeof(char));
	char *new = (char*)malloc(256 * sizeof(char));
		
	height = 10;
	width = 100;
	my_win =create_newwin(height, width, LINES - 3, 0);
	echo();
	mvwprintw(my_win,1, 1, "Enter the word to be replaced : ");
	refresh();
	wgetstr(my_win, orig);
	mvwprintw(my_win,2, 1, "Enter the new word : ");
	refresh();
	wgetstr(my_win, new);
	searchandreplace(t, orig, new);
	noecho();
	destroy_win(my_win);
	endwin();

}
void searchui(char * str){
	WINDOW *my_win2;
	int startx, starty, width, height, x4, y4;
	height = 10;
	width = 100;
	my_win2 =create_newwin(height, width, LINES - 3, 0);
	echo();
	mvwprintw(my_win2,1, 1, "Enter the word to be searched :");
	refresh();
	wgetstr(my_win2, str);
	noecho();
	destroy_win(my_win2);
	endwin();
	return;
}
void saveui(char * str){
	WINDOW *my_win2;
	int startx, starty, width, height, x4, y4;
	height = 10;
	width = 100;
	my_win2 =create_newwin(height, width, LINES - 3, 0);
	echo();
	mvwprintw(my_win2,1, 1, "Enter the file name :");
	refresh();
	wgetstr(my_win2, str);
	noecho();
	destroy_win(my_win2);
	endwin();
	return;
}
void printtotaldata(TOTALDATA *t) {
	
	CHARNODE *p;
	int i;

	for(i = 0; i < t->numoflines; i++){
		p = t->arr[i].head;
		
		while(p != NULL) {
			printw("%c",p->c);
			p = p->next;
		}
		printw("\n");
	}
}



void printtotaldata2(TOTALDATA *t) {
	
	CHARNODE *p;
	int i;
	//printf("I'm printing the lines\n");
	printf("\nnum of lines = %d \n",t->numoflines);
	for(i = 0; i< t->numoflines; i++) {
		printf("%d -> %d \n",i,t->arr[i].length);
	}
	int j = 0;
	printf("%d\n",t->numoflines );
	for(i = 0; i < t->numoflines; i++){
		p = t->arr[i].head;
		j = 0;
		printf("line %d ",i);
		while(p != NULL) {
			printf("\t %d -> %c\n",j,p->c);
			p = p->next;
			j++;
		}
		printf("\n");
	}
	printf("..........%d %d\n",LINES, COLS );
}