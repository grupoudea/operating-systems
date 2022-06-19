#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include "clientBridge.h"
#include "constants.h"
#include "utilsClientBridge.h"

char** argsv;
int argsc;

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

void usageMenu(){
    printf("uso: clientBridge [--help] \n");
    printf("\n");
    printf("  %s <path-file> \t\t\t\t Lee las líneas de un archivo y las muestra en orden inverso. \n", ORDEN_INVERSO);
    printf("  %s <path-file> \t\t\t\t\t Baraja aleatoriamente las líneas de un archivo. \n", RANDOM);
    printf("  %s <path-file> \t\t\t Lee un archivo .c y valida la simetría de los paréntesis y llaves. \n", VALIDAR_SIMETRIA);
    printf("  %s <path-file> <ALTA|MEDIA|BAJA> \t Crea una cola con prioridad \n", COLA_PRIORIDAD);
    printf("  %s <path-file> \t\t\t\t Destruye una lista totalmente \n", DESTRUIR_LISTA);
    printf("  %s <path-file> \t\t\t\t Invierte los nodos de una lista \n", INVERTIR);
    printf("  %s <path-file-1> <path-file-2> \t\t Concatena dos listas \n", CONCATENAR);
    printf("  %s <path-file> <n> \t\t\t\t Rota la lista N veces a la derecha \n", ROTACION);
    printf("  %s <path-file> \t\t\t\t Limpia los valores idénticos de una lista. \n", LIMPIAR_LISTA);
    printf("  %s <path-file> \t\t\t\t\t Busca el mayor en una lista. \n", MAYOR);
}

char* getPathFile(){
    char* pathfile = "/uk";
    if(argsc>2){
        pathfile = argsv[2];
    }else{
        usageMenu();
        killClientBridge("no se ha encontrado un pathfile");
    }
    return pathfile;
}

void chooseOption(char* option){
    char* pathfile = getPathFile();
    char** linesArray;
    readFile(pathfile, linesArray);

    for ( int i = 0; i < 7; i++)
    {
        printf("valor regreso= %s",linesArray[i]);
    }

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

char* getOption(){
    char *option = "--help";
    if(argsc>1){
      option = argsv[1];
    }
    return option;
}

int main(int argc, char *argv[]){
    argsc = argc;
    argsv = argv;

    chooseOption(getOption());
    return 0;
}
