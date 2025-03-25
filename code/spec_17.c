#include "shell.h"

#define COLOR_RESET "\x1b[0m"
#define COLOR_RED "\033[1;31m"

void remove_html_tags(char *str) 
{
    char *src=str;
    char *dst=str;
    int in_tag=0;
    while(*src)
    {
        if(*src=='<') 
            in_tag = 1;
        else if(*src=='>') 
        {
            in_tag = 0;
            src++;
            continue;
        }
        if(!in_tag)
            *dst++ = *src;
        src++;
    }
    *dst = '\0';
}

void spec_17_iman(char *instruction)
{
    char *host="man.he.net";
    int port=80;
    char request[512];
    char buff[4096];
    int sock;
    struct sockaddr_in database;
    struct hostent *he;

    if((he=gethostbyname(host))==NULL) 
    {
        printf(COLOR_RED"Error occurred while getting host ip\n"COLOR_RESET);
        return;
    }

    if((sock=socket(AF_INET,SOCK_STREAM,0))<0) 
    {
        printf(COLOR_RED"Error occurred while creating socket\n"COLOR_RESET);
        return;
    }

    memset(&database,0,sizeof(database));
    database.sin_family=AF_INET;
    database.sin_port=htons(port);
    memcpy(&database.sin_addr,he->h_addr_list[0],he->h_length);

    if(connect(sock,(struct sockaddr *)&database,sizeof(database))< 0) 
    {
        printf(COLOR_RED"Error occurred while connecting\n"COLOR_RESET);
        close(sock);
        return;
    }

    snprintf(request,sizeof(request),"GET /?topic=%s&section=all HTTP/1.0\r\nHost: %s\r\nConnection: close\r\n\r\n",instruction,host);
    if(send(sock,request,strlen(request),0)<0) 
    {
        printf(COLOR_RED"Error occurred while sending request\n"COLOR_RESET);
        close(sock);
        return;
    }

    int received_len;
    int header_complete_flag=0;

    while((received_len=recv(sock,buff,4095,MSG_NOSIGNAL))>0) 
    {
        buff[received_len]='\0';
        if (header_complete_flag==0) {
            char *data=strstr(buff,"\n\n");
            if(data) 
            {
                header_complete_flag=1;
                data=data+2;
                remove_html_tags(data);
                printf("%s",data);
            }
            else 
            {
                remove_html_tags(buff);
                printf("%s",buff);
            }
        } 
        else 
        {
            remove_html_tags(buff);
            printf("%s",buff);
        }
    }

    if (received_len<0) 
    {
        printf(COLOR_RED"Error while receiving data\n"COLOR_RESET);
    }

    close(sock);
    return;
}