//
// Created by lmayall on 4/4/19.
//

#ifndef FILEBROWSER_DIRECTORY_FUNC_H
#define FILEBROWSER_DIRECTORY_FUNC_H

#include "structs.h"
#include "error_handling.h"

//Function declarations
void sort_directory_items(open_directory * sortDir);
ERROR_STATE count_directory_items(int *itemCount, char *dirName);
void read_directory(open_directory *openDir);
struct dirent * item_by_index(open_directory *, int);
ERROR_STATE open_new_directory(open_directory **newDir, char *dirPath);
ERROR_STATE close_directory(open_directory *oldDir);
int change_index_selection(open_directory *openDir, int newIndex);

void item_path_by_index(char **newPath, open_directory *openDir, int index);

#endif //FILEBROWSER_DIRECTORY_FUNC_H
