#include"shell.h"

#define COLOR_RED "\033[1;31m"
#define COLOR_RESET "\033[0m"

void shell_execution(char *instruction,char *hdirectory,char *prev_directory,int history,int *numberback,char *last_command,int *last_command_duration,process *processes[],int *count)
{
    if(strstr(instruction,"log")==NULL && history==1)
            spec_5_log(instruction,hdirectory);
    spec_2_input(instruction,hdirectory,prev_directory,history,numberback,last_command,last_command_duration,processes,count);
    return;
}

int present_pid=-1;
int ctrl=0;
int shell_gpid;
aliases aliasess[1024];
int alias_count=0;

int main()
{
    signal(SIGTSTP,spec_14_ctrl_Z);
    int fd=open(".myshrc",O_CREAT | O_TRUNC ,0644);
    close(fd);
    int numberback=1;
    int last_command_duration=0;
    process *processes[1024];
    int count=0;
    char last_command[1024];
    char hdirectory[PATH_MAX];
    char prev_directory[PATH_MAX]="";
    if(getcwd(hdirectory,sizeof(hdirectory))==NULL)
    {
        printf(COLOR_RED "Error occured while getcwd of home in main\n" COLOR_RESET);
        exit(0);
    }
    while(1)
    {
        spec_1_display(hdirectory,last_command,&last_command_duration);
        char instruction[1024];
        if(fgets(instruction,sizeof(instruction),stdin)==NULL)
        {
            spec_14_ctrl_D(processes,&count);
        }
        shell_execution(instruction,hdirectory,prev_directory,1,&numberback,last_command,&last_command_duration,processes,&count);
    }
}