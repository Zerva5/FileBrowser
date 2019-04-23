//
// Created by lmayall on 4/4/19.
//
#ifndef FILEBROWSER_CONFIG_H
#define FILEBROWSER_CONFIG_H

#include <stdbool.h>

#ifndef SELECTION_UP
#define SELECTION_UP 'w'     //w
#endif

#ifndef SELECTION_DOWN
#define SELECTION_DOWN 's'   //s
#endif

#ifndef ENTER_DIR
#define ENTER_DIR 'e'    // e
#endif

#ifndef UP_DIR_LEVEL
#define UP_DIR_LEVEL 'q' // q
#endif

#ifndef LAST_DIR
#define LAST_DIR 'b'     // Q
#endif

#ifndef QUIT
#define QUIT 'q'
#endif


const bool SHOWHIDDEN;
//
const int DEFAULTSORT;
const char HIDDENFILESPREFIX;
const char EXCLUSIONS[1][1 + 1];

#endif