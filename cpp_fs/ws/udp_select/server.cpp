#include <sys/socket.h> 
#include <iostream>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <cstring> /* memset */
#include <arpa/inet.h>

#define PROTOCOL (0)
#define Q_LEN (5)
#define PORT_BC (4651)
#define PORT_TCP (5000)
#define PORT_UDP (6000)
#define STDIN (0)
#define BUFF_SIZE (30)

int main()
{
    struct sockaddr_in client_bc_addr, client_tcp_addr, client_udp_addr;
    char read_buff[5];
    int broadcast = 1;
    fd_set readfds;
    
    struct timeval tv;
    tv.tv_sec = 30;
    tv.tv_usec = 0;
    
    client_bc_addr.sin_family = AF_INET;
    client_bc_addr.sin_addr.s_addr = inet_addr("10.1.255.255"); /*INADDR_BROADCAST*/ /*INADDR_ANY*/;
    client_bc_addr.sin_port = htons(PORT_BC);

    client_tcp_addr.sin_family = AF_INET;
    client_tcp_addr.sin_addr.s_addr = INADDR_ANY;
    client_tcp_addr.sin_port = htons(PORT_TCP);

    client_udp_addr.sin_family = AF_INET;
    client_udp_addr.sin_addr.s_addr = INADDR_ANY;
    client_udp_addr.sin_port = htons(PORT_UDP);

    int sockfd_bc = socket(AF_INET, SOCK_DGRAM , IPPROTO_UDP);
    if (-1 == sockfd_bc)
    {
        perror("socket bc: ");
    }

    int sockfd_udp = socket(AF_INET, SOCK_DGRAM , IPPROTO_UDP);
    if (-1 == sockfd_udp)
    {
        perror("socket udp: ");
    }

    int sockfd_tcp = socket(AF_INET, SOCK_STREAM , IPPROTO_TCP);
    if (-1 == sockfd_tcp)
    {
        perror("socket tcp: ");
    }
    
    if (-1 == setsockopt(sockfd_bc, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)))
    {
        perror("setsockopt");
    }

    FD_SET(STDIN, &readfds);
    FD_SET(sockfd_bc, &readfds);
    FD_SET(sockfd_tcp, &readfds);
    FD_SET(sockfd_udp, &readfds);

    for (int i = 0; i < 30 ; ++i)
    {
        sleep(10);

        fd_set temp_set = readfds;
        if (-1 == select(sockfd_tcp + 1, &temp_set, NULL, NULL, &tv))
        {
            perror("select: ");
        }
        
        for (int i = 0; i < sockfd_tcp + 1; ++i)
        {
            if (FD_ISSET(i, &temp_set))
            {
                if (0 == i)
                {
                    std::cout << "A key was pressed" << std::endl; 
                }

                else if (i == sockfd_bc)
                {
                    std::cout << "sockfd_bc" << std::endl; 
                    char buff[] = "replay on bc";
                    if (-1 == sendto(sockfd_bc, buff, 
                                        BUFF_SIZE, MSG_CONFIRM, 
                                        (struct sockaddr *)&client_bc_addr, 
                                        sizeof(client_bc_addr)))
                    {
                        perror("server bc sendto");
                    }
                }
                    
                else if (i == sockfd_tcp)
                {
                    std::cout << "sockfd_tcp" << std::endl; 
                    char buff[] = "replay on tcp";
                    if (-1 == sendto(sockfd_tcp, buff, 
                                        BUFF_SIZE, MSG_CONFIRM, 
                                        (struct sockaddr *)&client_tcp_addr, 
                                        sizeof(client_tcp_addr)))
                    {
                        perror("server tcp sendto");
                    }
                }
                else if (i == sockfd_udp)
                {
                    std::cout << "sockfd_udp" << std::endl; 
                    char buff[] = "replay on udp";
                    if (-1 == sendto(sockfd_udp, buff, 
                                        BUFF_SIZE, MSG_CONFIRM, 
                                        (struct sockaddr *)&client_udp_addr, 
                                        sizeof(client_udp_addr)))
                    {
                        perror("server udp sendto");
                    }
                }
            } // end of -> if (FD_ISSET(i, &temp_set))
        }

        std::cout << "buff = " << read_buff << std::endl;
        
    }

    close(sockfd_bc);
    close(sockfd_udp);
    close(sockfd_tcp);
}