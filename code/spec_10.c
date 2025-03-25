#include"shell.h"

#define COLOR_RESET "\x1b[0m"
#define COLOR_RED "\033[1;31m"

void spec_10_redirect(char *instruction,int append,int write,int read,char *hdirectory,char *prev_directory,int *numberback,char *last_command,int *last_command_duration,process *processes[],int *count)
{
    if(append==1)
    {
        if(read==0)
        {
            char *command=strtok(instruction,">>");
            char *filename=strtok(NULL,"\n");
            memmove(filename,filename+1,strlen(filename));
            filename=remove_spaces(filename);
            int file=open(filename,O_CREAT | O_APPEND | O_WRONLY,0644);
            if(file<0)
            {
                printf(COLOR_RED"ERROR! Unable to open file\n"COLOR_RESET);
                return;
            }
            freopen(filename,"a",stdout);
            shell_execution(command,hdirectory,prev_directory,0,numberback,last_command,last_command_duration,processes,count);
            freopen("/dev/tty","a",stdout);
            close(file);
        }
        else
        {
            char *command=strtok(instruction,"<");
            char *filename1=strtok(NULL,">>");
            char *filename2=strtok(NULL,"\n");
            filename1=remove_spaces(filename1);
            memmove(filename2,filename2+1,strlen(filename2));
            filename2=remove_spaces(filename2);
            int file2=open(filename2,O_CREAT | O_APPEND | O_WRONLY,0644);
            if(file2<0)
            {
                printf(COLOR_RED"ERROR! Unable to open file\n"COLOR_RESET);
                return;
            }
            int file1=open(filename1,O_RDONLY);
            if(file1<0)
            {
                printf(COLOR_RED"Error! File not exist\n"COLOR_RESET);
                return;
            }
            freopen(filename2,"a",stdout);
            freopen(filename1,"r",stdin);
            shell_execution(command,hdirectory,prev_directory,0,numberback,last_command,last_command_duration,processes,count);
            freopen("/dev/tty","a",stdout);
            freopen("/dev/tty","r",stdin);
            close(file1);
            close(file2);

        }
    }
    else if(write==1)
    {   
        if(read==0)
        {
            char *command=strtok(instruction,">");
            char *filename=strtok(NULL,"\n");
            filename=remove_spaces(filename);
            int file=open(filename,O_CREAT | O_WRONLY,0644);
            if(file<0)
            {
                printf(COLOR_RED"ERROR! Unable to open file\n"COLOR_RESET);
                return;
            }
            freopen(filename,"w",stdout);
            shell_execution(command,hdirectory,prev_directory,0,numberback,last_command,last_command_duration,processes,count);
            freopen("/dev/tty","w",stdout);
            close(file);
        }
        else
        {
            char *command=strtok(instruction,"<");
            char *filename1=strtok(NULL,">");
            char *filename2=strtok(NULL,"\n");
            filename1=remove_spaces(filename1);
            filename2=remove_spaces(filename2);
            int file2=open(filename2,O_CREAT | O_WRONLY,0644);
            if(file2<0)
            {
                printf(COLOR_RED"ERROR! Unable to open file\n"COLOR_RESET);
                return;
            }
            int file1=open(filename1,O_RDONLY);
            if(file1<0)
            {
                printf(COLOR_RED"Error! File not exist\n"COLOR_RESET);
                return;
            }
            freopen(filename2,"w",stdout);
            freopen(filename1,"r",stdin);
            shell_execution(command,hdirectory,prev_directory,0,numberback,last_command,last_command_duration,processes,count);
            freopen("/dev/tty","w",stdout);
            freopen("/dev/tty","r",stdin);
            close(file1);
            close(file2);
        }
    }
    else if(read==1)
    {
        char *command=strtok(instruction,"<");
        char *filename=strtok(NULL,"\n");
        filename=remove_spaces(filename);
        int file=open(filename,O_RDONLY);
        if(file<0)
        {
            printf(COLOR_RED"Error! File not exist\n"COLOR_RESET);
            return;
        }
        freopen(filename,"r",stdin);
        shell_execution(command,hdirectory,prev_directory,0,numberback,last_command,last_command_duration,processes,count);
        freopen("/dev/tty","r",stdin);
        close(file);
    }
}