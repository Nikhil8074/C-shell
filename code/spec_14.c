#include"shell.h"

#define COLOR_RESET "\x1b[0m"
#define COLOR_BLUE "\x1b[34m" 
#define COLOR_RED "\033[1;31m"

void spec_14_signals(int pid,int signal_number)
{
    int signal_code=signal_number%32;
    if(kill(pid,0)==-1)
    {
        printf(COLOR_RED"No such Process found\n"COLOR_RESET);
        return;
    }

    if(kill(pid,signal_code)==0)
        printf("Sent signal %d to process with pid %d\n",signal_code,pid);
    else
        printf(COLOR_RED"Error Sending Signal\n"COLOR_RESET);      
}

void spec_14_ctrl_D(process *processes[],int *count)
{
    printf(COLOR_BLUE"\nLogging out of Shell....\n"COLOR_RESET);
    for(int i=0;i<(*count);i++)
        kill(processes[i]->pid,SIGTERM);
    exit(0);
}

// void spec_14_ctrl_C(int sig)
// {
//     if(present_pid!=-1)
//     {
//         kill(present_pid,SIGINT);
//         printf("\n");
//     }
//     return;
// }

void spec_14_ctrl_Z(int sig)
{
    if(present_pid!=-1)
    {
        kill(present_pid,SIGTSTP);
        ctrl++;
        printf("\n[%d]+  Stopped    pid : %d\n",ctrl,present_pid);
    }
    return;
}
