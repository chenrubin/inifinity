#include <sys/socket.h> 
#include <iostream>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <arpa/inet.h>

#define BUFF_SIZE (30)
#define PORT (4651)

int main()
{
    sockaddr_in server_addr;
    char buff[BUFF_SIZE] = "sending tcp";
    char read_buff[BUFF_SIZE]; 

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    int sockfd = socket(AF_INET, SOCK_STREAM , IPPROTO_TCP);
    if (-1 == sockfd)
    {
        perror("socket");
    }

	if (-1 == connect(sockfd, (sockaddr *)&server_addr, sizeof(server_addr)))
    {
        perror("connect");
    }

    socklen_t size = sizeof(server_addr);

    if (-1 == send(sockfd, buff, BUFF_SIZE, 0))
    {
        perror("client tcp send");
    }
    
	read(sockfd , read_buff, 256);
    std::cout << "buff = " << read_buff << std::endl;
}
    