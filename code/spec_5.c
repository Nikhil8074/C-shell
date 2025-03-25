#include"shell.h"

#define COLOR_RED "\033[1;31m"
#define COLOR_RESET "\033[0m"

void spec_5_log(char* instruct,char *home)
{   
    char logfilepath[PATH_MAX];
    snprintf(logfilepath,sizeof(logfilepath),"%s/%s",home,"log_history");
    FILE *file=fopen(logfilepath,"a");
    if(file==NULL)
    {
        printf(COLOR_RED "Error occured while fopen the logfile\n" COLOR_RESET);
        return;
    }
    fclose(file);

    file=fopen(logfilepath,"r");
    if(file==NULL)
    {
        printf(COLOR_RED "Error occured while fopen the logfile\n" COLOR_RESET);
        return;
    }
    char *log[15]={NULL};
    int log_count=0;
    char logline[1024];

    while(fgets(logline,sizeof(logline),file)!=NULL)
    {
        log[log_count]=strdup(logline);
        log_count++;
    }

    if(strcmp(instruct,log[log_count-1])==0)
        return;

    if(log_count==15)
    {
        free(log[0]);
        memmove(log,log+1,14*sizeof(char *));
        log[14]=strdup(instruct);
    }
    else
    {
        log[log_count]=strdup(instruct);
        log_count++;
    }
    fclose(file);
    file=fopen(logfilepath,"w");
    if(file==NULL)
    {
        printf(COLOR_RED "Error occured while fopen the logfile\n" COLOR_RESET) ;
        return;
    }
    for(int i=0;i<log_count;i++)
    {
        fprintf(file,"%s",log[i]);
        free(log[i]);
    }
    fclose(file);
}

void spec_5_log_print(char *home)
{
    char logfilepath[PATH_MAX];
    snprintf(logfilepath,sizeof(logfilepath),"%s/%s",home,"log_history");
    FILE *file=fopen(logfilepath,"a");
    if(file==NULL)
    {
        printf(COLOR_RED "Error occured while fopen the logfile\n" COLOR_RESET);
        return;
    }
    fclose(file);

    file=fopen(logfilepath,"r");
    if(file==NULL)
    {
        printf(COLOR_RED "Error occured while fopen the logfile\n" COLOR_RESET);
        return;
    }
    int log_count=0;
    char logline[1024];

    while(fgets(logline,sizeof(logline),file)!=NULL)
    {
        printf("%s",logline);
        log_count++;
    }
    if(log_count==0)
        printf(COLOR_RED "No History(log file empty)\n" COLOR_RESET);
    fclose(file);    
}

void spec_5_log_purge(char *home)
{
    char logfilepath[PATH_MAX];
    snprintf(logfilepath,sizeof(logfilepath),"%s/%s",home,"log_history");
    FILE *file=fopen(logfilepath,"w");
    if(file==NULL)
    {
        printf(COLOR_RED "Error occured while fopen the logfile\n" COLOR_RESET);
        return;
    }
    fclose(file);
}

void spec_5_log_execute(char *home,int n,char *prev_directory,int *numberback,char *last_command,int *last_command_duration,process *processes[],int *count)
{
    char logfilepath[PATH_MAX];
    snprintf(logfilepath,sizeof(logfilepath),"%s/%s",home,"log_history");
    FILE *file=fopen(logfilepath,"a");
    if(file==NULL)
    {
        printf(COLOR_RED "Error occured while fopen the logfile\n" COLOR_RESET);
        return;
    }
    fclose(file);

    file=fopen(logfilepath,"r");
    if(file==NULL)
    {
        printf(COLOR_RED "Error occured while fopen the logfile\n" COLOR_RESET);
        return;
    }
    char *log[15];
    int log_count=0;
    char logline[1024];

    while(fgets(logline,sizeof(logline),file)!=NULL)
    {
        log[log_count]=strdup(logline);
        log_count++;
    }
    if(n>log_count)
    {
        printf(COLOR_RED "no of commands in log file is %d\n" COLOR_RESET,log_count);
        return;
    } 
    shell_execution(log[log_count-n],home,prev_directory,0,numberback,last_command,last_command_duration,processes,count);
}