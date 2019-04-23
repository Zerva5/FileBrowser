//
// Created by lmayall on 4/8/19.
//

#ifndef FILEBROWSER_ERROR_HANDLING_H
#define FILEBROWSER_ERROR_HANDLING_H

typedef enum {
    SUCCESS = 0,
    FAIL_UNKNOWN = 1,
    BAD_PATH = 2,
    NOT_DIRECTORY = 3,
    NULL_ERROR = 4,
    MALLOC_ERROR = 5,
    UNKNOWN_DIR_TYPE = 6} ERROR_STATE;

ERROR_STATE check_null(void *);

#endif //FILEBROWSER_ERROR_HANDLING_H
