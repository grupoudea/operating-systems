#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
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

    for (int i = 0; i < numOfLines; i++){
        char *line= file[i];
        for (int j = 0; j < strlen(line); j++){
            if(line[j]=='{' | line[j]=='('){
                char tmp[2] = {line[j],'\0'} ;
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
    printf("\n---------  Original list  --------\n");
    for (int i = 0; i < numOfLines; i++){
        printf("(%d)%s \n",i,arrayLines[i]);
        write_message(fd, BRIDGE_W_L, arrayLines[i]);
    }
    printf("\n------------------------\n");
   
    send_empty_command(fd, BRIDGE_INVERT_L);
    printf("\n*****  list inverted , see details in Kernel  ********\n");

    printf("\n---------  New list  --------\n");
    char valor[MAX_LENGTH_CHAR_BRIDGE];
    for (int i = 0; i < numOfLines; i++){
        read_message(fd, BRIDGE_R_L, valor);
        printf("(%d) %s \n",i, valor);
    }
    printf("\n------------------------\n");
}

void greatherValue(char **arrayLines, int numOfLines){
    int fd = callModule();
    
    printf("\n---------  Original list  --------\n");
    for (int i = 0; i < numOfLines; i++){
        printf("(%d)%s  \n", i,arrayLines[i]);
        write_message(fd, BRIDGE_W_L, arrayLines[i]);
    }
    printf("\n------------------------\n");
    char  value [MAX_LENGTH_CHAR_BRIDGE];
    printf("\n---------  Greather value --------\n");
    write_message(fd, BRIDGE_GREATER_VAL_L, value);  
    printf("Greather value : %s \n",value);
    send_empty_command(fd,BRIDGE_DESTROY_L);     
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

void priorityQueue(char** arrayLines){
    int opcion, esc;
    char message[100], priority[256];
    long priorityInt = 0;
    printf("##### Inserting strings in priorities queues #####\n");
    do {
        do{
            printf("Insert message:\n\n");
            fflush( stdin );
            scanf("%s",&message);
        }while((strlen(message)<1));

        do{
            printf("Insert priority (Integer only):\n1. HIGH\n2. MIDDLE\n3. LOW\n");
            fflush( stdin );
            scanf("%s",&priority);
            priorityInt = strtol(priority, NULL, 10);
        }while(!isValidInteger(priority)||(priorityInt<1 || priorityInt>3));
        printf("We are going to store a message\n\n");
        storeMessageInPriorityQueue(message, priorityInt);

        do{
            printf("Insert more message?\n1. Yes\n2. No\n\n");
            scanf("%d",&esc);
        }while(esc < 1 || esc >2);

    }while(esc!=2);

    printf("We're going to show you the message in orden of priority:\n\n");
    readMessageWithPriority();
}

char* getPriorityText(int priority){

    if(priority==1){
        return "HIGH";
    }else if(priority == 2){
        return "MIDDLE";
    }else if(priority == 3){
        return "LOW";
    }else{
        return "UNKNOWN";
    }
}

int isValidInteger(char num[]){
    for (int i = 0; i < strlen(num); i++)
    {
        if(!isdigit(num[i]) ) {
            printf("Insert a valid number\n\n");
            return 0;
        }
    }
    return 1;
}

void storeMessageInPriorityQueue(char* message, int levelPriority){
    int fd = callModule();
    switch (levelPriority){
        case 1:
            write_message(fd, BRIDGE_W_HIGH_PRIOR_Q, message);
            break;
        case 2:
            write_message(fd, BRIDGE_W_MIDDLE_PRIOR_Q, message);
            break;
        case 3:
            write_message(fd, BRIDGE_W_LOW_PRIOR_Q, message);
            break;
        default:
            printf("We could store this in the high PQ, but we won't\n");
            break;
    }
}

void readMessageWithPriority(){
    int fd = callModule();
    printf("#### HIGH PRIORITY\n");
    read_all_messages_specify(fd, BRIDGE_STATE_HP_Q,BRIDGE_R_HIGH_PRIOR_Q);

    printf("\n#### MIDDLE PRIORITY\n");
    read_all_messages_specify(fd, BRIDGE_STATE_MP_Q,BRIDGE_R_MIDDLE_PRIOR_Q);

    printf("\n#### LOW PRIORITY\n");
    read_all_messages_specify(fd, BRIDGE_STATE_LP_Q,BRIDGE_R_LOW_PRIOR_Q);
}