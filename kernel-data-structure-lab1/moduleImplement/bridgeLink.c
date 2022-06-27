#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "bridgeIO.h"
#include "bridgeLink.h"
#include "../constants.h"

int callModule(){
    const char *file_name = "/dev/bridgeOwn"; //used by ioctl
    int fd;

    fd = open(file_name, O_RDWR);
    if (fd == -1){
        perror("Bridge ioctl file open");
        return 2;
    }
}

void validarSimetria(char **file,int numOfLines){
    char test[MAX_LENGTH_CHAR_BRIDGE];
    int fd = callModule();
    ioctl(fd, BRIDGE_W_S, "Hola este es un mensaje a la pila \n");
    ioctl(fd, BRIDGE_R_S, test);
    printf("%s\n",test);
}

void ordenInverso(char** arrayLines, int numOfLines){
    printf("\n######  Lineas del archivo orden original   ######\n");
    int fd = callModule();
    for (int i = 0; i < numOfLines; i++){
        printf("%s",arrayLines[i]);
        write_message(fd, BRIDGE_W_S, arrayLines[i]);
    }
    char fileLine[MAX_LENGTH_CHAR_BRIDGE];
    printf("\n######  Lineas del archivo orden inverso   ######\n");
    for (int i = 0; i < numOfLines; i++){
        write_message(fd, BRIDGE_R_S, fileLine);
        printf("%s",fileLine);
    }
}

void rotateToRight(char* numberRotations, char** arrayLines, int numOfLines){
    printf("ROTAR A LA DERECHA\n\n");
    int fd = callModule();
    printf("Number of rotations = %s \n\n", numberRotations);
    for (int i = 0; i < numOfLines; i++){
        printf("%s \n", arrayLines[i]);
        write_message(fd, BRIDGE_W_L, arrayLines[i]);
    }
    printf("\n\n LISTA ROTADA A LA DERECHA \n\n");
    char valor[MAX_LENGTH_CHAR_BRIDGE];
    write_message(fd, BRIDGE_ROTATE_L, numberRotations);
    for (int i = 0; i < numOfLines; i++){
        read_message(fd, BRIDGE_R_L, valor);
        printf("%s \n", valor);
    }
}

void concatTwoLists(char** firstList, int numFirstList, char** secondList, int numSecondList){
    int fd = callModule();
    char** lists[2] = {firstList, secondList};
    write_message(fd, BRIDGE_CONCAT_L, "");
}

void cleanList(char** arrayLines, int numOfLines){
    printf("LIMPIAR VALORES IDENTICOS DE LA LISTA\n\n");
    int fd = callModule();
    for (int i = 0; i < numOfLines; i++){
        printf("%s \n", arrayLines[i]);
        write_message(fd, BRIDGE_W_L, arrayLines[i]);
    }
    char* valorCantidadRegistros[MAX_LENGTH_CHAR_BRIDGE];
    read_message(fd, BRIDGE_CLEAN_L, valorCantidadRegistros);
    int cantidadRegistros = atoi(valorCantidadRegistros);
    printf("\n\n");
    printf("LISTA LIMPIA DE VALORES IDENTICOS\n\n");
    char valor[MAX_LENGTH_CHAR_BRIDGE];
    for (int i = 0; i < cantidadRegistros; i++){
        read_message(fd, BRIDGE_R_L, valor);
        printf("%s \n", valor);
    }
   
}