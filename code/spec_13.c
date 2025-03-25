#include"shell.h"

#define COLOR_RESET "\x1b[0m"
#define COLOR_RED "\033[1;31m"

int compare_by_name(const void *a,const void *b) 
{
    process *A=*(process **)a;
    process *B=*(process **)b;
    return strcmp(A->instruction,B->instruction); 
}


void spec_13_activities(process *processes[],int *count)
{
    int bgcount=0;
    process *bgprocess[1024];
    for(int i=0;i<(*count);i++)
    {
        char proc_path[PATH_MAX];
        FILE *stat_file;
        snprintf(proc_path,sizeof(proc_path),"/proc/%d/stat",processes[i]->pid);
        stat_file=fopen(proc_path,"r");
        if(stat_file==NULL)
        {
            continue;
        }
        fscanf(stat_file,"%*d %*s %c",&processes[i]->status);
        fclose(stat_file);
        if(processes[i]->background==1)
        {
            bgprocess[bgcount]=malloc(sizeof(process));
            bgprocess[bgcount]->pid=processes[i]->pid;
            strncpy(bgprocess[bgcount]->instruction,processes[i]->instruction,sizeof(bgprocess[bgcount]->instruction)-1);
            bgprocess[bgcount]->instruction[sizeof(bgprocess[bgcount]->instruction)-1] = '\0';
            bgprocess[bgcount]->background=1;
            bgprocess[bgcount]->status=processes[i]->status;
            bgcount++;
        }
    }
    qsort(bgprocess,bgcount,sizeof(process*),compare_by_name);
    for(int i=0;i<bgcount;i++)
    {
        if(bgprocess[i]->status=='T')
            printf("%d : %s - Stopped\n",bgprocess[i]->pid,bgprocess[i]->instruction);
        else
            printf("%d : %s - Running\n",bgprocess[i]->pid,bgprocess[i]->instruction);   
        free(bgprocess[i]);    
    }
}