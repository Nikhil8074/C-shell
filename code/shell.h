#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<bits/local_lim.h>
#include<dirent.h>
#include<sys/time.h>
#include<sys/stat.h>
#include<pwd.h>
#include<grp.h>
#include<ctype.h>
#include<time.h>
#include<wait.h>
#include<errno.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<signal.h>
#include<libgen.h>
#include<pthread.h>
#include<termios.h>

typedef struct process
{
    int pid;
    char instruction[1024];
    char status;
    int background;
}process;

typedef struct aliases
{
    char alias[1024];
    char commandtoreplace[1024];
}aliases;

extern int alias_count;
extern aliases aliasess[1024];
extern int present_pid;
extern volatile sig_atomic_t running;
extern int shell_gpid;
extern int ctrl;

char* remove_spaces(char* str);
void remove_html_tags(char *str);
void shell_execution(char *instruction,char *hdirectory,char *prev_directory,int history,int *numberback,char *last_command,int *last_command_duration,process *processes[],int *count);
void spec_1_display(char home[],char *last_command,int *last_command_duration);
void spec_2_input(char *instruction,char *home,char *prev,int history,int *numberback,char *last_command,int *last_command_duration,process *processes[],int *count);
void spec_2_execute(char* instruction,int background,char *hdirectory,char *prev_directory,int history,int *numberback,char *last_command,int *last_command_duration,process *processes[],int *count);
void spec_3_hop(char *dest,char *home,char *prev);
void spec_4_reveal(char *flagandpath,char *home,char *prev);
void spec_4_reveal_extract(char *fullpath,int a_flag,int l_flag);
void spec_4_reveal_print(int l_flag,struct stat fileinfo,struct dirent *file);
void spec_5_log(char* instruct,char *home);
void spec_5_log_print(char *home);
void spec_5_log_purge(char *home);
void spec_5_log_execute(char *home,int n,char *prev_directory,int *numberback,char *last_command,int *last_command_duration,process *processes[],int *count);
void spec_7_proclore(int pid,char *hdirectory,process *processes[],int *count);
void spec_8_seek(char *instruct,char *hdirectory,char *prev_directory);
void spec_8_seek_search(int f_flag,int d_flag,int e_flag,char *targetdir,char *search,char *rel,char *prev_directory);
void spec_9_myshrc();
void spec_9_replace(char *input);
void spec_10_redirect(char *instruction,int append,int write,int read,char *hdirectory,char *prev_directory,int *numberback,char *last_command,int *last_command_duration,process *processes[],int *count);
void spec_11_pipe(char *instruction,char *hdirectory,char *prev_directory,int *numberback,char *last_command,int *last_command_duration,process *processes[],int *count);
void spec_13_activities(process *processes[],int *count);
void spec_14_signals(int pid,int signal_number);
// void spec_14_ctrl_C(int sig);
void spec_14_ctrl_Z(int sig);
void spec_14_ctrl_D(process *processes[],int *count);
void spec_15_fg(char *pid_str,process *processes[],int *count);
void spec_15_bg(char *pid_str,process *processes[],int *count);
void spec_16_neonate_detect(char* x);
void spec_17_iman(char *instruction);