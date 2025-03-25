#include<stdio.h>
#include<string.h>
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

void shell_execution(char *instruction,char *hdirectory,char *prev_directory,int history,int *numberback,char *last_command,int *last_command_duration);
void spec_1_display(char home[],char *last_command,int *last_command_duration);
void spec_2_input(char *instruction,char *home,char *prev,int history,int *numberback,char *last_command,int *last_command_duration);
void spec_2_execute(char* instruction,int background,char *hdirectory,char *prev_directory,int history,int *numberback,char *last_command,int *last_command_duration);
void spec_3_hop(char *dest,char *home,char *prev);
void spec_4_reveal(char *flagandpath,char *home,char *prev);
void spec_4_reveal_extract(char *fullpath,int a_flag,int l_flag);
void spec_4_reveal_print(int l_flag,struct stat fileinfo,struct dirent *file);
void spec_5_log(char* instruct,char *home);
void spec_5_log_print(char *home);
void spec_5_log_purge(char *home);
void spec_5_log_execute(char *home,int n,char *prev_directory,int *numberback,char *last_command,int *last_command_duration);
void spec_7_proclore(pid_t pid,char *hdirectory);
void spec_8_seek(char *instruct,char *hdirectory,char *prev_directory);
void spec_8_seek_search(int f_flag,int d_flag,int e_flag,char *targetdir,char *search,char *rel,char *prev_directory);
