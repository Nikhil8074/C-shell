#include"shell.h"

#define COLOR_RED "\033[1;31m"
#define COLOR_RESET "\033[0m"


char* remove_spaces(char* str) 
{
    char* end;
    while (isspace((unsigned char)*str)) str++;
    if (*str == 0) 
        return str;
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    *(end + 1) = 0;
    return str;
}

void spec_2_execute(char* instruction,int background,char *hdirectory,char *prev_directory,int history,int *numberback,char *last_command,int *last_command_duration,process *processes[],int *count)
{
    (*last_command_duration)=0;
    instruction=remove_spaces(instruction);
    if(strlen(instruction)==0)
        return;
    if(strncmp(instruction,"exit",4)==0)
    {
        spec_14_ctrl_D(processes,count);
    }

    if(instruction==NULL)
    {
        printf(COLOR_RED "NO instruction given\n" COLOR_RESET);
        return;
    }
    else if(strchr(instruction,'<')!=NULL || strchr(instruction,'>')!=NULL)
    {
        if(strstr(instruction,">>")!=NULL)
        {
            if(strstr(instruction,"<")==NULL)
            {
                spec_10_redirect(instruction,1,0,0,hdirectory,prev_directory,numberback,last_command,last_command_duration,processes,count);
            }
            else
                spec_10_redirect(instruction,1,0,1,hdirectory,prev_directory,numberback,last_command,last_command_duration,processes,count);
        }
        else if(strchr(instruction,'>')!=NULL)
        {
            if(strstr(instruction,"<")==NULL)
                spec_10_redirect(instruction,0,1,0,hdirectory,prev_directory,numberback,last_command,last_command_duration,processes,count);
            else
                spec_10_redirect(instruction,0,1,1,hdirectory,prev_directory,numberback,last_command,last_command_duration,processes,count);       
        }
        else if(strchr(instruction,'<')!=NULL)
            spec_10_redirect(instruction,0,0,1,hdirectory,prev_directory,numberback,last_command,last_command_duration,processes,count);     
    }
    else if(strncmp(instruction,"activities",10)==0)
    {
        spec_13_activities(processes,count);
    }
    else if(strncmp(instruction,"log",3)==0 && strncmp(instruction+4,"purge",5)!=0 && strncmp(instruction+4,"execute",7)!=0)
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
        spec_5_log_execute(hdirectory,number,prev_directory,numberback,last_command,last_command_duration,processes,count);
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
            spec_7_proclore(getpid(),hdirectory,processes,count);
        else
            spec_7_proclore(atoi(proclorepid),hdirectory,processes,count);    
    }
    else if(strncmp(instruction,"seek",4)==0)
    {
        char *command=strtok(instruction+5,"\n");
        spec_8_seek(command,hdirectory,prev_directory);
    }
    else if(strncmp(instruction,"neonate",7)==0)
    {
        char *commandforneo=strtok(instruction+11,"\n");
        spec_16_neonate_detect(commandforneo);
    }
    else if(strncmp(instruction,"ping",4)==0)
    {
        char *pid_str=strtok(instruction+5," ");
        char *signal_str=strtok(NULL,"\n");
        int pid_sig=atoi(pid_str);
        int sig=atoi(signal_str);
        spec_14_signals(pid_sig,sig);
    }
    else if(strncmp(instruction,"fg",2)==0)
    {
        char *pid_str=strtok(instruction+3,"\n");
        spec_15_fg(pid_str,processes,count);
    }
    else if(strncmp(instruction,"bg",2)==0)
    {
        char *pid_str=strtok(instruction+3,"\n");
        spec_15_bg(pid_str,processes,count);
    }
    else if(strncmp(instruction,"iMan",4)==0)
    {
        char *commadforiman=strtok(instruction+5," ");
        spec_17_iman(commadforiman);
    }
    else if(strncmp(instruction,"nano .myshrc",12)==0)
    {
        spec_9_myshrc();
    }
    else
    {
        int pipefd[2];
        int pid, grandchild_pid;
        int pid2;
        if (pipe(pipefd) == -1) 
        {
            perror("pipe failed");
            exit(EXIT_FAILURE);
        }
        char *arg[4096];
        char commandforbg[1024];
        strcpy(commandforbg,instruction);
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
        pid=fork();
        if(pid<0)
        {
            printf(COLOR_RED "Fork Failed\n" COLOR_RESET);
            return;
        }
        else if(pid==0)
        {
            close(pipefd[0]);
            pid2=fork();
            if(pid2<0)
            {
                printf(COLOR_RED "Fork 2 Failed\n" COLOR_RESET);
                return;
            }
            else if(pid2==0)
            {   
                if(execvp(arg[0],arg)==-1)
                {
                    printf(COLOR_RED "Command not exist\n" COLOR_RESET);
                    exit(0);
                }
            }
            else
            {

                write(pipefd[1],&pid2,sizeof(pid2));
                close(pipefd[1]);
                if(background==1)
                {
                    printf("[%d] %d\n",*numberback,pid2);
                }
                int status;
                int pid3=waitpid(pid2,&status,WUNTRACED);
                if(background==1)
                {
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
            close(pipefd[1]);
            read(pipefd[0],&grandchild_pid,sizeof(grandchild_pid));
            close(pipefd[0]);
            if(background==0)
            {
                present_pid=grandchild_pid;
                waitpid(pid,NULL,0);
                present_pid=-1;
                gettimeofday(&end, NULL);
                (*last_command_duration)=(int)(end.tv_sec-start.tv_sec);
                if((*last_command_duration)>2)
                    strncpy(last_command,arg[0],1024);
                processes[*count]=malloc(sizeof(process));
                processes[*count]->pid=grandchild_pid;
                strncpy(processes[*count]->instruction,commandforbg,sizeof(processes[*count]->instruction)-1);
                processes[*count]->instruction[sizeof(processes[*count]->instruction)-1] = '\0';
                processes[*count]->background=0;
                (*count)++;
            }  
            else
            {
                (*numberback)++;
                setpgid(grandchild_pid,grandchild_pid);
                processes[*count]=malloc(sizeof(process));
                processes[*count]->pid=grandchild_pid;
                strncpy(processes[*count]->instruction,commandforbg,sizeof(processes[*count]->instruction)-1);
                processes[*count]->instruction[sizeof(processes[*count]->instruction)-1] = '\0';
                processes[*count]->background=1;
                (*count)++;
            }
            
        }
    }

}

void spec_2_input(char *instruction,char *home,char *prev,int history,int *numberback,char *last_command,int *last_command_duration,process *processes[],int *count)
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
        if(strchr(command[i],'|')!=NULL)
        {
            spec_11_pipe(instruction,home,prev,numberback,last_command,last_command_duration,processes,count);
        }
        else
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
                    spec_9_replace(subcommand2);
                    spec_2_execute(subcommand2,0,home,prev,history,numberback,last_command,last_command_duration,processes,count);
                }
                else
                {
                    spec_9_replace(subcommand2);
                    spec_2_execute(subcommand2,1,home,prev,history,numberback,last_command,last_command_duration,processes,count);     
                } 
                no--;
            }
        }
    }
}

