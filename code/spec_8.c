#include"shell.h"

#define COLOR_BLUE "\x1b[34m" 
#define COLOR_GREEN "\x1b[32m"
#define COLOR_RESET "\x1b[0m"
#define COLOR_RED "\033[1;31m"

int executable_or_not(char *path) {
    struct stat sb;
    if (stat(path,&sb)==-1) 
      return 0;

    return (sb.st_mode & S_IXUSR) || (sb.st_mode & S_IXGRP) || (sb.st_mode & S_IXOTH);
}

void spec_8_seek_search(int f_flag,int d_flag,int e_flag,char *targetdir,char *search,char *rel,char *prev_directory)
{
    DIR *dir;
    struct dirent *file;
    struct stat fileinfo;
    char path[PATH_MAX];
    int no_of_files=0;
    int no_of_directories=0;
    char last_file[1024],last_dir[1024];

    dir=opendir(targetdir);
    if(dir==NULL)
    {
        printf(COLOR_RED "Error Occured while opendir in seek\n" COLOR_RESET);
        return;
    }
    while((file=readdir(dir))!=NULL)
    {
        if(strcmp(file->d_name,".")==0 || strcmp(file->d_name,"..")==0)
            continue;
        snprintf(path,sizeof(path),"%s/%s",targetdir,file->d_name);
        if(stat(path,&fileinfo)==-1)
        {
            printf(COLOR_RED "Error Occured while stat in seek\n" COLOR_RESET);
            return;
        }
        int dir_or_not=S_ISDIR(fileinfo.st_mode);
        int file_or_not=S_ISREG(fileinfo.st_mode);

        if(strncmp(file->d_name,search,strlen(search))==0)
        {
            if((d_flag && dir_or_not) || (f_flag && file_or_not) || (!d_flag && !f_flag))
            {
                if(dir_or_not)
                {
                    no_of_directories++;
                    strcpy(last_dir,path);
                    if(strncmp(path,rel,strlen(rel))==0)
                    {
                        char *rel_path=path+strlen(rel);
                        printf(".%s%s%s\n",COLOR_BLUE,rel_path,COLOR_RESET);
                    }
                }
                else if(file_or_not)
                {
                    no_of_files++;
                    strcpy(last_file,path);
                    if(strncmp(path,rel,strlen(rel))==0)
                    {
                        char *rel_path=path+strlen(rel);
                        printf(".%s%s%s\n",COLOR_GREEN,rel_path,COLOR_RESET);
                    }
                }
            }
        }
        if(dir_or_not)
        {
            spec_8_seek_search(f_flag,d_flag,e_flag,path,search,rel,prev_directory);
        }
    }
    closedir(dir);

    if(e_flag)
    {
        if(no_of_files==1 && no_of_directories==0)
        {
            if(executable_or_not(last_file))
            {
                execl(last_file,last_file,(char *)NULL);
                perror("execl");
            }
            else
            {
                FILE *new=fopen(last_file,"r");
                if(new==NULL)
                {
                    if(errno==EACCES)
                        printf(COLOR_RED "Missing Permissions for task!\n" COLOR_RESET);
                    else
                        printf(COLOR_RED "Error occured while fopen in seek\n" COLOR_RESET);   
                    return;     
                }
                char line[1024];
                while(fgets(line,sizeof(line),new)!=NULL)
                    printf("%s",line);
                fclose(new); 
            }   
        }
        else if(no_of_files==0 && no_of_directories==1)
        {
            char presentdir[PATH_MAX];
            if(getcwd(presentdir,sizeof(presentdir))==NULL)
            {
                printf(COLOR_RED "Error occured while getcwd in seek\n" COLOR_RESET);
                return;
            }
            if(chdir(last_dir)==-1)
            {
                if(errno==EACCES)
                    printf(COLOR_RED "Missing Permissions for task!\n" COLOR_RESET);
                else 
                    printf(COLOR_RED "Error occured while chdir in seek\n" COLOR_RESET);
                return;        
            }
            strcpy(prev_directory,presentdir);
        }
    }
}

void spec_8_seek(char *instruct,char *hdirectory,char *prev_directory)
{
    int d_flag=0;
    int f_flag=0;
    int e_flag=0;
    char *search=NULL;
    char *targetdir=NULL;
    if(strstr(instruct,"-d")!=NULL)
        d_flag=1;
    if(strstr(instruct,"-f")!=NULL)
        f_flag=1; 
    if(strstr(instruct,"-e")!=NULL)
        e_flag=1;   

    if(d_flag==1 && f_flag==1)
    {
        printf(COLOR_RED "Invalid Flags\n" COLOR_RESET);
        return;
    }       
    char *start=instruct;
    if(d_flag==1 || f_flag==1)
    {
        start+=(e_flag)? 6 : 3;
    }
    else
    {
        start+=(e_flag)? 3 : 0;
    }
    search=strtok(start," ");
    if(search!=NULL)
        targetdir=strtok(NULL,"\n");
             
    char presentdirectory[PATH_MAX];
    if(getcwd(presentdirectory,sizeof(presentdirectory))==NULL)
    {
        printf(COLOR_RED "Error occured while getcwd in seek\n" COLOR_RESET);
        return;
    }
    if(targetdir==NULL)
        targetdir=(char*)presentdirectory;
    else if(strcmp(targetdir,"..")==0)
        targetdir=dirname(presentdirectory);
    else if(strcmp(targetdir,"~")==0)
        targetdir=(char*)hdirectory;
    else if(strcmp(targetdir,".")==0)
        targetdir=(char*)presentdirectory;
    else if(strcmp(targetdir,"-")==0 && strcmp(prev_directory,"")!=0)
        targetdir=(char*)prev_directory;
    else if(strcmp(targetdir,"-")==0 && strcmp(prev_directory,"")==0)   
    {
        printf(COLOR_RED "Error No previous targetdir\n" COLOR_RESET);
        return;
    } 
    else
    {
        char full_path[PATH_MAX];
        if(targetdir[0]=='.')
        {
            memmove(targetdir,targetdir+2,strlen(targetdir)-1);
            snprintf(full_path,sizeof(full_path),"%s/%s",hdirectory,targetdir);
            targetdir=realpath(full_path,NULL);
        }
    }
    spec_8_seek_search(f_flag,d_flag,e_flag,targetdir,search,targetdir,prev_directory);
}