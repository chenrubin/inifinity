#include <sys/socket.h> 
#include <iostream>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <arpa/inet.h>

#define PROTOCOL (0)
#define BUFF_SIZE (5)
#define PORT (4651)

int main()
{
    sockaddr_in server_addr;
    char buff[BUFF_SIZE] = "Pong";
    char read_buff[BUFF_SIZE]; 
    int broadcast = 1;    

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("10.1.255.255")/*INADDR_BROADCAST*/;
    server_addr.sin_port = htons(PORT);

    int sockfd = socket(AF_INET, SOCK_DGRAM , PROTOCOL);
    if (-1 == sockfd)
    {
        perror("socket");
    }

    if (-1 == setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)))
    {
        perror("setsockopt");
    }

    socklen_t size = sizeof(server_addr);

 //  sleep(10);

    if (-1 == sendto(sockfd, buff, BUFF_SIZE, MSG_CONFIRM, (struct sockaddr *)&server_addr, sizeof(server_addr)))
    {
        perror("client sendto");
    }
  //  recvfrom(sockfd, read_buff, BUFF_SIZE, MSG_WAITALL, (struct sockaddr *)&server_addr, &size);
  //  std::cout << "buff = " << read_buff << std::endl;
}
    