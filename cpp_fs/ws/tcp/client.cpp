#include <sys/socket.h> 
#include <iostream>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>

#define PROTOCOL (6)
#define BUFF_SIZE (5)
#define PORT (1237)

int main()
{
    sockaddr_in server_addr;
    char buff[5] = "Ping";
    char read_buff[5]; 

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    int sockfd = socket(AF_INET, SOCK_STREAM , PROTOCOL);
    if (-1 == sockfd)
    {
        perror("socket");
    }

    if (-1 == connect(sockfd, (sockaddr *)&server_addr, sizeof(server_addr)))
    {
        perror("connect");
    }

    for (int i = 0; i < 20; ++i)
    {
        send(sockfd, buff, BUFF_SIZE, 0);
        read(sockfd ,read_buff, 5);
        std::cout << "buff = " << read_buff << std::endl;
    }
}