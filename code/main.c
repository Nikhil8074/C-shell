#include"shell.h"

#define COLOR_RED "\033[1;31m"
#define COLOR_RESET "\033[0m"

void shell_execution(char *instruction,char *hdirectory,char *prev_directory,int history,int *numberback,char *last_command,int *last_command_duration)
{
    if(strstr(instruction,"log")==NULL && history==1)
            spec_5_log(instruction,hdirectory);
    spec_2_input(instruction,hdirectory,prev_directory,history,numberback,last_command,last_command_duration);
    return;
}

int main()
{
    int numberback=1;
    int last_command_duration=0;
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
        fgets(instruction,sizeof(instruction),stdin);
        shell_execution(instruction,hdirectory,prev_directory,1,&numberback,last_command,&last_command_duration);
    }
}
