#include <iostream> /* cout, endl */
#include <stdio.h> /* perror */
#include <arpa/inet.h> /* recvfrom, sendto */

#define PROTOCOL (0)
#define BUFF_SIZE (30)
#define PORT_BC (4651)

int main()
{
    sockaddr_in server_addr;
    char buff[BUFF_SIZE] = "sending bc";
    char read_buff[BUFF_SIZE]; 
    int broadcast = 1;    

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_BROADCAST;
    server_addr.sin_port = htons(PORT_BC);

    int sockfd = socket(AF_INET, SOCK_DGRAM , IPPROTO_UDP);
    if (-1 == sockfd)
    {
        perror("socket");
    }

    if (-1 == setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)))
    {
        perror("setsockopt");
    }

    socklen_t size = sizeof(server_addr);

    if (-1 == sendto(sockfd, buff, BUFF_SIZE, MSG_CONFIRM, (struct sockaddr *)&server_addr, sizeof(server_addr)))
    {
        perror("client bc sendto");
    }
    
    if (-1 == recvfrom(sockfd, read_buff, BUFF_SIZE, MSG_WAITALL, (struct sockaddr *)&server_addr, &size))
	{
		perror("bc received: ");
	}
    std::cout << "buff = " << read_buff << std::endl;
}