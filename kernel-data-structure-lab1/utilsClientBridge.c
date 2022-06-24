#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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


char** readFile(char* fileName,int *numOfLines){
    FILE* input_file = fopen(fileName, "r");
    if (!input_file){
        exit(EXIT_FAILURE);
    }
    int lines = 0;
    numberOfLines(input_file, &lines);
    printf("cantidad lineas: %d\n",lines);
    rewind(input_file);
    char** arrayChars = malloc(sizeof(char*) * lines);
    char* contents = NULL;
    size_t len = 0;
    int i = 0;
    while (getline(&contents, &len, input_file) != -1){
        int cm = stringLength(contents);
        char* newChars = (char*)malloc(cm);
        strcpy(newChars, contents);
        arrayChars[i] = (char*)malloc(cm);
        arrayChars[i] = newChars;
        // printf("%s\n", newChars);
        i++;

    }
    // for (int i = 0; i < lines; i++)
    // {
    //     printf("vector = %s\n",arrayChars[i]);
    // }
    fclose(input_file);
    free(contents);
    *numOfLines=lines;
     return arrayChars;
}

int stringLength(char * arrayChar){
    int i = 0;
    while (arrayChar[i]!='\0')
    {
        i++;
    }
    return i;
}

int numberOfLines(FILE* file, int* size){
    char character;
    int i = 0;
    while(1){
        character = fgetc(file);
        if(character == '\n'){
            i++;
        }
        if(character == EOF){  //Si el caracter es end of file imprimimos el contador y salimos del while
            i++;
            *size = i;
            // printf("%d\n",i);  //El número de lineas
            break;
        }
    }
}

