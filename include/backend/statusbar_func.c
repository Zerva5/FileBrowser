//
// Created by lmayall on 12/04/19.
//

#include "statusbar_func.h"
#include <dirent.h>
#include "directory_func.h"
#include "structs.h"
#include <stdlib.h>
#include "window_func.h"
#include <sys/stat.h>
#include <pwd.h>
#include <string.h>

const static int MAXNAMECHAR;
const static int MAXTYPELENGTH;

void new_status_window(status_window **newStatus, open_directory *currentDir, WINDOW *parentWin) {
    // Alocate memory for it
    *newStatus = (status_window *)malloc(sizeof(status_window));

    //Initialize all of the memory and variable
    (*newStatus)->directory = currentDir;

    (*newStatus)->window = NULL;

    (*newStatus)->itemCount = 0;

    (*newStatus)->itemOwner = strdup("NULL");

    (*newStatus)->type = strdup("NULL");

    (*newStatus)->itemByteSize = 0;

    (*newStatus)->itemName = "NULL";

    //Create a new window
    new_window(&(*newStatus)->window, parentWin, false, STATWIN);
}

void draw_status_window(status_window * statusWindow){
    wmove(statusWindow->window, 0, 0);

    //Its only one line so dont have to clear entire window
    wclrtoeol(statusWindow->window);

    //Print the status info
    wprintw(statusWindow->window, "%s  %s  %i / %i  %s",
            statusWindow->itemName,
            statusWindow->type,
            statusWindow->itemCount,
            statusWindow->itemByteSize,
            statusWindow->itemOwner);

}

void change_status_directory(status_window **statusWindow, open_directory *newDir){

    (*statusWindow)->directory = newDir;

    refresh_status_data(statusWindow);

}

void refresh_status_data(status_window ** statusWindow){

    // Create structs to be used for stat
    struct stat statbuffer;
    struct passwd * pwdBuffer;

    //initialize the temppath
    char * tempPath = NULL;

    //Find the path of the selected item by its index
    item_path_by_index(&tempPath, (*statusWindow)->directory, (*statusWindow)->directory->selectedIndex);

    // get stat info and get item owner
    stat(tempPath, &statbuffer);
    pwdBuffer = getpwuid(statbuffer.st_uid);
    (*statusWindow)->itemOwner = strdup(pwdBuffer->pw_name);

    // Get the itemcount inside the directory
    count_directory_items(&(*statusWindow)->itemCount, tempPath);

    //Convert the type integer to a string
    convert_type_to_string(&(*statusWindow)->type, (*(*statusWindow)->directory->items)[(*statusWindow)->directory->selectedIndex]->d_type);

    // Get the itemname
    (*statusWindow)->itemName = (*(*statusWindow)->directory->items)[(*statusWindow)->directory->selectedIndex]->d_name;

    //Set the byte size of all of the items in a directory
    if((*(*statusWindow)->directory->itemsStat)[1] != NULL){
        (*statusWindow)->itemByteSize = 100;
    }else{
        (*statusWindow)->itemByteSize = 1;
    }

}

void convert_type_to_string(char ** typeString, int type){
    // Convert the dt_type int to a string
    switch(type){
        case DT_LNK:
            *typeString = strdup("LINK");
            break;
        case DT_DIR:
            *typeString = strdup("DIR");
            break;
        case DT_REG:
            *typeString = strdup("ITEM");
            break;
        default:
            sprintf(*typeString, "%i", type);
//            *typeString = strdup(type);
            break;
    }

}

void close_statusbar(status_window * oldStatus){

    //Basic memory cleanup

    free(oldStatus->type);

    free(oldStatus->itemOwner);

    // cant forget to delete the window
    delwin(oldStatus->window);

    free(oldStatus);

}


// Name: $itemname    Type: #itemtype    Count: $itemcount/$size    Owner: $itemOwner