#include"shell.h"

#define COLOR_RESET "\x1b[0m"
#define COLOR_RED "\033[1;31m"

volatile sig_atomic_t running=1;

int check(int pids[],int c_pid,int *len)
{
    for(int i=0;i<(*len);i++)
        if(pids[i]==c_pid)
            return 0;
    return 1;        
}

void set_terminal_non_canonical_mode() {
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void reset_terminal_mode() {
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

int get_most_recent_pid(int pids[],int *len) 
{
    DIR *proc_dir;
    struct dirent *entry;
    int recent_pid=-1;
    proc_dir=opendir("/proc");
    if(!proc_dir)
    {
        printf(COLOR_RED"Error in opendir\n"COLOR_RESET);
        return -1;
    }

    while((entry=readdir(proc_dir))!=NULL)
        if (isdigit(entry->d_name[0])) 
        {
            int pid=atoi(entry->d_name);
            if (pid>recent_pid && check(pids,pid,len))
                recent_pid = pid;
        }    
    closedir(proc_dir);
    pids[*len]=recent_pid;
    (*len)++;
    return recent_pid;
}


void signal_handle()
{
    running=0;
}

void spec_16_neonate_detect(char* x)
{
    int time=atoi(x);
    int pids[8192];
    int len=0;
    int pid_neo=fork();
    if (pid_neo<0) 
    {
        printf(COLOR_RED"Fork Failed\n"COLOR_RESET);
        return;
    } 
    else if(pid_neo==0) 
    {
        char ch;
        while(running)
        {
            set_terminal_non_canonical_mode();
            ch=getchar();
            if (ch=='x' || ch=='X') 
            {
                kill(getppid(),SIGUSR1);
                break;
            }
        }
        reset_terminal_mode();
        exit(0);
    } 
    else 
    {
        signal(SIGUSR1,signal_handle);
        while(running)
        {
            int recent_pid=get_most_recent_pid(pids,&len);
            if (recent_pid!=-1) 
                printf("%d\n",recent_pid);
            sleep(time);
        }

        wait(NULL);
    }
    return;
}