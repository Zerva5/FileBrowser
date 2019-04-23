//
// Created by lmayall on 4/4/19.
//
#include "structs.h"
#include "directory_func.h"
#include "window_func.h"
#include <dirent.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include "error_handling.h"
#include "../display/draw_attributes.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "statusbar_func.h"


//Free the memory for a directory_window
ERROR_STATE close_directory_window(directory_window *oldWin){

    //Make sure it actually exists
    if(check_null(oldWin->directory) == NULL_ERROR){
        return NULL_ERROR;
    }

    //Free memory for the directory item
    close_directory(oldWin->directory);

    //delete the window
    delwin(oldWin->window);

    //free more memory, yay
    free(oldWin->lastDirectory);

    free(oldWin);

    return SUCCESS;
}

//TODO write more comments
ERROR_STATE new_directory_window(directory_window **newDirWin, char *dirPath, WINDOW *parentWindow) {
    // allocate memory for the new window
    *newDirWin = (directory_window *)malloc(sizeof(directory_window));

    // Every if is making sure whatever variable initialization happened was a success
    if(check_null(*newDirWin) == NULL_ERROR){
        return NULL_ERROR;
    }


    (*newDirWin)->lastDirectory = strdup(dirPath);

    if(check_null((*newDirWin)->lastDirectory) == NULL_ERROR){
        return NULL_ERROR;
    }

    open_new_directory(&(*newDirWin)->directory, dirPath);

    if(check_null((*newDirWin)->directory) == NULL_ERROR){
        return NULL_ERROR;
    }

    if(check_null((*newDirWin)->directory) == NULL_ERROR){
        return NULL_ERROR;
    }

    (*newDirWin)->parentWindow = parentWindow;

    if(check_null((*newDirWin)->parentWindow) == NULL_ERROR){
        return NULL_ERROR;
    }

    new_window(&(*newDirWin)->window, parentWindow, false, DIRWIN);

    if(check_null((*newDirWin)->window) == NULL_ERROR){
        return NULL_ERROR;
    }

    //Create a new status window for the directory
    new_status_window(&(*newDirWin)->statusWindow, (*newDirWin)->directory, (*newDirWin)->parentWindow);

    //Make sure it draws on the next loop
    (*newDirWin)->draw = true;

    // refreshed the status data for the status window
    refresh_status_data(&(*newDirWin)->statusWindow);

    //YAY
    return SUCCESS;

}



ERROR_STATE new_window(WINDOW **newWin, WINDOW *parentWindow, bool outline, int winType) {
    //Init window size and pos variables
    int h, w, y, x;


    *newWin = NULL;


    switch(winType){
        //Predefined constants ex NULLWIN
        case NULLWIN:
            getmaxyx(stdscr, h, w);
            *newWin = newwin(h, w, 0, 0);
            break;

        case STATWIN:
            getmaxyx(parentWindow, y, x);
            *newWin = derwin(parentWindow, y - 2, x - 2, 1, 1);
            break;

        case DIRWIN:
            getmaxyx(parentWindow, y, x);
            *newWin = derwin(parentWindow, y - 2, x - 2, 2, 1);
            getparyx(*newWin, y, x);
            break;
        default:
            return UNKNOWN_DIR_TYPE;
    }

    //Draw an outline, its dope
    if(outline){
        box(*newWin, 0, 0);
    }

    return SUCCESS;

}

void draw_directory_window(directory_window *dirWin) {
    //draw each line
    for (int i = 0; i < *dirWin->directory->itemCount; i = i + 1) {
        draw_directory_item(dirWin, i);
    }

    //Dont forget about this boi
    draw_status_window(dirWin->statusWindow);

}

void draw_directory_item(directory_window *dirWin, int itemNumber) {
    wmove(dirWin->window, itemNumber, 0);

    //Assume most of the items arent selected
    bool selected = false;

    if(dirWin->directory->selectedIndex == itemNumber){
        selected = true;
    }

    //print item with the right attributes
    attributes_on((*dirWin->directory->items)[itemNumber], dirWin->window, selected);
    wprintw(dirWin->window, " %i - %s \n", itemNumber, (*dirWin->directory->items)[itemNumber]->d_name);
    attributes_off((*dirWin->directory->items)[itemNumber], dirWin->window, selected);
}

void change_directory_selection(directory_window *dirWin, int newIndex) {
    //Not that hard cmon you dont need comments
    dirWin->directory->selectedIndex = newIndex;

    refresh_status_data(&dirWin->statusWindow);

    dirWin->draw = true;
}


ERROR_STATE change_window_directory_by_index(directory_window *changeWin, int openIndex) {

    // if its not a directory or a link dont do it. bad idea
    if((*changeWin->directory->items)[openIndex]->d_type != DT_DIR && (*changeWin->directory->items)[openIndex]->d_type != DT_LNK){
        return NOT_DIRECTORY;
    }

    char * newPath = NULL;
    item_path_by_index(&newPath, changeWin->directory, openIndex);


    change_window_directory(changeWin, newPath);

    free(newPath);
}


void change_to_last_directory(directory_window *changeWin) {
    change_window_directory(changeWin, changeWin->lastDirectory);
}


void change_window_directory(directory_window *changeWin, char *newPath) {

    //Change the directory open in the current window

    open_directory * newDir = NULL;

    //TODO Need better error handling for openening new dirs
    if(open_new_directory(&newDir, newPath) != SUCCESS){
        return;
    }

    //Set the last directory path
    free(changeWin->lastDirectory);
    changeWin->lastDirectory = strdup(changeWin->directory->path);

    //Close old dir
    close_directory(changeWin->directory);

    //Set to new dir
    changeWin->directory = newDir;

    //Update status stuff
    change_status_directory(&changeWin->statusWindow, changeWin->directory);


    changeWin->draw = true;
}
