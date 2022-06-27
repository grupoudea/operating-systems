#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "moduleImplement/bridgeLink.h"
#include "constants.h"
#include "utilsClientBridge.h"

char** argsv;
int argsc;

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

char* getNumberRotations(){
    if(argsc > 3){
        return argsv[3];
    }else{
        killClientBridge("Debe digitar la cantidad de rotaciones");
    }
}

char* getPathFileSecond(){
    char* pathfile = "/uk";
    if(argsc > 3){
        pathfile = argsv[3];
    }else{
        usageMenu();
        killClientBridge("no se ha encontrado el segundo pathfile");
    }
    return pathfile;
}

void chooseOption(char* option){

    char* pathfile = getPathFile();
    int numOfLines = 0;
    char** linesArray = readFile(pathfile, &numOfLines);

    if(strcmp(ORDEN_INVERSO, option) == 0){
        ordenInverso(linesArray, numOfLines);

    }else if(strcmp(RANDOM, option) == 0){
        randomLines(linesArray, numOfLines, pathfile);

    }else if(strcmp(VALIDAR_SIMETRIA, option) == 0){
        validarSimetria(linesArray, numOfLines);

    }else if(strcmp(COLA_PRIORIDAD, option) == 0){

    }else if(strcmp(DESTRUIR_LISTA, option) == 0){

    }else if(strcmp(INVERTIR, option) == 0){

    }else if(strcmp(CONCATENAR, option) == 0){
        char* secondPathfile = getPathFileSecond();
        int numOfLinesToConcat = 0;
        char** linesArrayToConcat = readFile(secondPathfile, &numOfLinesToConcat);
        concatTwoLists(linesArray, numOfLines, linesArrayToConcat, numOfLinesToConcat);
    }else if(strcmp(ROTACION, option) == 0){
        char* numberRotations = getNumberRotations();
        rotateToRight(numberRotations, linesArray, numOfLines);
    }else if(strcmp(LIMPIAR_LISTA, option) == 0){
        cleanList(linesArray, numOfLines);
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
