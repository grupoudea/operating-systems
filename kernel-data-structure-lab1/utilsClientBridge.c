#include <stdio.h>
#include <stdlib.h>
#include "utilsClientBridge.h"

int validateString(char* string){
    if(string == NULL){
        return 1;
    }else{
        return 0;
    }
}

void killClientBridge(char* message){
    printf("ERROR: %s n\n", message);
    exit(-1);
}