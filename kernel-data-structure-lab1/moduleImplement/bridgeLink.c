#include <stdio.h>
#include <sys/ioctl.h>
#include "bridgeIO.h"
#include "bridgeLink.h"

void validarSimetria(int fd,char **file,int numOfLines){
    char test[100];
    ioctl(fd, BRIDGE_W_S, "Hola este es un mensaje a la pila \n");
    ioctl(fd, BRIDGE_R_S, test);
    printf("%s\n",test);
}

