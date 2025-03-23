#include "util.h"
#include <stdio.h>
#include <stdlib.h>

void errIf(bool condition, const char* errmessage){
    if(condition){
        perror(errmessage);
        exit(EXIT_FAILURE);
    }
}