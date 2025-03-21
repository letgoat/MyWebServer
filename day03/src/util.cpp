#include "util.h"
#include <stdlib.h>
#include <stdio.h>

void errif(bool condition, const char* errmessage){
    if(condition){
        perror(errmessage);
        exit(EXIT_FAILURE);
        }
    }
  