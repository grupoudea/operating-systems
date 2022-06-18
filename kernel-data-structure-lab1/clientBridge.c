#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include "clientBridge.h"
#include "constants.h"

void write_message(int fd, unsigned long command, char * message){
    if (ioctl(fd, command, message) == -1){
        perror("Write message error at ioctl");
    }
}

void read_message(int fd, unsigned long command, char * message){
    if(ioctl(fd, command, message) == -1){
	perror("Read message error at ioctl");
    }else{
	printf("Copy the messsage from the kernel\n");
    }
}

void read_message_param(int fd, unsigned long command, int * value){
    if(ioctl(fd, command, value) == -1){
        perror("Read message param error at ioctl");
    }else{
        printf("Copy the messsage from the kernel\n");
    }
}

void write_int(int fd, unsigned long command, int * value){
    if (ioctl(fd, command, value) == -1){
        perror("Write int error at ioctl");
    }
}

void read_int(int fd, unsigned long command, int * value){
    if(ioctl(fd, command, value) == -1){
        perror("Read int error at ioctl");
    }else{
        printf("Copy the int from the kernel\n");
    }
}

void send_empty_command(int fd, unsigned long command){
    if(ioctl(fd, command) == -1){
       perror("Send command error at ioctl");
    }else{
        printf("Command OK to the kernel\n");
    }
}

int callModule(){
    const char *file_name = "/dev/bridgeOwn"; //used by ioctl
     int fd;

    fd = open(file_name, O_RDWR);
    if (fd == -1){
        perror("Bridge ioctl file open");
        return 2;
    }
}

int validateString(char* string){
    if(string == NULL){
        return 1;
    }else{
        return 0;
    }
}

void usageMenu(){
    printf("uso: clientBridge [--help] \n");
    printf("\n");
    printf("  %s <path-file> \t\t\t\t Lee las líneas de un archivo y las muestra en orden inverso. \n",ORDEN_INVERSO);
    printf("  --random <path-file> \t\t\t\t\t Baraja aleatoriamente las líneas de un archivo. \n");
    printf("  --validar-simetria <path-file> \t\t\t Lee un archivo .c y valida la simetría de los paréntesis y llaves. \n");
    printf("  --cola-prioridad <path-file> <ALTA|MEDIA|BAJA> \t Crea una cola con prioridad \n");
    printf("  --destruir-lista <path-file> \t\t\t\t Destruye una lista totalmente \n");
    printf("  --invertir <path-file> \t\t\t\t Invierte los nodos de una lista \n");
    printf("  --concatenar <path-file-1> <path-file-2> \t\t Concatena dos listas \n");
    printf("  --rotacion <n> <path-file> \t\t\t\t Rota la lista N veces a la derecha \n");
    printf("  --limpiar-lista <path-file> \t\t\t\t Limpia los valores idénticos de una lista. \n");
    printf("  --mayor <path-file> \t\t\t\t\t Busca el mayor en una lista. \n");
}

void chooseOption(char* option){
    if(strcmp(ORDEN_INVERSO, option) == 0){

    }else if(strcmp(RANDOM, option) == 0){

    }else if(strcmp(VALIDAR_SIMETRIA, option) == 0){

    }else if(strcmp(COLA_PRIORIDAD, option) == 0){

    }else if(strcmp(DESTRUIR_LISTA, option) == 0){

    }else if(strcmp(INVERTIR, option) == 0){

    }else if(strcmp(CONCATENAR, option) == 0){

    }else if(strcmp(ROTACION, option) == 0){

    }else if(strcmp(LIMPIAR_LISTA, option) == 0){

    }else if(strcmp(MAYOR, option) == 0){

    }else{
        usageMenu();
    }
}

int main(int argc, char *argv[]){
    char *option = "--help";
    if(validateString(argv[1]) == 0){
        option = argv[1];
    }
    chooseOption(option);
    return 0;
}
