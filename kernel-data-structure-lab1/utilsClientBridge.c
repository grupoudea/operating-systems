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
    printf("ERROR: %s \n\n", message);
    exit(-1);
}

char** readFile(char* fileName, int* numOfLines){
    FILE* inputFile = fopen(fileName, "r");
    if (!inputFile){
        exit(EXIT_FAILURE);
    }
    int lines = 0;
    numberOfLines(inputFile, &lines);
    rewind(inputFile);
    char** arrayChars = malloc(sizeof(char*) * lines);
    char* contents = NULL;
    size_t len = 0;
    int i = 0;
    while (getline(&contents, &len, inputFile) != -1){
        int lineLength = stringLength(contents);
        char* newChars = (char*)malloc(lineLength);
        int len = strlen(contents);
        if(contents[len - 1] == '\n'){
            contents[len - 1] = '\0';
        }
        strcpy(newChars, contents);
        arrayChars[i] = (char*)malloc(lineLength);
        arrayChars[i] = newChars;
        i++;
    }

    fclose(inputFile);
    free(contents);
    *numOfLines=lines;
    return arrayChars;
}

int stringLength(char * arrayChar){
    int i = 0;
    while (arrayChar[i]!='\0'){
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
        if(character == EOF){
            // i++;
            *size = i;
            break;
        }
    }
}

