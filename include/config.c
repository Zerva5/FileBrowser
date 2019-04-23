//
// Created by lmayall on 4/4/19.
//
#include "config.h"
#include <stdbool.h>
#include <dirent.h>
//
const int DEFAULTSORT = 1;
const char HIDDENFILESPREFIX = '.';
const char EXCLUSIONS[1][1 + 1] = {'.'};

int DIRECTORIES[] = {DT_DIR, DT_LNK};
