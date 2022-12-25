#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>

#define MY_PORT_ID 8008 /* numar > 5000 */

int sockid;

char* getServHostAddr()
{
    char hostbuffer[256];
    struct hostent *host_entry;

    gethostname(hostbuffer, sizeof(hostbuffer));
    host_entry = gethostbyname(hostbuffer);

    return inet_ntoa(*((struct in_addr*)host_entry->h_addr_list[0]));
}

int createConnection()
{
    // host IP
    char* servHostAddr = getServHostAddr();
    struct sockaddr_in ssock_addr;

    // create socket
    if ((sockid = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Eroare creare socket.\n");
        exit(-1);
    }

    // create ssock_addr
    bzero((char *) &ssock_addr, sizeof(ssock_addr));
    ssock_addr.sin_family = AF_INET;
    ssock_addr.sin_port = htons(MY_PORT_ID);

    if(inet_pton(AF_INET, servHostAddr, &ssock_addr.sin_addr) <= 0)
    {
        printf("\nAdresa invalida!\n");
        exit(-1);
    }

    // connect to socket
    if (connect(sockid, (struct sockaddr *) &ssock_addr, sizeof(ssock_addr)) < 0)
    {
        printf("Eroare conectare la server.\n");
        exit(-1);
    }
}

void closeConnection()
{
    close(sockid);
}

void printHelp()
{
    printf("===========================\n");
    printf("Usage: \n");
    printf("\"client [help]\" - displays this page\n");
    printf("\"client ls\" - lists files on server directory\n");
    printf("\"client get <filename>\" - downloads file from server\n");
    printf("===========================\n");
}

void getList()
{
    char list[1024];
    char cmd[] = "ls";
    if ( (write(sockid, &cmd, sizeof(cmd))) < 0)
    {
        printf("Eroare scriere in socket");
        exit(-1);
    }

    bzero((char *) &list, sizeof(list));
    if ( (read(sockid, &list, sizeof(list))) < 0)
    {
        printf("Eroare citire din socket");
        exit(-1);
    }

    printf("===========================\n");
    printf("Lista:\n");
    printf("===========================\n");
    printf("%s\n", list);
    printf("===========================\n");
}

void downloadFile(char* fileName)
{
    char fn[255];
    strcpy(fn, fileName);
    // TODO
    if ( (write(sockid, &fn, sizeof(fn))) < 0)
    {
        printf("Eroare scriere in socket");
        exit(-1);
    }
}


int main(int argc, char** argv)
{
    switch(argc)
    {
        case 1:
            // display Help page
            printHelp();
            break;
        case 2:
            // check if command is "ls"
            if(strcmp(argv[1], "ls") == 0)
            {
                createConnection();
                getList();
                closeConnection();
            }
            else if(strcmp(argv[1], "help") == 0)
            {
                printHelp();
            }
            else
            {
                printf("Unknown command! Run \"client help\" for help.\n");
                return -1;
            }
            break;
        case 3:
            if(strcmp(argv[1], "get") == 0)
            {
                createConnection();
                downloadFile(argv[2]);
                closeConnection();
            }
            else
            {
                printf("Unknown command! Run \"client help\" for help.\n");
                return -1;
            }
            break;
        default:
            printf("Unknown command! Run \"client help\" for help.\n");
            return -1;
            break;
    }   

    return 0;
}