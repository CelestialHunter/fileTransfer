#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <errno.h>
#include <unistd.h>
#include <strings.h> // bzero
#include <signal.h>
#include <stdlib.h>
#include <fcntl.h> // open file
#include <string.h> // strcmp
#define MY_PORT_ID 8008 /* a number > 5000 */

int sockid;
int running = 1;

char* srcDir = "server_source";

void createConnection()
{
    struct sockaddr_in ssock_addr;
    char msg[255];

    sockid = socket(AF_INET, SOCK_STREAM, 0);
    if (sockid < 0)
    {
        perror("Eroare creare socket");
        exit(-1);
    }

    bzero((char *) &ssock_addr, sizeof(ssock_addr));
    ssock_addr.sin_family = AF_INET;
    ssock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    ssock_addr.sin_port = htons(MY_PORT_ID);

    if (( bind(sockid, (struct sockaddr *) &ssock_addr, sizeof(ssock_addr)) < 0))
    {
        perror("Eroare de asociere");
        exit(-1);
    }

    if ( listen(sockid, 1) < 0)
    {
        perror("Eroare la ascultare");
        exit(-1);
    }
}

void closeConnection()
{
    close(sockid);
}

void intHandler(int sig)
{
    closeConnection();
    running = 0;
}

void sendList(int destSockId)
{
    char list[1024];
    char cmd[64];
    sprintf(cmd, "ls %s > temp.txt", srcDir);
    system(cmd);

    int listFile = open("temp.txt", O_RDONLY);
    bzero((char*)&list, sizeof(list));
    read(listFile, list, sizeof(list));

    close(listFile);
    remove("temp.txt");

    if(strlen(list) == 0)
        sprintf(list, "Directory is empty!\n");

    write(destSockId, list, sizeof(list));
}

void sendFile(int destSockId, char* fileName)
{
 // TODO
}

int main()
{
    int newsockid;
    char msg[255];

    createConnection();   

    while (running)
    {
        newsockid = accept(sockid, (struct sockaddr *) 0, (int *) 0);
        if (newsockid < 0)
        {
            perror("Eroare acceptare client");
            return -1;
        }
        if ((read(newsockid, &msg, sizeof(msg))) < 0)
        {
            perror("Eroare citire din socket");
            return -1;
        }

        printf("Mesaj primit: %s\n", msg);

        if(strcmp(msg, "ls") == 0)
        {
            sendList(newsockid);
        }
        else
        {
            sendFile(newsockid, msg);
        }


        close(newsockid);
    }
    return 0;
} 