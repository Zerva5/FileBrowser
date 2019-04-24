#include <dirent.h>
#include "directory_func.h"
#include "structs.h"
#include <string.h>
#include <stdlib.h>
#include "error_handling.h"
#include <sys/stat.h>



static const int ITEMCOUNTOFFSET = -1;
static const int INITIALITEMCOUNT = 0;


//Sort the directory alphabetically, dont fully understand this yet. Thanks stackoverflow
void sort_directory_items(open_directory *sortDir) {

    struct stat * statTemp;
    struct dirent * temp;

    int i,j;

    for(i = 0; i < *sortDir->itemCount; i = i + 1){

        for (j = i + 1; j < *sortDir->itemCount; j = j + 1) {

            if(strcmp((*sortDir->items)[i]->d_name, (*sortDir->items)[j]->d_name) > 0){

                temp = (*sortDir->items)[i];
                statTemp = (*sortDir->itemsStat)[i];

                (*sortDir->items)[i] = (*sortDir->items)[j];
                (*sortDir->itemsStat)[i] = (*sortDir->itemsStat)[j];

                (*sortDir->items)[j] = temp;
                (*sortDir->itemsStat)[j] = statTemp;
            }
        }
    }
}

//Counts all of the items in a directory, inificient, when opening a directory you have to go through it twice, once to
// count and once to retrieve items :(
ERROR_STATE count_directory_items(int *itemCount, char *dirName) {
    //Initialize the dir, and a item struct
    DIR * currentDir = NULL;
    struct dirent *ent = NULL;

    //Open the directory
    currentDir = opendir(dirName);

    //Make sure the directory is able to be opened
    if(check_null(currentDir) == NULL_ERROR){
        return NULL_ERROR;
    }

    //Set itemcount to an initial value to offset for "." which points to itself
    *itemCount = INITIALITEMCOUNT;

    //Read all directories and add em up
    while(1){
        ent = readdir(currentDir);
        if(ent != NULL){
            *itemCount = *itemCount + 1;
        }else{
            break;
        }
    }

    //Cleanup and set the itemcount
    closedir(currentDir);
    *itemCount = *itemCount + ITEMCOUNTOFFSET;
}


//refreshes the items in a directory
void read_directory(open_directory *openDir){
    int i;
    DIR * currentDir = NULL;

    struct dirent * newEnt = NULL;
    currentDir = opendir(openDir->path);

    for (i = 0; i < *openDir->itemCount; i = i + 1) {
        newEnt = readdir(currentDir);
        //replace with list of exclusions
        if(strcmp(".", newEnt->d_name) == 0){
            newEnt = readdir(currentDir);
        }

        *(*openDir->items)[i] = *newEnt;
        char * tempPath = NULL;
        item_path_by_index(&tempPath, openDir, i);

        stat(tempPath, (*openDir->itemsStat)[i]);
        free(tempPath);
    }

    closedir(currentDir);
}

//Returns the item at an index, nothing major
struct dirent * item_by_index(open_directory * currentDir, int index){
    return (*currentDir->items)[index];
}

//Opens a new directory and assigns it proper values, fuck me this was anoyying
ERROR_STATE open_new_directory(open_directory **newDir, char *dirPath) {

    //Make a new open_directory and assign it memory
    *newDir = (open_directory *)malloc(sizeof(open_directory));

        // Sets the current (selected) index to 0 and stores pointer in struct
    (*newDir)->selectedIndex = 0;

    //gets the items inside the directory, stores pointer in struct
    (*newDir)->itemCount = (int *)malloc(sizeof(int));

    if(count_directory_items((*newDir)->itemCount, dirPath) == NULL_ERROR){
        close_directory(*newDir);
        return NOT_DIRECTORY;
    }

    //Stores copy to new pointer of dirPath in struct
    (*newDir)->path = strdup(dirPath);

    (*newDir)->name = strdup("name");

    // allocates memory for the item array, to the size of itemCount, if itemCount changes have to close and then reopen directory
    (*newDir)->items = malloc(sizeof(struct dirent *) * (*(*newDir)->itemCount));

    (*newDir)->itemsStat = malloc(sizeof(struct stat *) * (*(*newDir)->itemCount));


    for (int i = 0; i < *(*newDir)->itemCount; i = i + 1) {
        (*(*newDir)->items)[i] = (struct dirent *)malloc(sizeof(struct dirent));
        (*(*newDir)->itemsStat)[i] = (struct stat *)malloc(sizeof(struct stat));
    }

    // itterates through directory and assigns the array values, takes info from the struct not a passthrough
    read_directory(*newDir);

    //Need to sort stat too
    sort_directory_items(*newDir);

    return SUCCESS;
}

//Free the memory for the directory and its items
ERROR_STATE close_directory(open_directory *oldDir){

    for (int i = 0; i < *oldDir->itemCount; ++i) {
        if((*oldDir->items)[i] != NULL){
            free((*oldDir->items)[i]);
        }
    }

    for (int i = 0; i < *oldDir->itemCount; ++i) {
        if((*oldDir->itemsStat)[i] != NULL){
            free((*oldDir->itemsStat)[i]);
        }
    }

    if(oldDir->items != NULL){
        free(oldDir->items);
    }

    if(oldDir->itemsStat != NULL){
        free(oldDir->itemsStat);
    }

    if(oldDir->path != NULL){
        free(oldDir->path);
    }

//    if(oldDir->)

    if(oldDir->name != NULL){
        free(oldDir->name);
    }

    if(oldDir->itemCount != NULL){
        free(oldDir->itemCount);
    }

    free(oldDir);

    return SUCCESS;

}

int change_index_selection(open_directory *openDir, int newIndex) {

    if(newIndex < 0){
        return openDir->selectedIndex;
    }else if(newIndex >= *openDir->itemCount){
        return openDir->selectedIndex;
    }else{
        return newIndex;
    }

}


void item_path_by_index(char **newPath, open_directory *openDir, int index) {


    char tempPath[4096];
    //Memset does not allocate memory, simple zeroes it out.
    memset(tempPath, 0, sizeof(tempPath));

    strcat(tempPath, openDir->path);

    if(tempPath[strlen(tempPath)-1] != '/'){
        strcat(tempPath, "/");
        strcat(tempPath, (*openDir->items)[index]->d_name);
    }else{
        strcat(tempPath, (*openDir->items)[index]->d_name);
    }

    *newPath = strdup(tempPath);
}
