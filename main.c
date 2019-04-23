#include<stdio.h>
#include<dirent.h>
#include<stdlib.h>
#include<ncurses.h>
#include<string.h>
#include<limits.h>
#include "include/backend/structs.h"
#include "include/backend/directory_func.h"
#include "include/backend/window_func.h"
#include "include/config.h"
#include "include/input.h"
#include "include/display/draw_attributes.h"
#include "include/backend/statusbar_func.h"

//DOES IT WORK!!!:

int main(){
    bool active = true;

    //Ncurses boilder plate
    initscr();
    noecho();
    raw();
    curs_set(0);
    start_color();

    init_color_pairs();


    //initial opening directory
    char * targetDir = "/";

    WINDOW * rootWin;

    directory_window * currentDir = NULL;
    directory_window * active_windows[1];

    new_window(&rootWin, NULL, true, NULLWIN);
    new_directory_window(&currentDir, targetDir, rootWin);

    active_windows[0] = currentDir;

    refresh();
    wrefresh(rootWin);

    while(active){
      for (int i = 0; i < 1; i = i + 1) {

            if(active_windows[i]->draw == true){

                wclear(active_windows[i]->window);
                draw_directory_window(active_windows[i]);
                wrefresh(active_windows[i]->window);
                wrefresh(active_windows[i]->statusWindow->window);
                active_windows[i]->draw = false;
            }

        }
        handleInput(&active, currentDir);


    }
    close_statusbar(currentDir->statusWindow);
    close_directory_window(currentDir);
    delwin(rootWin);
    endwin();

    return 0;
}