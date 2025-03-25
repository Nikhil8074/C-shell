# PART-A
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
and store the commands that not have log as substring and the last executed command

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
