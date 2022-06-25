#include <stdio.h>
#include <sys/ioctl.h>
#include "bridgeIO.h"
#include "bridgeLink.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

void validarSimetria(int fd,char **file,int numOfLines){
    char test[100];
    ioctl(fd, BRIDGE_W_S, "Hola este es un mensaje a la pila \n");
    ioctl(fd, BRIDGE_R_S, test);
    printf("%s\n",test);
}

void ordenInverso(int fd,char** arrayLines,int numOfLines){
    printf("\n######  Lineas del archivo orden original   ######\n");

    for (int i = 0; i < numOfLines; i++){
        printf("%s",arrayLines[i]);
        write_message(fd, BRIDGE_W_S, arrayLines[i]);
        sleep(2);
    }
    char test[100];
    printf("\n######  Lineas del archivo orden inverso   ######\n");
    for (int i = 0; i < numOfLines; i++){
        write_message(fd, BRIDGE_R_S, test);
        printf("%s",test);
    }

}

