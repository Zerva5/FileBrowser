//
// Created by lmayall on 4/9/19.
//

#ifndef FILEBROWSER_DRAW_ATTRIBUTES_H
#define FILEBROWSER_DRAW_ATTRIBUTES_H

#ifndef DIRCOLOR
#define DIRCOLOR 1
#endif

#ifndef SELCOLOR
#define SELCOLOR 2
#endif

#ifndef LNKCOLOR
#define LNKCOLOR 3
#endif

void init_color_pairs();
void attributes_on(struct dirent *drawItem, WINDOW *drawWindow, bool selected);
void attributes_off(struct dirent *drawItem, WINDOW *drawWindow, bool selected);

#endif //FILEBROWSER_DRAW_ATTRIBUTES_H
