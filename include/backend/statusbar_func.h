//
// Created by lmayall on 12/04/19.
//

#ifndef FILEBROWSER_STATUSBAR_FUNC_H
#define FILEBROWSER_STATUSBAR_FUNC_H

#include "structs.h"

void new_status_window(status_window **newStatus, open_directory *currentDir, WINDOW *parentWin);

void draw_status_window(status_window *);
void change_status_directory(status_window **, open_directory *);

void refresh_status_data(status_window **);
void convert_type_to_string(char ** typeString, int type);

void close_statusbar(status_window * oldStatus);
#endif //FILEBROWSER_STATUSBAR_FUNC_H
