#include <sys/ioctl.h>
#include <stdio.h>
#include "bridgeIO.h"

void write_message(int fd, unsigned long command, char * message){
    if (ioctl(fd, command, message) == -1){
        perror("Write message error at ioctl");
    }
}

void write_message_array(int fd, unsigned long command, char** message){
    if (ioctl(fd, command, message) == -1){
        perror("Write message error at ioctl");
    }
}

void read_message(int fd, unsigned long command, char * message){
    if(ioctl(fd, command, message) == -1){
	    perror("Read message error at ioctl");
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
    }
}

int send_empty_command(int fd, unsigned long command){
    int result = ioctl(fd, command);
    if( result == -1){
       perror("Send command error at ioctl");
    }else{
        //printf("Command OK to the kernel\n");
    }
    return result;
}

void write_several_messages(int fd){
    write_message(fd, BRIDGE_W_S, "Message 1");
    write_message(fd, BRIDGE_W_S, "Message 2");
    write_message(fd, BRIDGE_W_S, "Message 3");
}

void read_all_messages(int fd, unsigned long command){
	char message[100];
	while( send_empty_command(fd, BRIDGE_STATE_S) > 0){
	    read_message(fd, command, message);
	    printf("Message: %s\n", message);
	}
}