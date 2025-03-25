#include"shell.h"

#define COLOR_RED "\033[1;31m"
#define COLOR_RESET "\033[0m"

char* remove_spaces(char* str) {
    char* end;
    while (isspace((unsigned char)*str)) str++;
    if (*str == 0) 
        return str;
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    *(end + 1) = 0;
    return str;
}

void spec_2_execute(char* instruction,int background,char *hdirectory,char *prev_directory,int history,int *numberback,char *last_command,int *last_command_duration)
{
    (*last_command_duration)=0;
    instruction=remove_spaces(instruction);
    if(strlen(instruction)==0)
        return;
    if(strncmp(instruction,"exit",4)==0)
    {
        exit(0);
    }
    if(instruction==NULL)
    {
        printf(COLOR_RED "NO instruction given\n" COLOR_RESET);
        return;
    }
    if(strncmp(instruction,"log",3)==0 && strncmp(instruction+4,"purge",5)!=0 && strncmp(instruction+4,"execute",7)!=0)
    {
        spec_5_log_print(hdirectory);
    }
    else if(strncmp(instruction,"log",3)==0 && strncmp(instruction+4,"purge",5)==0)
    {
        spec_5_log_purge(hdirectory);
    }
    else if(strncmp(instruction,"log",3)==0 && strncmp(instruction+4,"execute",7)==0)
    {
        char *strnumber=strtok(instruction+12,"\n");
        int number=atoi(strnumber);
        spec_5_log_execute(hdirectory,number,prev_directory,numberback,last_command,last_command_duration);
    }
    else if(strncmp(instruction,"hop",3)==0)
    {
        char *steps=strtok(instruction+4,"\n");
        char *gotodirectory=strtok(steps," ");
        int check=0;
        while(gotodirectory!=NULL || check==0)
        {
            spec_3_hop(gotodirectory,hdirectory,prev_directory);
            gotodirectory=strtok(NULL," ");
            check=1;
        }
    }
    else if(strncmp(instruction,"reveal",6)==0)
    {
        char *flagandpath=strtok(instruction+7,"\n");
        spec_4_reveal(flagandpath,hdirectory,prev_directory);
    }
    else if(strncmp(instruction,"proclore",8)==0)
    {
        char *proclorepid=strtok(instruction+9,"\n");
        if(proclorepid==NULL)
            spec_7_proclore(getpid(),hdirectory);
        else
            spec_7_proclore(atoi(proclorepid),hdirectory);    
    }
    else if(strncmp(instruction,"seek",4)==0)
    {
        char *command=strtok(instruction+5,"\n");
        spec_8_seek(command,hdirectory,prev_directory);
    }
    else
    {
        char *arg[1024];
        char *tokenexec=strtok(instruction," ");
        int i=0;
        while(tokenexec!=NULL)
        {
            arg[i]=tokenexec;
            i++;
            tokenexec=strtok(NULL," ");
        }
        arg[i]=NULL;
        struct timeval start,end;
        gettimeofday(&start, NULL);
        int pid=fork();
        if(pid<0)
        {
            printf(COLOR_RED "Fork Failed\n" COLOR_RESET);
            return;
        }
        else if(pid==0)
        {
            
            int pid2=fork();
            if(pid2<0)
            {
                printf(COLOR_RED "Fork 2 Failed\n" COLOR_RESET);
                return;
            }
            else if(pid2==0)
            {
                if(execvp(arg[0],arg)==-1)
                {
                    printf(COLOR_RED "Error in execvp\n" COLOR_RESET);
                    exit(0);
                }
                exit(0);
            }
            else
            {
                if(background==1)
                {
                    printf("[%d] %d\n",*numberback,pid2);
                }
                int pid3=waitpid(pid2,NULL,0);
                if(background==1)
                {
                    int status;
                    if(pid3>0)
                    {
                        if(WIFEXITED(status))
                            printf("%s exited normally (%d)\n",instruction,pid2);
                        else if(WIFSIGNALED(status))
                            printf("%s exited abnormally (%d)\n",instruction,pid2);   
                    } 
                }
            }
            exit(0);
        }
        else
        {
            if(background==0)
            {
               waitpid(pid,NULL,0);
                gettimeofday(&end, NULL);
                (*last_command_duration)=(int)(end.tv_sec-start.tv_sec);
                if((*last_command_duration)>2)
                    strncpy(last_command,arg[0],1024);
            }  
            else
            {
                usleep(250);
                (*numberback)++;
            }
        }
    }

}

void spec_2_input(char *instruction,char *home,char *prev,int history,int *numberback,char *last_command,int *last_command_duration)
{
    char *command[1024];
    int commnad_count=0;
    char *token=strtok(instruction,";");
    while(token!=NULL)
    {
        command[commnad_count++]=token;
        token=strtok(NULL,";");
    }
    for(int i=0;i<commnad_count;i++)
    {
        int no=0;
        for(int k=0;k<strlen(command[i]);k++)
        if(command[i][k]=='&')
            no++;
        char *subcommand=strtok(command[i],"&");

        while(subcommand!=NULL)
        {
            char *subcommand2;
            subcommand2=subcommand;
            subcommand=strtok(NULL,"&");
            if(no==0)
            {
                spec_2_execute(subcommand2,0,home,prev,history,numberback,last_command,last_command_duration);
            }
            else
            {
                spec_2_execute(subcommand2,1,home,prev,history,numberback,last_command,last_command_duration);     
            } 
            no--;
        }
    }
}

