#include"shell.h"

#define COLOR_RED "\033[1;31m"
#define COLOR_RESET "\033[0m"

void spec_1_display(char home[],char *last_command,int *last_command_duration)
{
    char *username=getlogin();
    char systemhost[HOST_NAME_MAX];
    char pdirectory[PATH_MAX];

    if(username==NULL)
    {
        printf(COLOR_RED "Error occured at getlogin\n" COLOR_RESET);
        return;
    }
    if(gethostname(systemhost,HOST_NAME_MAX)==-1)
    {
        printf(COLOR_RED "Error occured while gethostname\n" COLOR_RESET);
        return;
    }
    if(getcwd(pdirectory,sizeof(pdirectory))==NULL)
    {
        printf(COLOR_RED "Error occured while getcwd\n" COLOR_RESET);
        return;
    }
    if((*last_command_duration)<=2)
    {
        if(strstr(pdirectory,home)==pdirectory)
        {
            printf("<%s@%s:~%s> ",username,systemhost,pdirectory+strlen(home));
        }
        else
        {
            printf("<%s@%s:~%s> ",username,systemhost,pdirectory);
        }
    }
    else if((*last_command_duration)>2)
    {
        if(strstr(pdirectory,home)==pdirectory)
        {
            printf("<%s@%s:~%s %s : %ds> ",username,systemhost,pdirectory+strlen(home),last_command,(*last_command_duration));
        }
        else
        {
            printf("<%s@%s:~%s %s : %ds> ",username,systemhost,pdirectory,last_command,(*last_command_duration));
        }
    }
    
}
