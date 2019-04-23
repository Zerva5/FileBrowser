//
// Created by lmayall on 4/4/19.
//

#ifndef FILEBROWSER_STRUCTS_H
#define FILEBROWSER_STRUCTS_H

#ifndef DIRWIN
#define DIRWIN 0
#endif

#ifndef NULLWIN
#define NULLWIN 1
#endif

#ifndef STATWIN
#define STATWIN 2
#endif

#include <ncurses.h>

// struct to contain all of the neccesary values to directory you are viewing
typedef struct{
    char *name;
    char *path;
    int *itemCount;
    int selectedIndex;
    //Pointer to an array of pointers to directory entity structs
    struct dirent *(*items)[];
    struct stat *(*itemsStat)[];
} open_directory;


typedef struct{
    WINDOW * window;
    open_directory * directory;
    int itemCount;
    int itemByteSize;
    char * type;

    char * itemName;
    char * itemOwner;
} status_window;

//typedef struct
//A windows that contains a directory
typedef struct{
    WINDOW * parentWindow;
    WINDOW * window;
    open_directory * directory;
    char * lastDirectory;
    bool draw;
    status_window * statusWindow;
} directory_window;

#endif //FILEBROWSER_STRUCTS_H
