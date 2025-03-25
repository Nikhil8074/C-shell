#include"shell.h"

int compare_entries(const void *a, const void *b) 
{
    return strcmp(((struct dirent **)a)[0]->d_name, ((struct dirent **)b)[0]->d_name);
}

#define COLOR_DIR "\x1b[34m" 
#define COLOR_EXEC "\x1b[32m"
#define COLOR_RESET "\x1b[0m"
#define COLOR_RED "\033[1;31m"

void spec_4_reveal_print(int l_flag,struct stat fileinfo,struct dirent *file)
{
    if(l_flag==1)
    {
        char permissions[11];
        permissions[0]=S_ISDIR(fileinfo.st_mode) ? 'd' : '-';
        permissions[1]=(fileinfo.st_mode & S_IRUSR) ? 'r' : '-';
        permissions[2]=(fileinfo.st_mode & S_IWUSR) ? 'w' : '-';
        permissions[3]=(fileinfo.st_mode & S_IXUSR) ? 'x' : '-';
        permissions[4]=(fileinfo.st_mode & S_IRGRP) ? 'r' : '-';
        permissions[5]=(fileinfo.st_mode & S_IWGRP) ? 'w' : '-';
        permissions[6]=(fileinfo.st_mode & S_IXGRP) ? 'x' : '-';
        permissions[7]=(fileinfo.st_mode & S_IROTH) ? 'r' : '-';
        permissions[8]=(fileinfo.st_mode & S_IWOTH) ? 'w' : '-';
        permissions[9]=(fileinfo.st_mode & S_IXOTH) ? 'x' : '-';
        permissions[10]='\0';

        struct passwd *pw=getpwuid(fileinfo.st_uid);
        struct group *gr=getgrgid(fileinfo.st_gid);
        char time[128];
        long size=fileinfo.st_size;
        long nlink=fileinfo.st_nlink;
        strftime(time,sizeof(time),"%b %d %H:%M",localtime(&fileinfo.st_mtime));
        printf("%s %ld %s %s %ld %s ",permissions,nlink,pw->pw_name,gr->gr_name,size,time);
        if(S_ISDIR(fileinfo.st_mode))
            printf(COLOR_DIR "%s\n" COLOR_RESET,file->d_name);
        else if((fileinfo.st_mode & S_IXUSR) || (fileinfo.st_mode & S_IXGRP) || (fileinfo.st_mode & S_IXOTH))   
            printf(COLOR_EXEC "%s\n" COLOR_RESET,file->d_name); 
        else
            printf("%s\n",file->d_name);    
    }
    else
    {
        if(S_ISDIR(fileinfo.st_mode))
            printf(COLOR_DIR "%s\n" COLOR_RESET,file->d_name);
        else if((fileinfo.st_mode & S_IXUSR) || (fileinfo.st_mode & S_IXGRP) || (fileinfo.st_mode & S_IXOTH))   
            printf(COLOR_EXEC "%s\n" COLOR_RESET,file->d_name); 
        else
            printf("%s\n",file->d_name);  
    }
}

void spec_4_reveal_extract(char *fullpath,int a_flag,int l_flag)
{
    DIR *repo;
    struct dirent *file;
    struct stat fileinfo;
    struct dirent *files[1024];
    int total=0;
    int count=0;
    
    if((repo=opendir(fullpath))==NULL)
    {
        printf(COLOR_RED "Error occured while opendir" COLOR_RESET);
        return;
    }
    while((file=readdir(repo))!=NULL)
    {
        if(a_flag==0 && file->d_name[0]=='.')
            continue;
        char filepath[4800];
        snprintf(filepath,sizeof(filepath),"%s/%s",fullpath,file->d_name);
        if(stat(filepath,&fileinfo)==-1)
        {
            printf(COLOR_RED "Error occured while stat" COLOR_RESET);
            return;
        }
        total=total+fileinfo.st_blocks;
        if(count<1024)
            files[count++]=file;
    }
    if(l_flag==1)
        printf("total %d\n",total/2);
    rewinddir(repo);

    qsort(files,count,sizeof(struct dirent *),compare_entries);

    for(int i=0;i<count;i++)
    {
        file=files[i];
        if(a_flag==0 && file->d_name[0]=='.')
            continue;
        char filepath[4800];
        snprintf(filepath,sizeof(filepath),"%s/%s",fullpath,file->d_name);
        if(stat(filepath,&fileinfo)==-1)
        {
            printf(COLOR_RED "Error occured while stat" COLOR_RESET);
            return;
        }
        spec_4_reveal_print(l_flag,fileinfo,file);
        
    }
}

void spec_4_reveal(char *flagandpath,char *home,char *prev)
{
    int a_flag=0;
    int l_flag=0;
    char *directory=".";
    char *token=NULL;
    token=strtok(flagandpath," ");
    while(token!=NULL)
    {
        if(strlen(token)!=1 && token[0]=='-')
        {
            if(strchr(token,'a')!=NULL)
                a_flag=1;
            if(strchr(token,'l')!=NULL)
                l_flag=1;   
        }
        else
        {
            directory=token;
        } 
        token=strtok(NULL," ");     
    }
    if(strcmp(directory,"~")==0)
        directory=(char*)home;
    else if(directory==NULL)
        directory="."; 
    else if(strcmp(directory,"-")==0 && strcmp(prev,"")!=0)
        directory=(char*)prev;
    else if(strcmp(directory,"-")==0 && strcmp(prev,"")==0)   
    {
        printf(COLOR_RED "Error No previous directory\n" COLOR_RESET);
        return;
    } 
    char fullpath[PATH_MAX];
    if(realpath(directory,fullpath)==NULL)
    {
        printf(COLOR_RED "Error occured while realpath" COLOR_RESET);
        return;
    }
    spec_4_reveal_extract(fullpath,a_flag,l_flag);
}
