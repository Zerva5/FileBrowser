//
// Created by lmayall on 4/4/19.
//

#ifndef FILEBROWSER_WINDOW_FUNC_H
#define FILEBROWSER_WINDOW_FUNC_H

#include "error_handling.h"

ERROR_STATE close_directory_window(directory_window *oldWin);
ERROR_STATE new_directory_window(directory_window **newDirWin, char *dirPath, WINDOW *parentWindow);
ERROR_STATE new_window(WINDOW **newWin, WINDOW *parentWindow, bool outline, int winType);

void draw_directory_window(directory_window *dirWin);
void draw_directory_item(directory_window *dirWin, int itemNumber);
void change_directory_selection(directory_window *dirWin, int newIndex);


void change_to_last_directory(directory_window * changeWin);
ERROR_STATE change_window_directory_by_index(directory_window *changeWin, int openIndex);
void change_window_directory(directory_window *changeWin, char *newPath);
#endif //FILEBROWSER_WINDOW_FUNC_H
