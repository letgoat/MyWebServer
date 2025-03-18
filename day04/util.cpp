#include "util.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

void errIf(bool condition, const char* message) {
    if(condition){
        perror(message);
        exit(EXIT_FAILURE);
    }
}
