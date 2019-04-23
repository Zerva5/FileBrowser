//
// Created by lmayall on 4/8/19.
//
#include "error_handling.h"
#include<stdlib.h>

ERROR_STATE check_null(void * pointer){
    if(pointer == NULL){
        return NULL_ERROR;
    }else{
        return SUCCESS;
    }
    
}

