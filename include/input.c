//
// Created by lmayall on 4/9/19.
////
//
#include <ncurses.h>
#include "backend/directory_func.h"
#include "backend/window_func.h"
#include "backend/structs.h"
#include "config.h"


//INPUT THINGS
void handleInput(bool * active, directory_window * currentDir){

    const int input = getch();

    switch(input){
        case QUIT:
            *active = false;
            break;
        case SELECTION_UP:
            if(change_index_selection(currentDir->directory, currentDir->directory->selectedIndex - 1) != currentDir->directory->selectedIndex){
                change_directory_selection(currentDir, currentDir->directory->selectedIndex - 1);
            }
            wrefresh(currentDir->window);
            break;
        case SELECTION_DOWN:
            if(change_index_selection(currentDir->directory, currentDir->directory->selectedIndex + 1) != currentDir->directory->selectedIndex){
                change_directory_selection(currentDir, currentDir->directory->selectedIndex + 1);
            }
            wrefresh(currentDir->window);
            break;
        case ENTER_DIR:
            change_window_directory_by_index(currentDir, currentDir->directory->selectedIndex);
            break;
        case LAST_DIR:
            change_to_last_directory(currentDir);
            break;

        default:
            break;
    }

}
