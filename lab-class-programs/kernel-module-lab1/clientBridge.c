#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include "clientBridge.h"

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

int main(int argc, char *argv[]){

    const char *file_name = "/dev/bridgeOwn"; //used by ioctl
    int fd;

    fd = open(file_name, O_RDWR);
    if (fd == -1){
        perror("Bridge ioctl file open");
        return 2;
    }

    char message[] = "mensajeEspecifico";
    int value = 10;

    send_empty_command(fd, BRIDGE_CREATE_Q);                    //Create a queue
    write_message(fd, BRIDGE_W_HIGH_PRIOR_Q, message);		//Send message with high priority
    write_message(fd, BRIDGE_W_MIDDLE_PRIOR_Q, message);	//Send message with middle priority
    write_message(fd, BRIDGE_W_LOW_PRIOR_Q, message);		//Send message with low priority
    read_message(fd, BRIDGE_R_HIGH_PRIOR_Q, message);		//Read a message with high priority
    read_message(fd, BRIDGE_R_MIDDLE_PRIOR_Q, message);		//Read a message with middle priority
    read_message(fd, BRIDGE_R_LOW_PRIOR_Q, message);		//Read a message with low priority
    send_empty_command(fd, BRIDGE_STATE_Q);			//Get an int indicating the state of the queue
    send_empty_command(fd, BRIDGE_DESTROY_Q);			//Destroy a queue completely releasing memory (IMPORTANT!!)

    send_empty_command(fd, BRIDGE_CREATE_S);                   	//Create a stack
    write_message(fd, BRIDGE_W_S, message);			//Write a message in the stack
    read_message(fd, BRIDGE_R_S, message);			//Read a message from the stack
    send_empty_command(fd, BRIDGE_STATE_S);			//Get an int indicating the statte of the stack
    send_empty_command(fd, BRIDGE_DESTROY_S);			//Destroy a stack completely releasing the memory (IMPORTANT!!)

    send_empty_command(fd, BRIDGE_CREATE_L);                    //Create a list
    write_message(fd, BRIDGE_W_L, message);			//Write a message in the last position of a list
    read_message(fd, BRIDGE_R_L, message);			//Read a message of the last position of a list
    send_empty_command(fd, BRIDGE_INVERT_L);			//Invert the order of the elements of a list
    write_int(fd, BRIDGE_ROTATE_L, &value);			//Rotate a list value position to the right
    send_empty_command(fd, BRIDGE_CLEAN_L);			//Eliminate repeated elements in a list
    read_message(fd, BRIDGE_GREATER_VAL_L, message);		//Find the greater value of a list of strings
    printf("Value obtained: %s\n", message);
    send_empty_command(fd, BRIDGE_END_L);			//End a list (to be able to create another list) (Module limited to at most three list)
    send_empty_command(fd, BRIDGE_CONCAT_L);			//Concatenate two previous list in a third new one
    send_empty_command(fd, BRIDGE_STATE_L);			//Get an int indicating the state of a list
    send_empty_command(fd, BRIDGE_DESTROY_L);			//Destroy all the list of the module releasing memory (IMPORTANT!!)

    close (fd);
    return 0;
}
