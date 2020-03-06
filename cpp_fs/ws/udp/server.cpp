#include <sys/socket.h> 
#include <iostream>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <cstring> /* memset */

#define PROTOCOL (0)
#define Q_LEN (5)
#define PORT (4651)
#define BUFF_SIZE (5)

int main()
{
    struct sockaddr_in addr, client_addr;
    char buff[5] = "Ping";
    char read_buff[5];
    
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    memset(&client_addr, 0, sizeof(client_addr));

    int sockfd = socket(AF_INET, SOCK_DGRAM , PROTOCOL);
    if (-1 == sockfd)
    {
        perror("socket");
    }
    
    if(-1 == bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)))
    {
        perror("bind");
    }

    socklen_t size = sizeof(client_addr);
    for (int i = 0; i < 20; ++i)
    {
        recvfrom(sockfd, read_buff, BUFF_SIZE, MSG_WAITALL, (struct sockaddr *)&client_addr, &size);
        std::cout << "buff = " << read_buff << std::endl;
        sendto(sockfd, buff, BUFF_SIZE, MSG_CONFIRM, (struct sockaddr *)&client_addr, sizeof(client_addr));
    }

    close(sockfd);
}