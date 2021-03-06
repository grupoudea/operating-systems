#ifndef CLIENT_BRIDGE_H
#define CLIENT_BRIDGE_H
#include <linux/ioctl.h>

// cmd ‘BRIDGE_DATA_VAR’ to send a message to the module

#define BRIDGE_CREATE_Q _IO('p', 1) 			//Create a queue
#define BRIDGE_W_HIGH_PRIOR_Q _IOW('p', 2, char *)
#define BRIDGE_W_MIDDLE_PRIOR_Q _IOW('p', 3, char *)
#define BRIDGE_W_LOW_PRIOR_Q _IOW('p', 4, char *)
#define BRIDGE_R_HIGH_PRIOR_Q _IOR('p', 5, char *)
#define BRIDGE_R_MIDDLE_PRIOR_Q _IOR('p', 6, char *)
#define BRIDGE_R_LOW_PRIOR_Q _IOR('p', 7, char *)
#define BRIDGE_STATE_Q _IO('p', 8)
#define BRIDGE_STATE_HP_Q _IO('p', 27)
#define BRIDGE_STATE_MP_Q _IO('p', 28)
#define BRIDGE_STATE_LP_Q _IO('p', 29)
#define BRIDGE_DESTROY_Q _IO('p', 9)

#define BRIDGE_CREATE_S _IO('p', 10) 			//Create a stack
#define BRIDGE_W_S _IOW('p', 11, char *)
#define BRIDGE_R_S _IOR('p', 12, char *)
#define BRIDGE_STATE_S _IO('p', 13)
#define BRIDGE_DESTROY_S _IO('p', 14)

#define BRIDGE_CREATE_L _IO('p', 15)			//Create a list
#define BRIDGE_W_L _IOW('p', 16, char *)
#define BRIDGE_R_L _IOR('p', 17, char *)
#define BRIDGE_INVERT_L _IO('p', 18)
#define BRIDGE_ROTATE_L _IOW('p', 19, int *)
#define BRIDGE_CLEAN_L _IO('p', 20)
#define BRIDGE_GREATER_VAL_L _IOR('p', 21, char *)
#define BRIDGE_END_L _IO('p', 22)
#define BRIDGE_CONCAT_L _IO('p', 23)
#define BRIDGE_STATE_L _IO('p', 24)
#define BRIDGE_DESTROY_L _IO('p', 25)
#define BRIDGE_RANDOM_L _IO('p', 26)


#endif


void write_message(int fd, unsigned long command, char * message);
void read_message(int fd, unsigned long command, char * message);
void read_message_param(int fd, unsigned long command, int * value);

void write_int(int fd, unsigned long command, int * value);
void read_int(int fd, unsigned long command, int * value);

int send_empty_command(int fd, unsigned long command);
void write_several_messages(int fd);
void read_all_messages(int fd, unsigned long command);
void read_all_messages_specify(int fd, unsigned long command, unsigned long queue);