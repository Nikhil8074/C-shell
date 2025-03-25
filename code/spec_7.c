#include"shell.h"

#define COLOR_RED "\033[1;31m"
#define COLOR_RESET "\033[0m"

void spec_7_proclore(pid_t pid,char *hdirectory)
{
    char path[PATH_MAX];
    char buf[1024];
    char status[1024];
    char exe[PATH_MAX];
    FILE *file;

    printf("pid : %d\n",pid);
    snprintf(path,sizeof(path),"/proc/%d/status",pid);
    file=fopen(path,"r");
    if(file==NULL)
    {
        printf(COLOR_RED "Error occured while proclore file\n" COLOR_RESET);
        return;
    }
    while(fgets(buf,sizeof(buf),file))
    {
        if(strncmp(buf,"State:",6)==0)
        {
            char *token=strtok(buf,":");
            if(token!=NULL)
            {
                token=strtok(NULL," \n");
                if(token!=NULL)
                {
                    strcpy(status,token);
                    break;
                }
            }
        }
    }
    fclose(file);
    int tpid=tcgetpgrp(STDIN_FILENO);
    int grouppid=getpgid(pid);
    if(strchr(status,'R')) 
        printf("process status : %s\n",(tpid==grouppid) ? "R+" : "R");
    else if(strchr(status,'S'))
        printf("process status : %s\n",(tpid==grouppid) ? "S+" : "S");
    else if(strchr(status, 'Z')) 
        printf("process status : Z\n");
    else 
        printf("process status : Unknown\n");

    printf("Process Group : %d\n",grouppid);

    file=fopen(path,"r");
    if(file==NULL)
    {
        printf(COLOR_RED "Error occured while proclore file\n" COLOR_RESET);
        return;
    } 
    while(fgets(buf,sizeof(buf),file)) 
    {
        if(strncmp(buf,"VmSize:",7)==0) 
        {
            sscanf(buf,"VmSize:\t%s",status);
            printf("Virtual memory : %s\n",status);
            break;
        }
    }
    fclose(file);

    snprintf(path,sizeof(path),"/proc/%d/exe",pid);
    int new=readlink(path,exe,sizeof(exe)-1);
    if(new<0)
    {
        printf(COLOR_RED "Error occured while reading exe in proclore\n" COLOR_RESET);
        return;
    }
    exe[new]='\0';
     if(strstr(exe,hdirectory)==exe)
    {
        printf("executable path : ~%s\n",exe+strlen(hdirectory));
    }
    else
    {
        printf("executable path : %s\n",exe);
    }
    
}