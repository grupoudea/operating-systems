#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
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
    printf("numero de lineas : %d \n",numOfLines);
    char line[MAX_LENGTH_CHAR_BRIDGE];
    int fd = callModule();
    // int value = send_empty_command(fd,BRIDGE_CREATE_S);
    // printf("valores en la pila : %d",value);
    // int value = send_empty_command(fd,BRIDGE_DESTROY_S);
    //  printf("valores en la pila : %d",value);
    for (int i = 0; i < numOfLines; i++){
        char *line= file[i];
        for (int j = 0; j < strlen(line); j++){
            if(line[j]=='{' | line[j]=='('){
                char tmp[2] = {line[j],'\0'} ;
                //printf("%s",tmp);
                write_message(fd, BRIDGE_W_S, tmp);
            }
            else if(line[j]=='}' | line[j]==')'){
                if(send_empty_command(fd,BRIDGE_STATE_S)<=0){
                    printf(" El archivo con codigo C no es simetrico \n");
                    return;
                }
                char c[2];
                write_message(fd, BRIDGE_R_S, c);
                char symbol[2];
                if(line[j]=='}') {
                    symbol[0] = '{';
                }else{
                    symbol[0]='(';
                } 
                if(c[0]!=symbol[0]){
                    printf(" El archivo con codigo C no es simetrico \n");
                    send_empty_command(fd, BRIDGE_DESTROY_S);
                    return;
                }
            }
        }
    }
    // //list empty validation
    int value = send_empty_command(fd,BRIDGE_STATE_S);
    if(value>0){
        printf("El archivo con codigo C NO es simetrico \n");
    }
    else{
        printf("El archivo con codigo C es simetrico \n");
    }
    send_empty_command(fd, BRIDGE_DESTROY_S);
}

void ordenInverso(char** arrayLines, int numOfLines){
    printf("\n######  Lineas del archivo orden original   ######\n");
    int fd = callModule();
    for (int i = 0; i < numOfLines; i++){
        printf("%s \n",arrayLines[i]);
        write_message(fd, BRIDGE_W_S, arrayLines[i]);
    }
    char fileLine[MAX_LENGTH_CHAR_BRIDGE];
    printf("\n######  Lineas del archivo orden inverso   ######\n");
    for (int i = 0; i < numOfLines; i++){
        write_message(fd, BRIDGE_R_S, fileLine);
        printf("%s \n",fileLine);
    }
}

int randomNumber(int maxNumber, int seed){
    int time1 = time(NULL)-((seed+1)*RAND_MAX);
    srand(time1);// numero aleatorio entre 0 y maxNumber
    int numberGenerate = (rand() % maxNumber);
    return numberGenerate;
}

void invertirLista(char **arrayLines, int numOfLines){
    int fd = callModule();
    printf("\n######  Creating list  ######\n");
    for (int i = 0; i < numOfLines; i++){
        //printf("%s \n",arrayLines[i]);
        write_message(fd, BRIDGE_W_L, arrayLines[i]);
    }
    printf("\n######  list created  ######\n");
   
    send_empty_command(fd, BRIDGE_INVERT_L);
    printf("\n######  list inversed , see details in Kernel  ######\n");
  
    send_empty_command(fd, BRIDGE_DESTROY_L);
    printf("\n######  list destroyed  ######\n");
}


void randomLines(char** arrayLines, const int numOfLines, char* fileName){
    int fd = callModule();

    for (int i = 0; i < numOfLines; i++){
        write_message(fd, BRIDGE_W_L, arrayLines[i]);
    }

    if(numOfLines>=2){
        int maxRandom = numOfLines;
        if(numOfLines==2){
            maxRandom = 2;
        }
        for (int i = 0; i < maxRandom; i++){
            int randomNumberInt = randomNumber(maxRandom,(i));
            char randomNumberChar[20];
            sprintf(randomNumberChar, "%d", randomNumberInt);
            write_message(fd, BRIDGE_RANDOM_L, randomNumberChar);
        }
    }

    char fileLine[MAX_LENGTH_CHAR_BRIDGE];
    for (int i = 0; i < numOfLines; i++){
        write_message(fd, BRIDGE_R_L, fileLine);
        printf("Random Lines: %s\n",fileLine);
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

void destroyList(char** arrayLines, int numOfLines){
    printf("DESTRUYE UNA LISTA COMPLETAMENTE\n\n");
    int fd = callModule();
    for (int i = 0; i < numOfLines; i++){
        printf("%s \n", arrayLines[i]);
        write_message(fd, BRIDGE_W_L, arrayLines[i]);
    }
    char valor[MAX_LENGTH_CHAR_BRIDGE];
    read_message(fd, BRIDGE_DESTROY_L, "");
    for (int i = 0; i < numOfLines; i++){
        read_message(fd, BRIDGE_R_L, valor);
        if(strcmp(valor, "void") == 0){
            printf("LISTA DESTRIUDA\n\n");
            break;
        }
        printf("%s \n", valor);
    }
}