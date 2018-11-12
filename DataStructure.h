/*	we will strore the lines read from the file into the data structure  
	*line will be the NULL terminated linked list
	*the number of lines will be saved as the array of line pointers
	*/

typedef struct charnode {
	char c;
	struct charnode *next, *prev;
}CHARNODE;

typedef struct line {
	CHARNODE *head, *tail;
	int length;
}LINE; //this will store a line

typedef struct totaldata {
	LINE *arr;
	int arrsize;
	int numoflines;
}TOTALDATA;//this will try to save the data
	//always we will try to check the number of lines
	// are grater than the array 

typedef struct cursor {
	int x, y;
}CURSOR;

typedef struct selected {
	CURSOR start,end;
}SELECTED;

typedef struct clipboard {
	char *clip;
	int length;
}CLIPBOARD;
//Functions :

//For the lines :
void addchar(TOTALDATA *t, int linenumber, int col, int ch);
void deletechar(TOTALDATA *t, int linenumber,int col);

//file save function
void savefile(TOTALDATA *t, char * filename, int *flag);
FILE * openfile(char *filename, TOTALDATA *t, int flag);
void saveui(char * str);

//For totaldata
void inittotaldata(TOTALDATA *t);
void initcursor(CURSOR *c);
void initselected(SELECTED *s);
void initclipboard(CLIPBOARD *clip);
void increasetotaldata(TOTALDATA *t);

//for searching data
CURSOR searchstr(TOTALDATA *t, char *searchstr, CURSOR start);
void searchandreplace(TOTALDATA *t,char *str, char * replace);
void searchandreplaceui(TOTALDATA *t);
void searchui(char *str);
//for data copy and paste data
void copydata(TOTALDATA *t, CLIPBOARD *c,SELECTED *s);
void cutdata(TOTALDATA *t, CLIPBOARD *c, SELECTED *s);
void pastedata(TOTALDATA *t, CLIPBOARD *c, CURSOR s);


//printing the datastructure on the screen everytime
void printscreen(TOTALDATA *t, CURSOR *show, CURSOR cur);


//for dubugging purpose
void printtotaldata(TOTALDATA *t);
void printtotaldata2(TOTALDATA *t);
