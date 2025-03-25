#include"shell.h"

#define COLOR_RED "\033[1;31m"
#define COLOR_RESET "\033[0m"

void spec_3_hop(char *dest,char *home,char *prev)
{
    if(dest==NULL || strcmp(dest,"~")==0)
        dest=(char*)home;
    else if(strcmp(dest,"-")==0 && strcmp(prev,"")!=0)
        dest=(char*)prev;
    else if(strcmp(dest,"-")==0 && strcmp(prev,"")==0)   
    {
        printf(COLOR_RED "Error No previous directory\n" COLOR_RESET);
        return;
    } 
    char present[PATH_MAX];
    if(getcwd(present,sizeof(present))==NULL)
    {
        printf(COLOR_RED "Error occured while getcwd in hop\n" COLOR_RESET);
        return;
    } 
    
    if(chdir(dest)!=0)
    {
        printf(COLOR_RED "Error occured while chdir\n" COLOR_RESET);
        return;
    }

    char printpresent[PATH_MAX];
    if(getcwd(printpresent,sizeof(printpresent))==NULL)
    {
        printf(COLOR_RED "Error occured while getcwd in hop\n" COLOR_RESET);
        return;
    }    
    printf("%s\n",printpresent);
    strcpy(prev,present);

}