#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "bridgeIO.h"
#include "bridgeLink.h"

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
    char test[100];
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
    char fileLine[100];
    printf("\n######  Lineas del archivo orden inverso   ######\n");
    for (int i = 0; i < numOfLines; i++){
        write_message(fd, BRIDGE_R_S, fileLine);
        printf("%s",fileLine);
    }
}

void rotateToRight(char* numberRotations){
    int fd = callModule();
    printf("%s \n\n", numberRotations);
    write_message(fd, BRIDGE_ROTATE_L, numberRotations);
}