#include <sys/socket.h> 
#include <iostream>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>

#define PROTOCOL (6)
#define Q_LEN (5)
#define PORT (3333)
#define BUFF_SIZE (5)

int main()
{
    struct sockaddr_in addr;
    socklen_t size = 0;
    char buff[5] = "Pong";
    char read_buff[5];
    
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    int sockfd = socket(AF_INET, SOCK_STREAM , PROTOCOL);
    if (-1 == sockfd)
    {
        perror("socket");
    }
    
    if(-1 == bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)))
    {
        perror("bind");
    }

    std::cout << "server before listen" << std::endl;
    if (-1 == listen(sockfd, Q_LEN))
    {
        perror("listen");
    }

    std::cout << "server after listen and before accept" << std::endl;
    size = sizeof(addr);
    int new_socket = accept(sockfd, (struct sockaddr *)&addr, &size);

    std::cout << "server after accept" << std::endl;
    for (int i = 0; i < 60; ++i)
    {
        read(new_socket , read_buff, 256);
        sleep(1);
        std::cout << "buff = " << read_buff << std::endl;
        send(new_socket, buff, BUFF_SIZE, 0);
    }

    close(sockfd);
}