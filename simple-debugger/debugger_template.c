#include <stdio.h>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/syscall.h>
#include <errno.h>
#include <sys/personality.h>
#include "linenoise.h"

struct breakpoint{
    uint64_t addr;
    uint8_t prev_opcode;
    uint8_t active;
};

struct debugee{
    char* name;
    pid_t pid;
};

struct reg_descriptor {
    int dwarf_r;
    char* name;
};

struct option_menu{
    int code;
    char* name;
};

const struct option_menu options_menu[]={
    {0,"break"},
    {1,"continue"},
    {2,"register"},
    {3,"next"}
};
const int n_registers = 27;

const struct reg_descriptor g_register_descriptors[] = {
    { 0, "r15" },
    { 1, "r14" },
    { 2, "r13" },
    { 3, "r12" },
    { 4, "rbp" },
    { 5, "rbx" },
    { 6, "r11" },
    { 7, "r10" },
    { 8, "r9" },
    { 9, "r8" },
    { 10, "rax" },
    { 11, "rcx" },
    { 12, "rdx" },
    { 13, "rsi" },
    { 14, "rdi" },
    { 15, "orig_rax" },
    { 16, "rip" },
    { 17, "cs" },
    { 18, "eflags" },
    { 19, "rsp" },
    { 20, "ss" },
    { 21, "fs_base" },
    { 22, "gs_base" },
    { 23, "ds" },
    { 24, "es" },
    { 25, "fs" },
    { 26, "gs" },
};

struct breakpoint  *break_point_list[5];
int count_break_point=0;
int count_step = 0;
void handle_command(char*);
void step_over_breakpoint();

struct debugee *child;
struct breakpoint *breakpt;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Program name not specified");
        return -1;
    }
    child = (struct debugee*)malloc(sizeof(struct debugee));
    breakpt  = (struct breakpoint*)malloc(sizeof(struct breakpoint));
    breakpt->active = 0;

    child->name = argv[1];
    child->pid = fork();
    printf("Child's PID: %d\n",child->pid);
    if (child->pid == 0) {
        personality(ADDR_NO_RANDOMIZE);
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        execl(child->name, child->name, NULL);
    }else if (child->pid >= 1)  {
        int status;
        int options = 0;
        waitpid(child->pid, &status, options);
        char* line = NULL;
        while((line = linenoise("minidbg> ")) != NULL) {
            handle_command(line);
            linenoiseHistoryAdd(line);
            linenoiseFree(line);
        }
    }

    free(child);
    clean_break_points();
    // free(breakpt);

    return 0;
}

void handle_command(char* line){
    char* values[6];
    char* str_copy = strdup(line);
    char * token = strtok(str_copy, " ");
    int counter_inputs =0;
    while( token != NULL ) {
        values[counter_inputs] = strdup(token);
        token = strtok(NULL, " ");
        counter_inputs++;
        if(counter_inputs == 6) printf("You can provide up to 5 words in the input");
    }
    int code = get_option_menu(values[0]);
    if(code==-1){//command does not exist
        printf("Command does not exist");
       return -1;
    }

    switch (code)
    {
        case 0:
            if(count_break_point<5){
                nextcreate_break_point(values[1]);
            }
            else{
                printf("You have a limit of 5 break points");
            }
            break;
        case 1:
            continue_process();
            break;
        case 2:
            register_intruction(values[1],values[2]);
            break;
        case 3:
            /* next */
            count_step++;
            if(count_step==1){
                continue_process();

            }else{
                step2();
            }
            
            break;
    }
}

int ptrace_instruction_pointer(uint64_t *rip)
{
    struct user_regs_struct regs;
    if( ptrace(PTRACE_GETREGS, child->pid, NULL, (void*)&regs) ) {
        fprintf(stderr, "Error fetching registers from child process: %s\n",
            strerror(errno));
        return -1;
    }
    if( rip )
        *rip = regs.rip;
    return 0;
}

int singlestep()
{
    int retval, status, signal = 0;
    retval = ptrace(PTRACE_SINGLESTEP, child->pid, NULL, signal);

    if( retval ) {
        return retval;
    }
    waitpid(child->pid, &status, 0);
    return status;
}

void step2(){
    uint64_t rip;
    struct user_regs_struct regs;

    ptrace_instruction_pointer(&rip);

    fprintf(stderr, "RIP: %p\n", (void*)rip);
    singlestep();

    

}

void step(){
    struct user_regs_struct regs;
    //nos sirve para obtener la instrucción proxima a ejecutar
    ptrace(PTRACE_GETREGS, child->pid, NULL, &regs);
    printf("instrucción que se ejecuta: %lld\n",regs.rip);
    printf("val = 0x%" PRIx64 "\n", regs.rip);

    int status;
     //To execute a singe step
    ptrace(PTRACE_SINGLESTEP, child->pid, NULL, NULL);
    waitpid(child->pid,status,0);
}

void step_by_step(){
    int status;

    struct user_regs_struct regs;
    int start = 0;
    long ins;
    while(1) {
        waitpid(child->pid, &status, 0);
        if(WIFEXITED(status)){
            break;
        }
        ptrace(PTRACE_GETREGS,child->pid, NULL, &regs);
        if(start == 1) {
            ins = ptrace(PTRACE_PEEKTEXT,child, regs.rdi,NULL);
            printf("EIP: %lx Instruction "
                    "executed: %lx\n",
                    regs.rdi, ins);
        }
        if(regs.orig_rax == SYS_write) {
            start = 1;
            ptrace(PTRACE_SINGLESTEP, child,
                    NULL, NULL);
        }
        else
            ptrace(PTRACE_SYSCALL, child,
                    NULL, NULL);
    }
}

void register_intruction(char* cmd,char* addr){
    struct user_regs_struct regs ;
    uint64_t *p;
    uint64_t *reg_addr;
    ptrace(PTRACE_GETREGS,child->pid,NULL,&regs);
    p=(uint64_t*)&(regs.rip);
    if(strcmp(cmd,"read")==0){
        printf("register is in : %lx \n",*p);
    }else if (strcmp(cmd,"write")==0){
        *p=(uint64_t)strtol(addr,NULL,0);
        ptrace(PTRACE_SETREGS,child->pid,NULL,&regs);
    }
}


void create_break_point(char* addr){
    struct breakpoint *tempbp ;
    tempbp = malloc(sizeof(tempbp));
    tempbp->addr=(uint64_t)strtol(addr,NULL,0);
    uint64_t data = ptrace(PTRACE_PEEKDATA, child->pid, tempbp->addr, NULL);
    tempbp->prev_opcode = (uint8_t)(data & 0xff);
    uint64_t int3 = 0xcc;
    uint64_t data_with_int3 = ((data & ~0xff) | int3);
    ptrace(PTRACE_POKEDATA, child->pid, tempbp->addr, data_with_int3);
    tempbp->active = 1;
    break_point_list[count_break_point]=tempbp;
    count_break_point= count_break_point+1;
}

void enable_break(struct breakpoint* breakpoint){
    uint64_t data = ptrace(PTRACE_PEEKDATA, child->pid, breakpoint->addr, NULL);
    breakpoint->prev_opcode = (uint8_t)(data & 0xff);
    uint64_t int3 = 0xcc;
    uint64_t data_with_int3 = ((data & ~0xff) | int3);
    ptrace(PTRACE_POKEDATA, child->pid, breakpoint->addr, data_with_int3);
    breakpoint->active = 1;
}

void disable_break(struct breakpoint* breakpoint){
    //To disable a breakpoint
    uint64_t data = ptrace(PTRACE_PEEKDATA, child->pid, breakpoint->addr, NULL);
    uint64_t restored_data = ((data & ~0xff) | breakpoint->prev_opcode);
    ptrace(PTRACE_POKEDATA, child->pid, breakpoint->addr, restored_data);
    breakpoint->active = 0;
}

void clean_break_points(){
    for (int i = 0; i < 5; i++)
    {
        if(break_point_list[i]!=NULL){
            free(break_point_list[i]);
        }
    }
}

void rewindInstructionRegister(uint64_t newValeInstructionRegister){
    for(int i=0; i < 5; +i++){
        if(break_point_list[i] != NULL){
            if(newValeInstructionRegister == break_point_list[i]->addr){
                struct user_regs_struct regs;
                ptrace(PTRACE_GETREGS, child->pid, NULL, &regs);
                regs.rip = newValeInstructionRegister;
                ptrace(PTRACE_SETREGS, child->pid, NULL, &regs);
                disable_break(break_point_list[i]);
                ptrace(PTRACE_SINGLESTEP, child->pid, NULL, NULL);
                int status;
                waitpid(child->pid,&status,0);
                enable_break(break_point_list[i]);
            }
        }
    }
}


void continue_process(){
    struct user_regs_struct regs;
    //nos sirve para obtener la instrucción proxima a ejecutar
    ptrace(PTRACE_GETREGS, child->pid, NULL, &regs);
    //printf("instrucción que se ejecuta: %lld\n",regs.rip);
    //printf("val = 0x%" PRIx64 "\n", regs.rip);
    uint64_t currentValueInstructionRegister = (regs.rip);
    uint64_t preInstructionRegister = currentValueInstructionRegister-1;
    rewindInstructionRegister(preInstructionRegister);

    int status;
    ptrace(PTRACE_CONT,child->pid,NULL ,NULL);

    waitpid(child->pid,status,0);
    //printf("valor de signal: %d\n",status);
}


//main handle command
void handle_commandOld(char* line) {

    //At this point you must to implement all the logic to manage the inputs of the program:
    //continue -> To continue the execution of the program
    //next -> To go step by step
    //register write/read <reg_name> <value>(when write format 0xVALUE) -> To read/write the value of a register (see the global variable g_register_descriptors)
    //break <0xVALUE> (Hexadecimal) -> To put a breakpoint in an adress

    //The following lines show a basic example of how to use the PTRACE API

    //Read the registers
    struct user_regs_struct regs;
    uint64_t *register_address;
    ptrace(PTRACE_GETREGS, child->pid, NULL, &regs);

    //Write the registers -> If you want to change a register, you must to read them first using the previous call, modify the struct user_regs_struct
    //(the register that you want to change) and then use the following call
    ptrace(PTRACE_SETREGS, child->pid, NULL, &regs);

    //If you want to enable a breakpoint (in a provided adress, for example 0x555555554655), you must to use the following CALL
    breakpt->addr =  ((uint64_t)strtol("0x5555555551a9", NULL, 0));
    uint64_t data = ptrace(PTRACE_PEEKDATA, child->pid, breakpt->addr, NULL);
    breakpt->prev_opcode = (uint8_t)(data & 0xff);
    uint64_t int3 = 0xcc;
    uint64_t data_with_int3 = ((data & ~0xff) | int3);
    ptrace(PTRACE_POKEDATA, child->pid, breakpt->addr, data_with_int3);
    breakpt->active = 1;

    //To disable a breakpoint
    data = ptrace(PTRACE_PEEKDATA, child->pid, breakpt->addr, NULL);
    uint64_t restored_data = ((data & ~0xff) | breakpt->prev_opcode);
    ptrace(PTRACE_POKEDATA, child->pid, breakpt->addr, restored_data);
    breakpt->active = 0;

    //To execute a singe step
    ptrace(PTRACE_SINGLESTEP, child->pid, NULL, NULL);

    //To read the value in a memory adress
    uint64_t value_in_memory = (uint64_t)ptrace(PTRACE_PEEKDATA, child->pid, (uint64_t)strtol("0x5555555551a9", NULL, 0), NULL);

    //To write a value in an adress
    ptrace(PTRACE_POKEDATA, child->pid, (uint64_t)strtol("0x555555554655", NULL, 0), (uint64_t)strtol("0x555555554655", NULL, 0));

    //If you want to continue with the execution of the debugee program
    ptrace(PTRACE_CONT, child->pid, NULL, NULL);
    int status;
    int options = 0;
    waitpid(child->pid, &status, options);
}


const  int get_option_menu(char* option_name){
    for (int i = 0; i < 4; i++)
    {
        if(strcmp(options_menu[i].name,option_name)==0){
            return(options_menu[i].code);
        }
    }
    return -1;
}