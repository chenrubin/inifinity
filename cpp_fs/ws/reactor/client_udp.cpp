#include <iostream> /* cout, endl */
#include <stdio.h> /* perror */
#include <arpa/inet.h> /* recvfrom, sendto */

#define BUFF_SIZE (30)
#define PORT (4651)

int main()
{
    sockaddr_in server_addr;
    char buff[BUFF_SIZE] = "client sending udp";
    char read_buff[BUFF_SIZE]; 

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    server_addr.sin_port = htons(PORT);

    int sockfd = socket(AF_INET, SOCK_DGRAM , IPPROTO_UDP);
    if (-1 == sockfd)
    {
        perror("socket");
    }

	socklen_t size = sizeof(server_addr);
    if (-1 == sendto(sockfd, buff, BUFF_SIZE, MSG_CONFIRM, (struct sockaddr *)&server_addr, size))
    {
        perror("client sendto");
    }
	
	if (-1 == recvfrom(sockfd, read_buff, BUFF_SIZE, MSG_WAITALL, (struct sockaddr *)&server_addr, &size))
	{
		perror("bc received: ");
	}
    std::cout << "buff = " << read_buff << std::endl;
}  