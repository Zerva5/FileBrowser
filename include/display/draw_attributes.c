//
// Created by lmayall on 4/9/19.
//
#include <dirent.h>
#include <ncurses.h>
#include "draw_attributes.h"

const int DIR_fg = COLOR_WHITE;
const int DIR_bg = COLOR_BLUE;

const int LNK_fg = COLOR_WHITE;
const int LNK_bg = COLOR_CYAN;

const int SEL_fg = COLOR_RED;
const int SEL_bg = COLOR_WHITE;

void init_color_pairs(){
    init_color(COLOR_WHITE, 255, 255, 255);
    init_color(COLOR_BLACK, 0, 0, 0);

    init_color(COLOR_BLUE, 0, 0, 255);
    init_color(COLOR_RED, 255, 0, 0);
    init_color(COLOR_GREEN, 0, 255, 0);

    init_color(COLOR_CYAN, 0, 255, 255);
    init_color(COLOR_MAGENTA, 255, 0, 255);
    init_color(COLOR_YELLOW, 255, 255, 0);

    init_pair(DIRCOLOR, DIR_fg, DIR_bg);
    init_pair(SELCOLOR, SEL_fg, SEL_bg);
    init_pair(LNKCOLOR, LNK_fg, LNK_bg);
}

void attributes_off(struct dirent *drawItem, WINDOW *drawWindow, bool selected) {

    wattroff(drawWindow, A_BOLD);
    if(selected){
        wattroff(drawWindow, COLOR_PAIR(SELCOLOR));
    }else if(drawItem->d_type == DT_DIR){
        wattroff(drawWindow, COLOR_PAIR(DIRCOLOR));
    }else if(drawItem->d_type == DT_LNK){
        wattroff(drawWindow, COLOR_PAIR(LNKCOLOR));
    }
}

void attributes_on(struct dirent *drawItem, WINDOW *drawWindow, bool selected) {

    wattron(drawWindow, A_BOLD);
    if(selected){
        wattron(drawWindow, COLOR_PAIR(SELCOLOR));
    }else if(drawItem->d_type == DT_DIR){
        wattron(drawWindow, COLOR_PAIR(DIRCOLOR));
    }else if(drawItem->d_type == DT_LNK){
        wattron(drawWindow, COLOR_PAIR(LNKCOLOR));
    }
}