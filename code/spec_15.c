#include"shell.h"

#define COLOR_RESET "\x1b[0m"
#define COLOR_RED "\033[1;31m"

void spec_15_fg(char *pid_str,process *processes[],int *count)
{
    shell_gpid=getpgid(0);
    int target_pid=atoi(pid_str);
    int found=0;
    int i=0;
    for(i=0;i<(*count);i++)
        if(processes[i]->pid==target_pid && processes[i]->background==1)
        {
            found=1;
            break;
        }
    if(found==0)
    {
        printf(COLOR_RED"No background process found with pid %d\n"COLOR_RESET,target_pid);
        return;
    }
    int kill_pid=kill(target_pid,SIGCONT);
    signal(SIGTTOU,SIG_IGN);
    tcsetpgrp(STDIN_FILENO,getpgid(target_pid));
    if(kill_pid<0)
    {
        printf(COLOR_RED"Error in bringing the process from stopped to running\n"COLOR_RESET);
        tcsetpgrp(STDIN_FILENO,getpid());
        signal(SIGTTOU,SIG_DFL);
        return;
    }
    processes[i]->background=0;
    int stat;
    waitpid(kill_pid,&stat,WUNTRACED);
    tcsetpgrp(STDIN_FILENO,getpid());
    signal(SIGTTOU,SIG_DFL);
    return;

}

void spec_15_bg(char *pid_str,process *processes[],int *count)
{
    int target_pid=atoi(pid_str);
    int found=0;
    for(int i=0;i<(*count);i++)
        if(processes[i]->pid==target_pid && processes[i]->background==1)
        {
            found=1;
            break;
        }
    if(found==0)
    {
        printf(COLOR_RED"No background process found with pid %d"COLOR_RESET,target_pid);
        return;
    }    

    if(kill(target_pid,SIGCONT)<0)
    {
        printf(COLOR_RED"Error in bringing the process from stopped to running\n"COLOR_RESET);
        return;
    }

}