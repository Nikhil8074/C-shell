#include"shell.h"

#define COLOR_RESET "\x1b[0m"
#define COLOR_RED "\033[1;31m"

void spec_11_pipe(char *instruction,char *hdirectory,char *prev_directory,int *numberback,char *last_command,int *last_command_duration,process *processes[],int *count)
{
    char *command[1024];
    int i=0;
    int no_of_pipes=0;
    for(int k=0;k<strlen(instruction);k++)
        if(instruction[k]=='|')
            no_of_pipes++;
    char *token=strtok(instruction,"|");
    while(token!=NULL)
    {
        command[i]=token;
        command[i]=remove_spaces(command[i]);
        if(strcmp(command[i],"")==0)
        {
            printf(COLOR_RED"Invalid piping\n"COLOR_RESET);
            return;
        }
        i++;
        token=strtok(NULL,"|");
    }
    if(i!=no_of_pipes+1)
    {
        printf(COLOR_RED"Invalid piping\n"COLOR_RESET);
        return;
    }
    command[i]=NULL;
    int no_of_commands=i;
    int pipefd[2*(i-1)];
    for(int i=0;i<no_of_commands;i++)
        if(pipe(pipefd+2*i)==-1)
        {
            printf(COLOR_RED"Error! pipe failed\n"COLOR_RESET);
            return;
        }
    int pid[no_of_commands];        
    for(int i=0;i<no_of_commands;i++)
    {
        pid[i]=fork();
        if(pid[i]<0)
        {
            printf(COLOR_RED"Error! Fork failed\n"COLOR_RESET);
            return;
        }
        else if(pid[i]==0)
        {
            if(i>0)
            {
                close(pipefd[(i-1)*2+1]);  
                dup2(pipefd[(i-1)*2],STDIN_FILENO);
                close(pipefd[(i-1)*2]);
            }
            if(i<no_of_commands-1)
            {
                close(pipefd[i*2]);
                dup2(pipefd[i*2+1],STDOUT_FILENO);
                close(pipefd[i*2+1]);
            }

            for(int j=0;j<2*(i-1);j++)
                close(pipefd[j]);

            char *commands=command[i];
            shell_execution(commands,hdirectory,prev_directory,0,numberback,last_command,last_command_duration,processes,count);
            exit(0);
        }
        else
        {
            usleep(500);
        }
    }
    for(int i=0;i<2*(no_of_commands-1);i++)
        close(pipefd[i]);
    for(int i=0;i<no_of_commands;i++)
        waitpid(pid[i],NULL,0);


}