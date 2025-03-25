# MINI PROJECT-1 :
# FINAL SUBMISSION
## Spec-1:

(Refer to spec_1.c)

Used `getlogin()` and `gethostname()` and `getcwd()` for getting username , systemhost and current directory 

if any of above function returns an error then a error message will be printed in red color 

and also while displaying name and sys host if previous command is a foreground process and it takes more than 2 seconds then command and time taken will be printed  

and also every directory file names are printed relative to shell code directory

## Spec-2:

(Refer to spec_2.c)

Management of the given instruction takes place here. Using `strtok()` we tokenize given instruction with `;`and store them in an array

stored array of commands are each tokenized with `&` and then the subcommands obtained will go to execute

the process will be forked and the given command will run in child process and if given command needs to run in background  the parent process will return and next command will go on if given command needs to run in foreground the parent process will wait until child gets completed and then returns

if given Command doesn't exist then it will print `Error in execvp()` in red text

## Spec-3:

(Refer to spec_3.c)

In hop i used `chdir()` to change the directory from current directory and and store the current directory in prev_directory

used Given defination for the symbols `~` `-` `.` `..` 

and also if no argument given then it will go to the home directory

**NOTE**: home directory means the directory in which code is running


## Spec-4:

(Refer to spec_4.c)

In reveal part it will take the directory from given arguments using `strtok()` and the symbols `~` `-` `.` `..` has their given defination

using dirent and stat structures i read the directory using `readdir()`  and after  storing the file names and permission set red from directory

using `qsort` i will sort them lexicographically 
and then by reading the flags given i will print the data 

The error messages will be printed if stat or dirent functions fails in color red
The directories and executables will be printed in color as asked

## Spec-5:

(Refer to spec_5.c)

in log commands i will create a file named log_history 
and store the commands that not start with log and the last executed command

using file system calls like `fopen()` `fread()` `fclose` and `fwrite` log commands are implemented

An error will be printed while executing log execute n command if n is greater than the number of commands that are present in log_history file

## Spec-6:

(Refer to spec_2.c)

as given the forground and background process run . if fore ground proces takes more than 2 sec time then it will print the warning in display section

and the background process pid and exiting status will be printed

## Spec-7:

(Refer to spec_7.c)

using file sys calls a file with path `proc/[pid]/status` will be opened and red from which we can scrap the data neded uding  `strtok()` and for process state we will comapare the values obtained from `tcgetpgrp` and `getpgid` if both of them are same then it was a foreground process and if not it will backround process 

## Spec-8:

(Refer to spec_8.c)

using the dirent and stat structures the code will open the target directory and read it with `opendir()` and `readdir()` respectively

it will search recursively for the search argument given

the directories and files will printed as per the color given

if there was an e_flag it will execute the file if it is executable print the file if not. and if it was a directory it will open it

## Spec-9:

(Refer to spec_9.c)

I have only implemented alias and not implemented any functions

For aliases user have to use alias key word and if we want to edit the file `.myshrc` user have to give `nano .myshrc` and this will print the previous alias and also user can add the new ones and then from file it will stored in a array of strcut 

and before passing any instruction it will check if any alias word is present or not if it is present it will change it

## Spec-10:

(Refer to spec_10.c)

i have used `freopen` for this specification for redirection the filestream like `stdout` and `stdin` 

using strtok the given instruction will split into 3 parts command, input file_stream , output_filestream

after the completion of process `freopen` will redirect `stdout` and `stdin` back to terminal

## Spec-11:

(Refer to spec_11.c)

Using strtok i have splitup and whole instruction with pipes to commands and using dup changing the file streams from one pipe end to other end

tokenization with `|` will be done in between `;` and `&`

## Spec-12.c:

(Refer to spec_2.c and sepc_11.c)

By completing both spec-10 and spec-11 ,spec-12 will be done 

## Spec-13.c:

(Refer to spec_13.c)

the programme will store every process that done in a array of structure and it will contain it's pid, background or not, instruction and status

for each pid the stat_file will readed  from which programme will extract each backrgound process status 

`qsort()` will be used to print all the processes lexicographically

## Spec-14.c:

(Refer to spec_14.c)

In this specification i had implemented ping, ctrl+D, ctrl+Z but i didn't implemented ctrl+C because of for implementation of ctrl+C we have to set groupid of background process different from foreground process 

If implementation of ctrl+C is done it was effecting spec-13(activities) and spec-7(proclore)

for ping part it will use `kill` function with given signal

## Spec-15.c:

(Refer to spec_15.c)

For fg the program just handover the terminal to required pid and waits for it to complete and then takes back the control but i can't implement fg for background process which are stopped due to other specification collision

For bg the program just use `SIGCOUNT` signal to start running the stopped background process

## Spec-16.c:

(Refer to spec_16.c)

for the specification i used forking where the child process will scan of `x` continuously and the parent process will print the most recent pid that will be obtained from get_most_recent_pid function 

for this specification i removed the repitation of pid as we have to print the pids in order of creation 

I have set the terminal to non conanical mode to take input as soon as possible without pressing the enter button and after scanning terminal will reset

## Spec-17.c:

(Refer to spec_17.c)

for the specification i have used sockets 

first i have connected with server using`connect` function and then send a request using `GET` and `send`  and after getting the data i moved pointer by 2 units such that headers will be removed

By usage of `remove_html_tags` function i have removed the all the tags and printed it

## ASSUMPTIONS:

* Maximum length of instruction is 1024
* in hop ~/test `~` refers to home 
* if there is no previous directory but hop - or reveal - has been done then error message will be printed
* erronous commands also stored in log
* log file will be created in shell home directory
* if the given instruction has log as one of command then it will not stored in log file
* the time in prompt if the previous command takes more than 2 seconds will be the time of last command not the whole instruction
* if an erronous command is passed then `Command not found` will be printed
* if given command as functions defined by me and a `&` at end no error message is printed
* using alias keyword in spec-9
* in log file original string will be stored not the chnaged one after alias command
* errors will be printed in file if the file has been redirected as `stdout`
* pipes will be run sequentially
* exit status for background process wil be printed even tho user pressed ctrl+D
  
## Use of AI:

![1](images/1.png?raw=true "1")

![2](images/2.png?raw=true "2")

![3](images/3.png?raw=true "3")

![4](images/4.png?raw=true "4")

![5](images/5.png?raw=true "5")

![6](images/6.png?raw=true "6")

![7](images/7.png?raw=true "7")

![8](images/8.png?raw=true "8")

![9](images/9.png?raw=true "9")

![10](images/10.png?raw=true "10")

![11](images/11.png?raw=true "11")

![12](images/12.png?raw=true "12")

![13](images/13.png?raw=true "13")

![14](images/14.png?raw=true "14")

![15](images/15.png?raw=true "15")

![16](images/16.png?raw=true "16")

![17](images/17.png?raw=true "17")

![18](images/18.png?raw=true "18")

![19](images/19.png?raw=true "19")

![20](images/20.png?raw=true "20")

![21](images/21.png?raw=true "21")