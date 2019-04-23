cmake_minimum_required(VERSION 3.13.4)



set(CMAKE_C_STANDARD 11)


add_executable(FileBrowser main.c include/backend/statusbar_func.c include/backend/statusbar_func.h include/backend/structs.h include/backend/directory_func.c include/backend/directory_func.h include/backend/window_func.c include/backend/window_func.h include/config.h include/config.c include/backend/error_handling.h include/backend/error_handling.c include/input.c include/input.h include/display/draw_attributes.c include/display/draw_attributes.h)

target_link_libraries(FileBrowser ncurses)