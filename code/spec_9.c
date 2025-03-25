#include"shell.h"

#define COLOR_RESET "\x1b[0m"
#define COLOR_RED "\033[1;31m"

void add_aliases(char *name1,char *name2)
{
    strcpy(aliasess[alias_count].alias,name1);
    strcpy(aliasess[alias_count].commandtoreplace,name2);
    alias_count++;
}

void spec_9_myshrc_edit()
{
    char input_aliases[1024];
    while(1)
    {
        fgets(input_aliases,sizeof(input_aliases),stdin);
        if(strncmp(input_aliases,"exit",4)==0)
            return;
        FILE *file=fopen(".myshrc","a");
        fprintf(file,"%s",input_aliases);
        fclose(file);  
        if(strncmp(input_aliases,"alias",5)==0 && strchr(input_aliases,'=')!=NULL)
        {
            char *name1=strtok(input_aliases+6,"=");
            char *name2=strtok(NULL,"\n");
            name1=remove_spaces(name1);
            name2=remove_spaces(name2);
            add_aliases(name1,name2);
        }
    }
}

void spec_9_myshrc()
{
    FILE *file=fopen(".myshrc","r");
    int ch;
    while((ch=fgetc(file))!=EOF)
        putchar(ch);
    fclose(file);

    spec_9_myshrc_edit();    
}

void spec_9_replace(char *input)
{
    char *result=malloc(1024*sizeof(char));
    char *token;

    result[0]='\0';
    token=strtok(input," \t\n");
    while(token!=NULL)
    {
        int replaced=0;
        for(int i=0;i<alias_count;i++)
        {
            if(strcmp(token,aliasess[i].alias)==0)
            {
                strcat(result,aliasess[i].commandtoreplace);
                replaced=1;
            }
        }
        if(!replaced)
            strcat(result,token);
        strcat(result," ");
        token=strtok(NULL," \t\n");    
    }
    strcat(result,"\n");
    strcpy(input,result);
    free(result);
}