#include<stdio.h>
#include<dirent.h>
#include<stdlib.h>
#include<ncurses.h>
#include<string.h>

int directoryItemCount(char dirName[]);

// struct to contain all of the neccesary values to directory you are viewing
typedef struct{
    char *name;
    char *path;
    int itemCount;
    int currentIndex;
    //Pointer to an array of pointers to directory entity structs
    struct dirent *(*items)[];
} open_directory;

//A windows that contains a directory
typedef struct{
    WINDOW * parentWindow;
    WINDOW * window;
    open_directory * directory;
} directory_window;

//Function declarations
void refresh_directory_items(open_directory *openDir);
struct dirent * item_by_index(open_directory *, int);
WINDOW * new_window(WINDOW * parentWindow, bool outline);
open_directory * open_new_directory(char * dirPath);
directory_window * new_directory_window(char * dirPath, WINDOW * parentWindow);



int main(){
    int i;
    //Ncurses boilder plate
    initscr();
    noecho();
    raw();
    curs_set(0);


    //initial opening directory
    char * targetDir = "/home/lmayall";
    int selectedIndex = 1;

    WINDOW * rootWin = new_window(NULL, true);
    WINDOW * contentWin = new_window(rootWin, false);

    directory_window * testWin = new_directory_window(targetDir, contentWin);

    for (i = 0; i < testWin->directory->itemCount; ++i) {
        wprintw(testWin->window, "%i - %s\n", i, (*testWin->directory->items)[i]->d_name);
    }

    refresh();
    wrefresh(rootWin);
    wrefresh(testWin->window);

    getch();
    endwin();
    return 0;
}

//Counts all of the items in a directory, inificient, when opening a directory you have to go through it twice, once to
// count and once to retrieve items :(
int directoryItemCount(char * dirName){
    DIR * currentDir = NULL;
    struct dirent *ent = NULL;

    currentDir = opendir(dirName);
    int itemCount = 0;

    while((ent = readdir(currentDir))){
        itemCount = itemCount + 1;
    }

    closedir(currentDir);

    return itemCount;
}


//refreshes the items in a directory
void refresh_directory_items(open_directory *openDir){
    int i;
    DIR * currentDir = NULL;

    currentDir = opendir(openDir->path);

    for (i = 0; i < openDir->itemCount; i = i + 1) {
        //This is a pointer to an array of pointers, setting index i to a pointer, its anoyying ik
        (*openDir->items)[i] = readdir(currentDir);
    }

    closedir(currentDir);
}

//Returns the item at an index, nothing major
struct dirent * item_by_index(open_directory * currentDir, int index){
    return (*currentDir->items)[index];
}

//TODO NEED TO MAKE A close_directory TO AVOID MEMORY LEAK
//Opens a new directory and assigns it proper values, fuck me this was anoyying
open_directory * open_new_directory(char * dirPath){
    //Make a new open_directory and assign it memory
    open_directory * newDir = malloc(sizeof(open_directory *));

    // Sets the current (selected) index to 0 and stores pointer in struct
    int currentIndex = 0;
    newDir->currentIndex = currentIndex;

    //gets the items inside the directory, stores pointer in struct
    int itemCount = directoryItemCount(dirPath);
    newDir->itemCount = itemCount;

    //Stores copy to new pointer of dirPath in struct
    newDir->path = strdup(dirPath);

    // allocates memory for the item array, to the size of itemCount, if itemCount changes have to close and then reopen directory
    newDir->items = malloc(sizeof(struct dirent *) * newDir->itemCount);
    // itterates through directory and assigns the array values, takes info from the struct not a passthrough
    refresh_directory_items(newDir);

    printw("%i -> %i\n", 1, (*newDir->items)[1]);
    //Returns this behemoth of fuckery
    return newDir;
}

//TODO write more comments
directory_window * new_directory_window(char * dirPath, WINDOW * parentWindow){

    directory_window * newDirWin = malloc(sizeof(directory_window *));

    open_directory * newDir = open_new_directory(dirPath);

    newDirWin->directory = malloc(sizeof(open_directory *));
    newDirWin->directory = newDir;
    printw("1 -- %i\n", (*newDirWin->directory->items)[1]);
    newDirWin->parentWindow = parentWindow;
    newDirWin->window = new_window(parentWindow, false);

    return newDirWin;
}


WINDOW * new_window(WINDOW * parentWindow, bool outline){
    int h, w, y, x;
    WINDOW * returnWin;

    if(parentWindow == NULL){
        getmaxyx(stdscr, h, w);
        returnWin = newwin(h, w, 0, 0);
    }else{
        getmaxyx(parentWindow, y, x);
        returnWin = derwin(parentWindow, y - 2, x - 2, 1, 1);
        getparyx(returnWin, y, x);
    }

    if(outline){
        box(returnWin, 0, 0);
    }

    return returnWin;
}


